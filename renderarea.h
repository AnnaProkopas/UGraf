
#ifndef RENDERAREA_H
#define RENDERAREA_H

#include "data.h"

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

public slots:

    void setPen(const QPen &pen);
    void setBrush(const QBrush &brush);
    void setAntialiased(bool antialiased);
    void setTransformed(bool transformed);
    //void plot(t_real* &nX, t_real* &nY, uint32_t k1, uint32_t k2, uint32_t _size, uint32 isCircle);
    void plot(t_node &node, t_cont &cont, t_step &step, t_size num_s);

    void getMin(double mX, double mY);
    void getMax(double mX, double mY);
    QPoint getShift(QPoint now, double last);
    std::string nowChoose();
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    /*const*/ size_t cZoom = 350;//добавить алгоритм вычисления - в зависимости от max-min
    int Xmin, Ymin, Xmax, Ymax;
    QPointF gMin, gMax;
    double Zoom = 1;
    QPoint Shift = QPoint(0, 0), PressPont;
    QPoint max;
    QPen pen;
    QBrush brush;
    std::list<std::pair<QPoint *, int>> points;
    std::vector<std::pair<QPoint *, int>> drawing;
    QImage panel;
    std::list<std::pair<QPoint, QPoint>> addr[200][200];
    //bool: dot(true) or edge (false)
    uint32_t sizeL = 0;
    QSize sizeW;
    QPoint LOOK[2];
    bool antialiased;
    bool transformed;
    bool panel_change = true;
    bool choosed = false;
    /*static */void search(QPoint pos);
    QPoint* resetSize(QPoint * & p, uint32 s);
    int addEdge(QPoint b, QPoint e, int last);
    void chooseCZoom(size_t size);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    /*void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;*/
};

#endif // RENDERAREA_H
