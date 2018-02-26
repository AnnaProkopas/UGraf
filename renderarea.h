
#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QWheelEvent>

#include "data.h"

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
    void plot(t_real* &nX, t_real* &nY, uint32_t k1, uint32_t k2, uint32_t _size, uint32 isCircle);
    void getMin(double mX, double mY);
    void getMax(double mX, double mY);
    QPoint getShift(QPoint now, double last);
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    static const uint32 cZoom = 170;
    int Xmin, Ymin, Xmax, Ymax;
    double Zoom = 1;
    QPoint Shift = QPoint(0, 0);
    QPoint max;
    QPen pen;
    QBrush brush;
    std::list<std::pair<QPoint *, int>>points;
    uint32_t sizeL = 0;
    QSize sizeW;
    bool antialiased;
    bool transformed;
    QPoint* resetSize(QPoint * & p, uint32 s);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    /*void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;*/
};

#endif // RENDERAREA_H
