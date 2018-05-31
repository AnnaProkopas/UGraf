
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
    struct cell{
        QPoint pair[2];
        //t_field &circuit;
        int p1, p2, cont, step;
        uint32_t rgb;
        //cell(QPoint b, QPoint e, /*t_field &f,*/ int _p1, int _p2, int c, int s, uint32_t col) : pair({b, e}),// circuit(f),
        //    p1(_p1), p2(_p2), cont(c), step(s), rgb(col) { }
        cell(QPoint b, QPoint e, int _p1, int _p2, int c, int s, uint32_t col) : pair({b, e}),
            p1(_p1), p2(_p2), cont(c), step(s), rgb(col) {}
        QPoint first() { return pair[0]; }
        QPoint second() { return pair[1]; }
    };
    std::list<cell> addr[200][200];
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
    int addEdge(QPoint b, QPoint e, int _p1, int _p2, int c, int s, uint32_t col);
    void chooseCZoom(size_t size);
    void clearField();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    /*void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;*/
};

#endif // RENDERAREA_H
