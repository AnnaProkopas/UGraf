#include "renderarea.h"
#include <iostream>

#include <QPainter>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    antialiased = false;
    transformed = false;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    sizeW = QSize(width(), height());
}

QSize RenderArea::minimumSizeHint() const
{
    return QSize(200, 200);
}

QSize RenderArea::sizeHint() const
{
    return QSize(300, 300);
}


void RenderArea::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}

void RenderArea::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}

void RenderArea::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

void RenderArea::setTransformed(bool transformed)
{
    this->transformed = transformed;
    update();
}

void RenderArea::plot(t_real* &nX, t_real* &nY, uint32_t k1, uint32_t k2, uint32_t _size, uint32 isCircle){
    points.push_back(std::make_pair(new QPoint[_size + 1], _size));
    sizeL = points.size();
    for(int j = 0, i = k1; i <= k2 && j < _size; i++, j++){
        points.back().first[j] = QPoint(-1*Xmin + (int)(cZoom*nX[i]), -1*Ymin + (int)(cZoom*nY[i]));
    }
    if (isCircle)
        points.back().first[_size - 1] = QPoint(Xmin, Ymin) +  QPoint((int)(cZoom*nX[k1]), (int)(cZoom*nY[k1]));
    //update();
}


void RenderArea::getMin(double mX, double mY){
    Xmin = (int)(cZoom*mX);
    Ymin = (int)(cZoom*mY);
}

void RenderArea::getMax(double mX, double mY){
    Xmax = (int)(cZoom*mX) - Xmin;
    Ymax = (int)(cZoom*mY) - Ymin;
    /*if (Xmax > Ymax)
        cZoom = (int)(70*width()/Xmax);
    else cZoom = (int)(70*height()/Ymax);*/
    Xmax = (int)(cZoom*(mX - Xmin/70));
    Ymax = (int)(cZoom*(mY - Ymin/70));
    max = QPoint(Xmax, Ymax);
}

QPoint* RenderArea::resetSize(QPoint * & p, uint32 s){
    /*int tZ = zoom;
    if (Xmax > Ymax)
        zoom = (int)(70*width()/Xmax);
    else zoom = (int)(70*height()/Ymax);
    Xmax = (int)(zoom*Xmax/tZ);
    Ymax = (int)(zoom*Ymax/tZ);
    //static bool isFirst = true;
    QPoint * NowPoint = new QPoint[s];
    for(int i = 0; i < s; i++)
        NowPoint[i] = max/2 + zoom*p[i]/cZoom;*/
    return new QPoint[s];
}

QPoint RenderArea::getShift(QPoint now, double last){
    return (now + Shift)/last*Zoom - now;
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    sizeW = QSize(width(), height());
    QPainter painter(this);
    painter.setPen(pen);
    painter.setBrush(brush);
    if (antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);

    int x= 0, y = 0;
            painter.save();
            painter.translate(x, y);
            if (transformed) {
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.9);
                painter.translate(-50, -50);
            }
    painter.restore();
    //resetSize();
    if (sizeL > 2){
        int i = 0;
        std::list<QPoint*> temp;
        for (std::list<std::pair<QPoint *, int>>::iterator it=points.begin(); it != points.end(); i++, ++it){
            temp.push_back(new QPoint[(*it).second]);
            for (int i0 = 0; i0 < (*it).second; i0++){
                temp.back()[i0] = (*it).first[i0]*Zoom - Shift;
            }
            painter.setPen(QColor(4*i % 255, 0, 0, 70));
            painter.drawPolyline(temp.back(), (*it).second - 1);
            painter.setPen(QColor(0, 0, 4*i % 255, 200));
            painter.drawPoints(temp.back(), (*it).second - 1);
            /*for(int r1 = 0; r1 < 3; r1 ++){
                //RGB[r1] = 2 *( (l * hc >= dc) ? (l * hc - dc):0);
                //RGB[r1] = (RGB <= 1).* RGB + (RGB > 1);????????
            }/*/
        }
    }
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}


void RenderArea::mousePressEvent(QMouseEvent *event) {

}
void RenderArea::mouseMoveEvent(QMouseEvent *event) {
    //if ((event->buttons() & Qt::LeftButton) && scribbling)
    //    drawLineTo(event->pos());
}
void RenderArea::mouseReleaseEvent(QMouseEvent *event) {

}

void RenderArea::wheelEvent(QWheelEvent *event) {
    double lZ = Zoom;
    if (event->delta() > 0)
        Zoom *= 1.5;
    else
        Zoom /= 1.5;
    std::cout << event->globalX() << "  " << event->globalY() << "\n";
    std::cout << width() << "  " << height() << "\n";
    Shift = getShift(event->pos(), lZ);
    update();
}
