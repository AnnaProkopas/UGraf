#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    Q_INIT_RESOURCE(basicdrawing);
    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();
    w->show();
    w->read();
    return a.exec();
}
