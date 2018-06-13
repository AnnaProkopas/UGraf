#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "data.h"
#include "renderarea.h"
#include <QWidget>
#include <QPainter>
#include <QtWidgets>
#include <iostream>
#include <string> // подключаем строки
#include <fstream> // подключаем файлы
#include <cstdint>
#include <QGuiApplication>
#include <QImage>
#include <QDebug>

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
     QPlainTextEdit *chooseEdge;
     QImage image;
     QAction *openAct;
     QAction *saveAct;
     QSlider *timeSlider;
     QListWidget *listEdge;
     QLabel *timeLabel;
     //MainWindow *ui;

     uint32* shiftInFile;
     t_field *field = NULL;
     std::ifstream file;

     //void read();
     void open();
     void save();
     void createActions();
     void readTo(t_size t);
public slots:
     void value(int v){
         readTo((t_size)(v * field->getNumT() / 100));
         timeLabel->setText(QString::number((int)(v* field->getNumT() / 100)));
     }
     void addE(){
         QString q = renderArea->nowChoose();
         listEdge->addItem(q);
     }
     void addD(){
         QString q = renderArea->nowChooseD();
         listEdge->addItem(q);
     }
protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU
     bool keypres(QKeyEvent *keyevent);
};

#endif // MAINWINDOW_H

