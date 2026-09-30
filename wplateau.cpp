#include <QPainter>
#include <QVariantAnimation>
#include <QtMath>
#include <string.h>
#include "wplateau.h"
#include "CSolver.h"

// Couleur et proportions reprises de anim/anim.svg (case de 50, bille de rayon 15)
#define COULEUR_BILLE		QColor("#b39ddb")
#define RATIO_RAYON_BILLE	0.3
#define RATIO_RAYON_TROU	0.36

// Zone d'infos en haut : police à chasse fixe, taille et nombre de lignes fixes pour que le plateau ne bouge pas
#define COULEUR_TEXTE		QColor("#4527a0")
#define TAILLE_POLICE		14
#define NB_LIGNES_INFO		4
#define MARGE_TEXTE			6
#define TAILLE_TITRE		40

// Zone sous la grille (notation des coups ou histogramme) : hauteur minimale, le plateau rétrécit pour la laisser
#define HAUTEUR_PANNEAU_MIN	150
#define TAILLE_HISTORIQUE	22
#define NB_HISTORIQUE		4

// Plateau rond (en cases) : rayon du disque et du sillon intérieur
#define COULEUR_PLATEAU		QColor("#ede7f6")
#define RAYON_PLATEAU		3.85
#define RAYON_SILLON		3.6

// Saut : hauteur max (en cases) et grossissement de la bille au sommet
#define HAUTEUR_SAUT		0.35
#define ZOOM_SAUT			0.3
// Moment du saut (0..1) où la bille sautée commence puis finit de disparaître
#define DEBUT_DISPARITION	0.35
#define FIN_DISPARITION		0.9

WPlateau::WPlateau(QWidget *parent) : QWidget{parent} {
    // Sans solveur lancé : plateau vide, que des trous
    memcpy(plateau, modele, NB_BILLE * sizeof(unsigned char));

    // Animation linéaire : la courbe est appliquée par setProgression()
    animation = new QVariantAnimation(this);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);

    connect(animation, &QVariantAnimation::valueChanged, this, [this](const QVariant &valeur) {
        setProgression(valeur.toReal());
    });
    connect(animation, &QVariantAnimation::finished, this, [this]() {
        termineCoup();
        emit coupJoue();
    });
}

// Taille de police (au plus TAILLE_TITRE) pour que le texte tienne dans la largeur donnée
static int tailleQuiTient(QFont police, const QString &texte, qreal largeur) {
    int taille = TAILLE_TITRE;

    police.setPixelSize(taille);
    while(taille > TAILLE_POLICE && QFontMetricsF(police).horizontalAdvance(texte) > largeur) {
        police.setPixelSize(--taille);
    }

    return taille;
}

// Notation d'un coup : case de départ -> case d'arrivée (ex. "D4 → B4")
static QString notation(CCoup coup) {
    int depuis = coup.getDepuis();
    int dest = depuis + coup.getPas() * 2;

    return QString("%1%2 → %3%4").arg(QChar('A' + depuis % NB_COLONNE)).arg(depuis / NB_COLONNE + 1)
                                  .arg(QChar('A' + dest % NB_COLONNE)).arg(dest / NB_COLONNE + 1);
}

void WPlateau::setPlateau(const unsigned char *plateau, const QString &info) {
    animation->stop();
    animDepuis = animSautee = animDest = -1;
    coupsJoues.clear();
    coupEnCours.clear();
    memcpy(this->plateau, plateau, NB_BILLE * sizeof(unsigned char));
    this->info = info;
    update();
}

void WPlateau::setTitre(const QString &titre) {
    this->titre = titre;
    update();
}

void WPlateau::setHistogramme(const QVector<int> &valeurs, int courante) {
    histogramme = valeurs;
    etapeCourante = courante;
    update();
}

void WPlateau::joueCoup(CCoup coup, const QString &info, int duree) {
    debuteCoup(coup, info);
    animation->setDuration(duree);
    animation->start();
}

void WPlateau::debuteCoup(CCoup coup, const QString &info) {
    coupAnime = coup;
    animDepuis = coup.getDepuis();
    animSautee = animDepuis + coup.getPas();
    animDest = animDepuis + coup.getPas() * 2;
    progression = 0;
    coupEnCours = notation(coup);
    this->info = info;
    update();
}

// t de 0 à 1, linéaire dans le temps
void WPlateau::setProgression(qreal t) {
    progression = courbe.valueForProgress(t);
    update();
}

