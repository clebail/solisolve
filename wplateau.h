#ifndef WPLATEAU_H
#define WPLATEAU_H

#include <QWidget>
#include <QEasingCurve>
#include <QStringList>
#include <QVector>
#include "CPlateau.h"

class QPainter;
class QVariantAnimation;

class WPlateau : public QWidget
{
    Q_OBJECT
public:
    explicit WPlateau(QWidget *parent = nullptr);
    void setPlateau(const unsigned char *plateau, const QString &info);
    // Texte en gros, centré dans la zone d'infos (vide pour ne rien afficher)
    void setTitre(const QString &titre);
    // Histogramme des plateaux retenus par étape sous la grille, étape courante mise en valeur (-1 : aucune)
    void setHistogramme(const QVector<int> &valeurs, int courante);
    // Anime le coup sur le plateau affiché puis émet coupJoue()
    void joueCoup(CCoup coup, const QString &info, int duree);
    // Pilotage manuel de l'animation d'un coup (extraction image par image)
    void debuteCoup(CCoup coup, const QString &info);
    void setProgression(qreal t);
    void termineCoup(void);
signals:
    void coupJoue();
protected:
    virtual void	paintEvent(QPaintEvent *event);
private:
    void dessineBille(QPainter &painter, const QPointF &centre, qreal rayon);
    void dessineOmbre(QPainter &painter, const QPointF &centre, qreal rayon, qreal hauteur);
    void dessineTrou(QPainter &painter, const QPointF &centre, qreal taille);
    void dessinePlateau(QPainter &painter, const QPointF &centre, qreal taille);
    void dessineCoups(QPainter &painter, const QRectF &zone, const QFont &police);
    void dessineHistogramme(QPainter &painter, const QRectF &zone, const QFont &police);

    // Copie du plateau : l'appelant peut le modifier ou le détruire ensuite
    unsigned char plateau[NB_BILLE];
    QString info;
    QString titre;

    // Sous la grille : coups joués en notation (mode solution) ou histogramme (mode solve)
    QStringList coupsJoues;
    QString coupEnCours;
    QVector<int> histogramme;
    int etapeCourante = -1;

    // Coup en cours d'animation (animDepuis à -1 si aucun)
    QVariantAnimation *animation;
    CCoup coupAnime;
    int animDepuis = -1;
    int animSautee = -1;
    int animDest = -1;
    qreal progression = 0;
    QEasingCurve courbe = QEasingCurve(QEasingCurve::InOutSine);
};

#endif // WPLATEAU_H
