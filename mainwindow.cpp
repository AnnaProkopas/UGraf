
#include "renderarea.h"
#include "mainwindow.h"

#include <QtWidgets>

const int IdRole = Qt::UserRole;

MainWindow::MainWindow()
{

    renderArea = new RenderArea();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    setLayout(mainLayout);

    setWindowTitle(tr("Basic Drawing"));
}

void MainWindow::plot(t_real* &listX, t_real* &listY, uint32_t k1, uint32_t k2, uint32_t _size, uint32 isCircle){

    renderArea->plot(listX, listY, k1, k2, _size, isCircle);
}

void MainWindow::getMin(double &mX, double &mY){
    renderArea->getMin(mX, mY);
}
void MainWindow::getMax(double &mX, double &mY){
    renderArea->getMax(mX, mY);
}

MainWindow::~MainWindow()
{
    //...
}


