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

     void read();
     void open();
     void save();
     void createActions();
     void readTo(t_size t);
public slots:
     void value(int v){
         readTo((t_size)(v * field->getNumT() / 100));
         timeLabel->setText(QString::number((int)(v* field->getNumT() / 100)));
     }
     void addE(int i, int i1, int i2){
         QString q = QString("Choose edge: \n") + QString::number(field->node()[0](i)) + QString(" : ")
                 + QString::number(field->node()[1](i)) + QString("  -  ") + QString::number(field->node()[0](i1))
                 + QString(" : ") + QString::number(field->node()[1](i1)) + QString("\n1: № ") + QString::number(i)
                     + QString(" \n2: № ")+ QString::number(i1) + QString("\n№ cont ") + QString::number(i2)
                   ;
         listEdge->addItem(q);
     }
     void addD(int i, int i1){
         QString q = QString("Choose dot: \n") + QString::number(field->node()[0](i)) + QString(" : ")
                 + QString::number(field->node()[1](i)) +  QString("\n№ ") + QString::number(i)
                      + QString("\n№ cont ") + QString::number(i1);
         listEdge->addItem(q);
     }
protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU
     bool keypres(QKeyEvent *keyevent);
};

#endif // MAINWINDOW_H

