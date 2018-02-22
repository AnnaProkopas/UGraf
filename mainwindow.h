#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>

#include "data.h"

class RenderArea;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
     MainWindow();
    ~MainWindow();
     void plot(t_real* &listX, t_real* &listY, uint32_t k1, uint32_t k2, uint32_t _size, uint32 isCircle);
     void getMin(double &mX, double &mY);
     void getMax(double &mX, double &mY);
private:
     RenderArea *renderArea;
};

#endif // MAINWINDOW_H

