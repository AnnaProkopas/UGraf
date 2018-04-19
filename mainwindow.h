#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QtWidgets>
#include <iostream>
#include <string> // подключаем строки
#include <fstream> // подключаем файлы
#include <cstdint>
#include <QGuiApplication>
#include <QImage>

class QAction;
class QMenu;
/*class QPlainTextEdit;
class QSessionManager;*/
class RenderArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     MainWindow();
    ~MainWindow();
     void read();
private:
     QMenu *fileMenu;
     RenderArea *renderArea;
     QImage image;
     QAction *openAct;
     QAction *saveAct;
     //QLabel *infoLabel;

     void open();
     void save();
     void createActions();
protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU
     bool keypres(QKeyEvent *keyevent);
};

#endif // MAINWINDOW_H

