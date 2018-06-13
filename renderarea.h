
#ifndef RENDERAREA_H
#define RENDERAREA_H

#include "data.h"
#include "helpstract.h"
#include <cmath>

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QWheelEvent>
#include <thread>
#include <QImage>
#include <QPoint>
#include <QLabel>

typedef uint32_t uint32;
typedef unsigned char uchar;

typedef double t_real; typedef uint32_t t_size; typedef bool t_bool;

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    RenderArea(QWidget *parent = 0);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    QString nowChoose();
    QString nowChooseD();

public slots:

    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setTransformed(bool transformed);
    //void plot(t_real* &nX, t_real* &nY, uint32_t k1, uint32_t k2, uint32_t _size, uint32 isCircle);
    void plot(t_node &node, t_cont &cont, t_step &step);

    void setMin(double mX, double mY);
    void setMax(double mX, double mY);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    /*const*/ size_t cZoom = 350;//добавить алгоритм вычисления - в зависимости от max-min
    int xMin, yMin, xMax, yMax;
    QPointF gMin, gMax;
    double zoom = 1;
    QPoint shift = QPoint(0, 0), pressPoint, max;
    QPen pen;
    QBrush brush;
    std::list<Circuit> points;
    std::vector<std::pair<QPoint *, int>> drawing;
    QImage panel;
    std::list<Cell> addr[200][200];
    //bool: dot(true) or edge (false)
    uint32_t sizeL = 0;
    QSize sizeW;
    std::list<Cell> look;
    bool antialiased, transformed, panelChange = true, choosed = false;

    /*static */void search(QPoint pos);
    QPoint* resetSize(QPoint * & p, uint32 s);
    void addEdge(QPoint b, QPoint e, int _p1, int _p2, int c, int s, uint32_t col);
    void chooseCZoom(size_t size);
    void clearField();
    QPoint setShift(QPoint now, double last);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    /*void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;*/
public:
    signals:
    void choose();
    void chooseD();
};

#endif // RENDERAREA_H