void WPlateau::termineCoup(void) {
    coupAnime.joue(plateau);
    animDepuis = animSautee = animDest = -1;
    coupsJoues.append(coupEnCours);
    coupEnCours.clear();
    update();
}

// Bille "brillante" : contour foncé, dégradé radial décalé vers le haut à gauche et reflet
void WPlateau::dessineBille(QPainter &painter, const QPointF &centre, qreal rayon) {
    QColor couleur = COULEUR_BILLE;
    QRadialGradient degrade(centre, rayon, centre - QPointF(rayon, rayon) * 0.3);

    degrade.setColorAt(0, couleur.lighter(115));
    degrade.setColorAt(0.7, couleur);
    degrade.setColorAt(1, couleur.darker(115));

    painter.setPen(QPen(couleur.darker(150), rayon * 0.12));
    painter.setBrush(degrade);
    painter.drawEllipse(centre, rayon, rayon);

    QPointF centreReflet = centre - QPointF(rayon, rayon) * 0.4;
    QRadialGradient reflet(centreReflet, rayon * 0.3);

    reflet.setColorAt(0, QColor(255, 255, 255, 220));
    reflet.setColorAt(0.6, QColor(255, 255, 255, 160));
    reflet.setColorAt(1, QColor(255, 255, 255, 0));

    painter.setPen(Qt::NoPen);
    painter.setBrush(reflet);
    painter.drawEllipse(centreReflet, rayon * 0.3, rayon * 0.3);
}

// Ombre portée au sol d'une bille en l'air : plus elle est haute, plus l'ombre est large et pâle
void WPlateau::dessineOmbre(QPainter &painter, const QPointF &centre, qreal rayon, qreal hauteur) {
    qreal r = rayon * (1 + hauteur * 0.3);
    QRadialGradient degrade(centre, r);

    degrade.setColorAt(0, QColor(0, 0, 0, 90 * (1 - hauteur * 0.6)));
    degrade.setColorAt(1, QColor(0, 0, 0, 0));

    painter.setPen(Qt::NoPen);
    painter.setBrush(degrade);
    painter.drawEllipse(centre, r, r);
}

// Trou creusé dans le plateau : éclairé par le haut à gauche, donc paroi haute dans l'ombre
void WPlateau::dessineTrou(QPainter &painter, const QPointF &centre, qreal taille) {
    qreal r = taille * RATIO_RAYON_TROU;
    QLinearGradient degrade(centre - QPointF(r, r) * 0.7, centre + QPointF(r, r) * 0.7);

    degrade.setColorAt(0, COULEUR_PLATEAU.darker(160));
    degrade.setColorAt(1, COULEUR_PLATEAU.darker(105));

    painter.setPen(QPen(COULEUR_PLATEAU.darker(120), r * 0.06));
    painter.setBrush(degrade);
    painter.drawEllipse(centre, r, r);
}

// Disque du plateau avec ombre portée, bord et sillon gravé près du bord
void WPlateau::dessinePlateau(QPainter &painter, const QPointF &centre, qreal taille) {
    qreal r = taille * RAYON_PLATEAU;
    qreal rSillon = taille * RAYON_SILLON;
    QPointF decalage(taille * 0.04, taille * 0.08);
    QRadialGradient ombre(centre + decalage, r * 1.05);

    ombre.setColorAt(0.9, QColor(0, 0, 0, 60));
    ombre.setColorAt(1, QColor(0, 0, 0, 0));
    painter.setPen(Qt::NoPen);
    painter.setBrush(ombre);
    painter.drawEllipse(centre + decalage, r * 1.05, r * 1.05);

    QRadialGradient degrade(centre, r, centre - QPointF(r, r) * 0.4);

    degrade.setColorAt(0, COULEUR_PLATEAU.lighter(106));
    degrade.setColorAt(1, COULEUR_PLATEAU.darker(106));
    painter.setPen(QPen(COULEUR_PLATEAU.darker(140), taille * 0.04));
    painter.setBrush(degrade);
    painter.drawEllipse(centre, r, r);

    // Sillon : trait sombre doublé d'un reflet clair décalé vers le bas à droite
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(Qt::white, taille * 0.03));
    painter.drawEllipse(centre + QPointF(taille, taille) * 0.02, rSillon, rSillon);
    painter.setPen(QPen(COULEUR_PLATEAU.darker(135), taille * 0.03));
    painter.drawEllipse(centre, rSillon, rSillon);
}

