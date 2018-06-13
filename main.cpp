#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "mainwindow.h"

int main(int argc, char *argv[]){
    Q_INIT_RESOURCE(basicdrawing);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
