#include "mainwindow.h"
#include <QApplication>
#include <QIcon>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Icône de fenêtre et de barre des tâches (toutes plateformes)
    QIcon icone;
    for(int taille : {16, 24, 32, 48, 64, 128, 256, 512}) {
        icone.addFile(QString(":/icons/%1.png").arg(taille), QSize(taille, taille));
    }
    a.setWindowIcon(icone);
    a.setDesktopFileName("solisolve");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "solisolve" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;

    w.show();
    return a.exec();
}