void WPlateau::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int x, y, idx;
    QPointF centreDepuis, centreDest;

    painter.setRenderHint(QPainter::Antialiasing);

    QFont police("monospace");

    police.setStyleHint(QFont::TypeWriter);
    police.setFixedPitch(true);
    police.setPixelSize(TAILLE_POLICE);
    painter.setFont(police);
    painter.setPen(COULEUR_TEXTE);

    int hauteurTexte = MARGE_TEXTE * 2 + painter.fontMetrics().lineSpacing() * NB_LIGNES_INFO;
    // Bande de texte en haut (stats à gauche, numéro du coup centré), plateau en dessous le plus grand possible
    // en laissant HAUTEUR_PANNEAU_MIN en bas, centré verticalement dans le widget tant qu'il n'empiète sur rien
    qreal diametre = RAYON_PLATEAU * 2 + 0.2;
    qreal taille = qMin((qreal)width(), (qreal)height() - hauteurTexte - HAUTEUR_PANNEAU_MIN) / diametre;
    qreal demi = taille * diametre / 2;
    QPointF centre(width() / 2.0, qBound(hauteurTexte + demi, height() / 2.0, height() - HAUTEUR_PANNEAU_MIN - demi));
    QRectF zoneTexte(MARGE_TEXTE, MARGE_TEXTE, width() - MARGE_TEXTE * 2, hauteurTexte - MARGE_TEXTE * 2);

    // Panneau (histogramme ou coups) toujours sous le plateau, sur toute la largeur
    qreal basPlateau = centre.y() + demi;
    QRectF zonePanneau(MARGE_TEXTE, basPlateau + MARGE_TEXTE, width() - MARGE_TEXTE * 2, height() - basPlateau - MARGE_TEXTE * 2);

    painter.drawText(zoneTexte, Qt::AlignLeft | Qt::AlignTop | Qt::TextDontClip, info);

    if(!titre.isEmpty()) {
        QFont policeTitre = police;

        policeTitre.setBold(true);
        policeTitre.setPixelSize(tailleQuiTient(policeTitre, titre, zoneTexte.width()));
        painter.setFont(policeTitre);
        painter.drawText(zoneTexte, Qt::AlignCenter, titre);
    }

    qreal offsetX = centre.x() - taille * NB_COLONNE / 2;
    qreal offsetY = centre.y() - taille * NB_LIGNE / 2;
    qreal rayon = taille * RATIO_RAYON_BILLE;

    dessinePlateau(painter, centre, taille);

    for(y=idx=0;y<NB_LIGNE;y++) {
        for(x=0;x<NB_COLONNE;x++,idx++) {
            if(plateau[idx] == UNDEF) {
                continue;
            }

            QPointF centreCase(offsetX + (x + 0.5) * taille, offsetY + (y + 0.5) * taille);

            dessineTrou(painter, centreCase, taille);

            if(idx == animDepuis) {
                // La bille qui saute est dessinée à part, par-dessus tout le reste
                centreDepuis = centreCase;
            } else if(idx == animDest) {
                centreDest = centreCase;
            } else if(idx == animSautee) {
                // La bille sautée s'efface et rétrécit un peu pendant le passage
                qreal t = qBound(0.0, (progression - DEBUT_DISPARITION) / (FIN_DISPARITION - DEBUT_DISPARITION), 1.0);

                painter.save();
                painter.setOpacity(1 - t);
                dessineBille(painter, centreCase, rayon * (1 - t * 0.4));
                painter.restore();
            } else if(plateau[idx] == BILLE) {
                dessineBille(painter, centreCase, rayon);
            }
        }
    }

    if(animDepuis != -1) {
        // Trajectoire en cloche : la bille monte (vers le haut de l'écran), grossit et laisse son ombre au sol
        qreal hauteur = qSin(progression * M_PI);
        QPointF sol = centreDepuis + (centreDest - centreDepuis) * progression;
        QPointF enLair = sol - QPointF(0, hauteur * HAUTEUR_SAUT * taille);

        dessineOmbre(painter, sol, rayon, hauteur);
        dessineBille(painter, enLair, rayon * (1 + hauteur * ZOOM_SAUT));
    }

    if(!coupsJoues.isEmpty() || !coupEnCours.isEmpty()) {
        dessineCoups(painter, zonePanneau, police);
    } else if(!histogramme.isEmpty()) {
        dessineHistogramme(painter, zonePanneau, police);
    }
}

