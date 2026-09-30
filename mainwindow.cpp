#include <QCoreApplication>
#include <QDir>
#include <QEventLoop>
#include <QFileDialog>
#include <QScreen>
#include <QTimer>
#include <string.h>
#include <vector>
#include "mainwindow.h"
#include "CSolver.h"

// Mode solve : plateaux montrés à chaque étape (répartis parmi ceux retenus) et durée de chacun
#define PLATEAUX_PAR_ETAPE  15
#define DELAI_SOLVE         33

// Mode solution : intro (question d'accroche sur le plateau de départ), coups, puis arrêt sur la dernière bille
#define DUREE_INTRO     2000
#define TEXTE_INTRO     "Finir avec une seule bille ?"
#define DUREE_COUP      700
#define PAUSE_SOLUTION  200
#define DUREE_FIN       3000
#define TEXTE_FIN       "Plus qu'une bille !"

// Format short (vertical 9:16) : le plateau fait 540x960 si l'écran le permet, en multiples de 18x32 pour des dimensions paires
#define SHORT_LARGEUR   18
#define SHORT_HAUTEUR   32
#define SHORT_MAX       30

// Cadence des images extraites en mode solution (en mode solve : une image par plateau montré)
#define IMAGES_PAR_SECONDE  24

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUi(this);
}

MainWindow::~MainWindow() {
}

// Attente qui laisse Qt redessiner : le solveur et la solution tournent dans le thread GUI
void MainWindow::attend(int ms) {
    QEventLoop loop;

    QTimer::singleShot(ms, &loop, &QEventLoop::quit);
    loop.exec();
}

// Une ligne d'info alignée : libellé sur 8 caractères puis valeur calée à droite sur 14
static QString ligneInfo(const QString &libelle, const QString &valeur) {
    return QString("%1 : %2").arg(libelle, -8).arg(valeur, 14);
}

// Statistiques du solveur pour un plateau : billes, rang parmi les plateaux retenus et poids (37 bits, en hexa)
static QString infoSolve(int nbBille, size_t num, size_t total, unsigned long poids) {
    return ligneInfo("Billes", QString("%1 / %2").arg(nbBille, 4).arg(MAX_BILLE, 4)) + "\n" +
           ligneInfo("Plateau", QString("%1 / %2").arg(num, 4).arg(total, 4)) + "\n" +
           ligneInfo("Poids", "0x" + QString("%1").arg(poids, 12, 16, QChar('0')).toUpper());
}

// En extraction, la pause devient des images fixes (pas besoin d'attendre réellement)
void MainWindow::pause(int ms) {
    if(cbImages->isChecked()) {
        sauveImage(ms * IMAGES_PAR_SECONDE / 1000);
    } else {
        attend(ms);
    }
}

// Enregistre nb fois l'état courant du plateau : <prefixe>_00000.png, <prefixe>_00001.png...
void MainWindow::sauveImage(int nb) {
    QPixmap image = wplateau->grab();
    QDir dossier(dossierImages);

    for(int i=0;i<nb;i++) {
        image.save(dossier.filePath(QString("%1_%2.png").arg(prefixeImage).arg(numImage++, 5, 10, QChar('0'))));
    }
    QCoreApplication::processEvents();
}

void MainWindow::setCommandesActives(bool actives) {
    pbSolve->setEnabled(actives);
    pbSolution->setEnabled(actives && !coups.empty());
    cbImages->setEnabled(actives);
    cbShort->setEnabled(actives);
}

// Redimensionne la fenêtre pour que le plateau (et donc les images extraites) soit au format short, taille bloquée
void MainWindow::on_cbShort_toggled(bool checked) {
    if(checked) {
        // Place prise autour du plateau : barre de commande, marges, décorations de la fenêtre
        QSize autour = size() - wplateau->size();
        int decoration = frameGeometry().height() - height();
        QRect ecran = screen()->availableGeometry();
        int n = qMin(SHORT_MAX, (ecran.height() - autour.height() - decoration) / SHORT_HAUTEUR);
        // La barre de commande impose une largeur minimale : on ne descend pas en dessous pour garder le ratio exact
        int nMin = (minimumSizeHint().width() - autour.width() + SHORT_LARGEUR - 1) / SHORT_LARGEUR;

        n = qMax(n, nMin);
        tailleAvantShort = size();
        setFixedSize(SHORT_LARGEUR * n + autour.width(), SHORT_HAUTEUR * n + autour.height());
    } else {
        setMinimumSize(0, 0);
        setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        resize(tailleAvantShort);
    }
}

