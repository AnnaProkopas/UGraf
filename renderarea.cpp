#include "renderarea.h"
#include <iostream>

#include <QPainter>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent)
{
    antialiased = false;
    transformed = false;

    panel = QImage(QSize(width(),height()),QImage::Format_RGB32);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    sizeW = QSize(2000, 2000);
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
    points.push_back(std::make_pair(new QPoint[_size], _size));
    sizeL = points.size();
    for(int j = 0, i = k1; i <= k2; i++, j++){
        points.back().first[j] = QPoint(-1*Xmin + (int)(cZoom*nX[i]), -1*Ymin + (int)(cZoom*nY[i]));
    }
    if (isCircle){
        points.back().first[_size - 1] =  points.back().first[0];
        //points.back().second++;
    }
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

QPoint RenderArea::getShift(QPoint now, double last){
    return (now + Shift)/last*Zoom - now;
}

void RenderArea::paintEvent(QPaintEvent * /* event */)
{
    sizeW = QSize(width(), height());
    QPainter paintI(&panel);
    QPainter painter(this);
    painter.setPen(pen); paintI.setPen(pen);
    painter.setBrush(brush); paintI.setBrush(brush);
    /*if (antialiased){
        painter.setRenderHint(QPainter::Antialiasing, true);
        paintI.setRenderHint(QPainter::Antialiasing, true);
    }*/
    int x = 0, y = 0;
    painter.save(); paintI.save();
    painter.translate(x, y); paintI.translate(x, y);
            /*if (transformed) {
                painter.translate(50, 50);
                painter.rotate(60.0);
                painter.scale(0.6, 0.9);
                painter.translate(-50, -50);
            }*/
    painter.restore(); paintI.restore();
    //resetSize();
    if (sizeL > 2){
        int i = 0, r = 0, g = 0, b = 0;
        drawing.clear();
        painter.setPen(QColor(100, 100, 100, 0));
        for (std::list<std::pair<QPoint *, int>>::iterator it=points.begin(); it != points.end(); i++, ++it){
            drawing.push_back(std::make_pair(new QPoint[(*it).second], (*it).second));
            for (int i0 = 0; i0 < (*it).second; i0++){
                drawing.back().first[i0] = (*it).first[i0]*Zoom - Shift;
                if (Zoom > 1.7)
                    painter.drawEllipse((*it).first[i0]*Zoom - Shift, 1, 1);
            }
            if (i < 45) b += 5;
            else if (i < 90) g += 5;
                else {
                    if (i == 90) g = 0;
                    if (i < 135) r += 5;
                    else {
                        if (i = 135) { r = 0; b = 0; }
                        if (i < 180) r += 5;
                        else if (i < 225) g += 5;
                    }
                }
            //std::cout << r << " " << g  << " " << b << "\n";
            painter.setPen(QColor(r, g, b, 100));
            painter.drawPolyline(drawing.back().first, drawing.back().second);
            painter.setPen(QColor(0, 0, 0, 70));
            painter.drawPoints(drawing.back().first, drawing.back().second);
            if (Shift == QPoint(0,0)){
                paintI.setPen(QColor(r, g, b, 70));
                paintI.drawPolyline((*it).first, (*it).second);
                paintI.setPen(QColor(0, 0, 0, 70));
                paintI.drawPoints((*it).first, (*it).second);
            }
            /*for(int r1 = 0; r1 < 3; r1 ++){
                //RGB[r1] = 2 *( (l * hc >= dc) ? (l * hc - dc):0);
                //RGB[r1] = (RGB <= 1).* RGB + (RGB > 1);????????
            }/*/
        }
        //std::cout << i << "\n";
    }
    /*if (sizeL > 2){
        int i = 0;
        drawing.clear();
        for (std::list<std::pair<QPoint *, int>>::iterator it=points.begin(); it != points.end(); i++, ++it){
            drawing.push_back(std::make_tuple(new QPoint[(*it).second], (*it).second, i,
                              (*it).first[0]*Zoom - Shift, (*it).first[0]*Zoom - Shift));
            for (int i0 = 0; i0 < (*it).second; i0++){
                std::get<0>(drawing.back())[i0] = (*it).first[i0]*Zoom - Shift;
                if (std::get<3>(drawing.back()).x() > std::get<0>(drawing.back())[i0].x())
                    std::get<3>(drawing.back()).setX(std::get<0>(drawing.back())[i0].x());
                else if (std::get<4>(drawing.back()).x() < std::get<0>(drawing.back())[i0].x())
                    std::get<4>(drawing.back()).setX(std::get<0>(drawing.back())[i0].x());
                if (std::get<3>(drawing.back()).y() > std::get<0>(drawing.back())[i0].y())
                    std::get<3>(drawing.back()).setY(std::get<0>(drawing.back())[i0].y());
                else if (std::get<4>(drawing.back()).y() < std::get<0>(drawing.back())[i0].y())
                    std::get<4>(drawing.back()).setY(std::get<0>(drawing.back())[i0].y());
            }
            painter.setPen(QColor(4*i % 255, 0, 0, 70));
            painter.drawPolyline(std::get<0>(drawing.back()), std::get<1>(drawing.back()));
            painter.setPen(QColor(0, 0, 4*i % 255, 200));
            painter.drawPoints(std::get<0>(drawing.back()), std::get<1>(drawing.back()));
            /*for(int r1 = 0; r1 < 3; r1 ++){
                //RGB[r1] = 2 *( (l * hc >= dc) ? (l * hc - dc):0);
                //RGB[r1] = (RGB <= 1).* RGB + (RGB > 1);????????
            }/
        }
    }*/
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}


void RenderArea::mousePressEvent(QMouseEvent *event) {
    PressPont = event->pos();
    QRgb readback = panel.pixel((event->pos()+Shift)/Zoom);
    std::cout << readback << "\n";
    //setPen(QColor().fromRgb(readback));
    /*std::thread thr(RenderArea::search, this, event->pos());
    thr.detach();//*/
    search(event->pos());
}

void RenderArea::search(QPoint pos){
    int j = 0;
    for (std::vector<std::pair<QPoint *, int>>::iterator it = drawing.begin(); it != drawing.end(); j++, it++){
            for (int i = 0; i < (*it).second; i++)
                if (pos.x() < ((*it).first[i].x() + 5) && pos.x() > ((*it).first[i].x() - 5)){
                    std::cout << j + 1 << ": " << i + 1 << "\n";
                    return;
                }
        }
    /*a = (y2 - y1) / (x2 - x1);
    b = y1 - x1 * a;*/
   /* for (std::list<std::tuple<QPoint *, int, int, QPoint, QPoint>>::iterator it = drawing.begin(); it != drawing.end(); it++){
        for (int i = 0; i < std::get<1>(*it); i++)
            if (pos.x() < (std::get<0>(*it)[i].x() + 5) && pos.x() > (std::get<0>(*it)[i].x() - 5)){
                std::cout << i << "\n";
                return;
            }
    }*/
}

void RenderArea::mouseMoveEvent(QMouseEvent *event) {
    Shift += PressPont - event->pos();
    PressPont = event->pos();
    update();
}
void RenderArea::mouseReleaseEvent(QMouseEvent *event) {

}

void RenderArea::wheelEvent(QWheelEvent *event) {
    double lZ = Zoom;
    if (event->delta() > 0)
        Zoom *= 1.5;
    else
        Zoom /= 1.5;
    /*std::cout << event->globalX() << "  " << event->globalY() << "\n";
    std::cout << width() << "  " << height() << "\n";*/
    Shift = getShift(event->pos(), lZ);
    update();
}