// Dernier coup en gros, les précédents en dessous, numérotés et de plus en plus pâles
void WPlateau::dessineCoups(QPainter &painter, const QRectF &zone, const QFont &police) {
    QStringList liste = coupsJoues;
    QFont policeCoup = police;
    QFont policeHistorique = police;

    if(!coupEnCours.isEmpty()) {
        liste.append(coupEnCours);
    }

    policeCoup.setBold(true);
    policeCoup.setPixelSize(tailleQuiTient(policeCoup, liste.last(), zone.width()));
    policeHistorique.setPixelSize(TAILLE_HISTORIQUE);

    painter.save();
    painter.setPen(COULEUR_TEXTE);
    painter.setFont(policeCoup);

    qreal y = zone.top();
    qreal hauteurCoup = QFontMetricsF(policeCoup).lineSpacing();
    qreal hauteurHistorique = QFontMetricsF(policeHistorique).lineSpacing();

    painter.drawText(QRectF(zone.left(), y, zone.width(), hauteurCoup), Qt::AlignCenter, liste.last());
    y += hauteurCoup;

    painter.setFont(policeHistorique);
    for(int i=1;i<=NB_HISTORIQUE && i<liste.size() && y + hauteurHistorique <= zone.bottom();i++) {
        int num = liste.size() - i;

        painter.setOpacity(0.7 - (i - 1) * 0.15);
        painter.drawText(QRectF(zone.left(), y, zone.width(), hauteurHistorique), Qt::AlignCenter,
                         QString("%1. %2").arg(num, 2).arg(liste[num - 1]));
        y += hauteurHistorique;
    }
    painter.restore();
}

// Une barre par étape (nombre de billes), échelle fixe jusqu'à MAX_TEST pour que les barres ne bougent pas
void WPlateau::dessineHistogramme(QPainter &painter, const QRectF &zone, const QFont &police) {
    QFontMetricsF fm(police);
    int nbEtapes = MAX_BILLE - 1;
    QString libelleMax = QString::number(MAX_TEST);
    qreal largeurLibelle = fm.horizontalAdvance(libelleMax) + MARGE_TEXTE;
    // Titre sur plusieurs lignes si la zone est étroite (colonne en paysage)
    QString titreHisto = "Plateaux retenus par nombre de billes";
    QRectF zoneTitre = fm.boundingRect(zone, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, titreHisto);
    qreal hautBarres = zoneTitre.bottom() + fm.lineSpacing() * 0.5;
    QRectF barres(zone.left() + largeurLibelle, hautBarres,
                  zone.width() - largeurLibelle, zone.bottom() - fm.lineSpacing() - hautBarres);
    qreal pas = barres.width() / nbEtapes;

    painter.save();
    painter.setFont(police);
    painter.setPen(COULEUR_TEXTE);
    painter.drawText(zone, Qt::AlignHCenter | Qt::AlignTop | Qt::TextWordWrap, titreHisto);

    // Axes : ligne du maximum en pointillés, ligne de base, graduations de billes
    painter.drawText(QRectF(zone.left(), barres.top() - fm.lineSpacing() / 2, largeurLibelle - MARGE_TEXTE, fm.lineSpacing()),
                     Qt::AlignRight | Qt::AlignVCenter, libelleMax);
    painter.drawText(QRectF(zone.left(), barres.bottom() - fm.lineSpacing() / 2, largeurLibelle - MARGE_TEXTE, fm.lineSpacing()),
                     Qt::AlignRight | Qt::AlignVCenter, "0");
    painter.setPen(QPen(COULEUR_PLATEAU.darker(140), 1, Qt::DashLine));
    painter.drawLine(barres.topLeft(), barres.topRight());
    painter.setPen(QPen(COULEUR_TEXTE, 1));
    painter.drawLine(barres.bottomLeft(), barres.bottomRight());
    for(int etape : {1, 10, 20, 30, nbEtapes}) {
        painter.drawText(QRectF(barres.left() + (etape - 1) * pas - pas, barres.bottom(), pas * 3, fm.lineSpacing()),
                         Qt::AlignCenter, QString::number(etape));
    }

    painter.setPen(Qt::NoPen);
    for(int i=0;i<histogramme.size() && i<nbEtapes;i++) {
        qreal h = barres.height() * qMin(histogramme[i], MAX_TEST) / MAX_TEST;

        painter.setBrush(i == etapeCourante ? COULEUR_TEXTE : COULEUR_BILLE);
        painter.drawRoundedRect(QRectF(barres.left() + i * pas + pas * 0.1, barres.bottom() - h, pas * 0.8, h), 2, 2);
    }
    painter.restore();
}
