#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <list>
#include "ui_mainwindow.h"
#include "CPlateau.h"
#include "CCoup.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    // Dernière solution trouvée : plateau de départ et coups dans l'ordre du jeu
    unsigned char depart[NB_BILLE];
    std::list<CCoup> coups;

    // Extraction des images du plateau : dossier, préfixe (solve/solution) et numéro de la prochaine image
    QString dossierImages;
    QString prefixeImage;
    int numImage = 0;

    // Taille de la fenêtre avant le passage en format short, restaurée en sortie
    QSize tailleAvantShort;

    void attend(int ms);
    void pause(int ms);
    void sauveImage(int nb = 1);
    void setCommandesActives(bool actives);
private slots:
    void on_pbSolve_clicked();
    void on_pbSolution_clicked();
    void on_cbImages_toggled(bool checked);
    void on_cbShort_toggled(bool checked);
};

#endif // MAINWINDOW_H