void MainWindow::on_cbImages_toggled(bool checked) {
    if(checked) {
        QString dossier = QFileDialog::getExistingDirectory(this, "Dossier des images", dossierImages);

        if(dossier.isEmpty()) {
            cbImages->setChecked(false);
        } else {
            dossierImages = dossier;
        }
    }
}

void MainWindow::on_pbSolve_clicked() {
    int dernierNbBille = 0;
    size_t dernierTotal = 0;
    QVector<int> histogramme(MAX_BILLE - 1, 0);
    CSolver solver([&](CPlateaux *plateaux, int nbBille) {
        std::vector<CPlateau *> tous(plateaux->begin(), plateaux->end());

        dernierNbBille = nbBille;
        dernierTotal = tous.size();
        histogramme[nbBille - 1] = tous.size();
        wplateau->setHistogramme(histogramme, nbBille - 1);
        for(int i=0;i<PLATEAUX_PAR_ETAPE;i++) {
            // Aux premières étapes il y a moins de plateaux que d'images : certains sont répétés
            size_t idx = (size_t)i * tous.size() / PLATEAUX_PAR_ETAPE;
            CPlateau *plateau = tous[idx];

            wplateau->setPlateau(plateau->getPlateau(), infoSolve(nbBille, idx + 1, tous.size(), plateau->getPoids()));
            if(cbImages->isChecked()) {
                sauveImage();
            } else {
                attend(DELAI_SOLVE);
            }
        }
    });

    wplateau->setTitre("");
    wplateau->setHistogramme(QVector<int>(), -1);
    prefixeImage = "solve";
    numImage = 0;
    coups.clear();
    setCommandesActives(false);
    solver.process();

    CPlateau *solution = solver.getSolution();

    if(solution != 0) {
        std::list<CCoup> tous = solution->getCoups();

        memcpy(depart, solution->getPlateau(), NB_BILLE * sizeof(unsigned char));
        for(std::list<CCoup>::iterator it=tous.begin();it!=tous.end();it++) {
            if(!(*it).isNull()) {
                coups.push_back(*it);
            }
        }
        // La solution est le premier plateau de la dernière étape : on garde ses stats et on ajoute les coups
        wplateau->setPlateau(depart, infoSolve(dernierNbBille, 1, dernierTotal, solution->getPoids()) + "\n" +
                                     ligneInfo("Coups", QString::number(coups.size())));
    } else {
        wplateau->setPlateau(modele, ligneInfo("Coups", "aucune solution"));
    }

    wplateau->setHistogramme(histogramme, -1);
    setCommandesActives(true);
}

void MainWindow::on_pbSolution_clicked() {
    int numCoup = 0;

    prefixeImage = "solution";
    numImage = 0;
    setCommandesActives(false);

    // Le numéro du coup s'affiche en gros pour rester lisible en vidéo, les coups en notation sous la grille
    wplateau->setHistogramme(QVector<int>(), -1);
    wplateau->setPlateau(depart, "");
    wplateau->setTitre(TEXTE_INTRO);
    pause(DUREE_INTRO);
    wplateau->setTitre(QString("Coup %1 / %2").arg(0, 2).arg(coups.size()));
    pause(PAUSE_SOLUTION);

    for(std::list<CCoup>::iterator it=coups.begin();it!=coups.end();it++) {
        wplateau->setTitre(QString("Coup %1 / %2").arg(++numCoup, 2).arg(coups.size()));
        if(cbImages->isChecked()) {
            // Image par image à cadence fixe, indépendamment du timer de l'animation
            int nbImages = DUREE_COUP * IMAGES_PAR_SECONDE / 1000;

            wplateau->debuteCoup(*it, "");
            for(int i=0;i<nbImages;i++) {
                wplateau->setProgression((qreal)i / nbImages);
                sauveImage();
            }
            wplateau->termineCoup();
        } else {
            QEventLoop loop;

            connect(wplateau, &WPlateau::coupJoue, &loop, &QEventLoop::quit);
            wplateau->joueCoup(*it, "", DUREE_COUP);
            loop.exec();
        }
        pause(PAUSE_SOLUTION);
    }

    wplateau->setTitre(TEXTE_FIN);
    pause(DUREE_FIN);

    setCommandesActives(true);
}
