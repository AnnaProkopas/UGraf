#include "renderarea.h"
#include <iostream>

#include <QPainter>

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent){
    antialiased = false;
    transformed = false;

    panel = QImage(QSize(width(),height()),QImage::Format_RGB32);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    sizeW = QSize(2000, 2000);

    LOOK[0] = QPoint(-1, 0);
}

QSize RenderArea::minimumSizeHint() const{
    return QSize(200, 200);
}

QSize RenderArea::sizeHint() const{
    return QSize(300, 300);
}


void RenderArea::setPen(const QPen &pen){
    this->pen = pen;
    update();
}

void RenderArea::setBrush(const QBrush &brush){
    this->brush = brush;
    update();
}

void RenderArea::setAntialiased(bool antialiased){
    this->antialiased = antialiased;
    update();
}

void RenderArea::setTransformed(bool transformed){
    this->transformed = transformed;
    update();
}

void RenderArea::plot(t_real* &nX, t_real* &nY, uint32_t k1, uint32_t k2, uint32_t _size, uint32 isCircle){
    points.push_back(std::make_pair(new QPoint[_size], _size));
    sizeL = points.size();
    int last = -1;
    for(int j = 0, i = k1; i <= k2; i++, j++){
        points.back().first[j] = QPoint(-1*Xmin + (int)(cZoom*nX[i]), -1*Ymin + (int)(cZoom*nY[i]));
        if (j > 0)
            last = addEdge(points.back().first[j - 1], points.back().first[j], last);
    }
    last = addEdge(points.back().first[k2 - k1 - 1], points.back().first[k2 - k1], last);
    if (isCircle){
        points.back().first[_size - 1] =  points.back().first[0];
        last = addEdge(points.back().first[_size - 2], points.back().first[_size - 1], last);
    }
    panel_change =true;
}

int RenderArea::addEdge(QPoint b, QPoint e, int last){
    int result = 1;
    b = (b + Shift)/Zoom; e = (e + Shift) / Zoom;
    int vert = (Ymin + Ymax) / 199, hor = (Xmin + Xmax) / 199;
    if (last > 0)
       addr[b.y() / vert][b.x() / hor].push_back(std::make_pair(b, b));
    addr[e.y() / vert][e.x() / hor].push_back(std::make_pair(e, e));
    if (b.y() < e.y())
        addr[b.y() / vert][b.x() / hor].push_back(std::make_pair(b, e));
    else addr[b.y() / vert][b.x() / hor].push_back(std::make_pair(e, b));
    QPoint move = b - e;
    if (b.y() < e.y())
        for (int i = e.y() / vert; i != b.y() / vert; i += (move.y() > 0) ? 1 : -1)
            for (int j = e.x() / hor; j != b.x() / hor; j += (move.x() > 0) ? 1 : -1)
                addr[b.y() / vert][b.x() / hor].push_back(std::make_pair(b, e));
    else
        for (int i = e.y() / vert; i != b.y() / vert; i += (move.y() > 0) ? 1 : -1)
            for (int j = e.x() / hor; j != b.x() / hor; j += (move.x() > 0) ? 1 : -1)
                addr[b.y() / vert][b.x() / hor].push_back(std::make_pair(e, b));
    return result;
}

void RenderArea::getMin(double mX, double mY){
    Xmin = (int)(cZoom*mX);
    Ymin = (int)(cZoom*mY);
}

void RenderArea::getMax(double mX, double mY){
    Xmax = (int)(cZoom*mX) - Xmin;
    Ymax = (int)(cZoom*mY) - Ymin;
    Xmax = (int)(cZoom*(mX - Xmin/70));
    Ymax = (int)(cZoom*(mY - Ymin/70));
    max = QPoint(Xmax, Ymax);
}

QPoint RenderArea::getShift(QPoint now, double last){
    return (now + Shift)/last*Zoom - now;
}

void RenderArea::paintEvent(QPaintEvent * /* event */){
    sizeW = QSize(width(), height());
    QPainter painter(this);
    QPainter paintI(&panel);
    painter.setPen(pen); paintI.setPen(pen);
    painter.setBrush(brush); paintI.setBrush(brush);
    int x = 0, y = 0;
    painter.save(); paintI.save();
    painter.translate(x, y); paintI.translate(x, y);
    painter.restore(); paintI.restore();
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
            painter.setPen(QColor(r, g, b, 150));
            painter.drawPolyline(drawing.back().first, drawing.back().second);
            painter.setPen(QColor(0, 0, 0, 70));
            //if (i == 1)
            //    painter.setPen(QColor(255, 0, 0, 255));
            painter.drawPoints(drawing.back().first, drawing.back().second);
            if (panel_change){
                paintI.setPen(QColor(r, g, b, 150));
                paintI.drawPolyline(drawing.back().first, drawing.back().second);
                paintI.setPen(QColor(0, 0, 0, 70));
                paintI.drawPoints(drawing.back().first, drawing.back().second);
            }
            /*for(int r1 = 0; r1 < 3; r1 ++){
                //RGB[r1] = 2 *( (l * hc >= dc) ? (l * hc - dc):0);
                //RGB[r1] = (RGB <= 1).* RGB + (RGB > 1);????????
            }/*/
        }
    }
    painter.setPen(QColor(0, 255, 0, 255));
    painter.setBrush(QColor(255, 0, 0, 170));
    if (LOOK[0].x() > 0){
        painter.drawEllipse(LOOK[0]*Zoom - Shift, 5, 5);
        if (LOOK[1].x() > 0){
            painter.drawEllipse(LOOK[1]*Zoom - Shift, 5, 5);
            painter.setPen(QColor(150, 150, 0, 255));
            painter.drawRect(QRect(LOOK[0]*Zoom - Shift, LOOK[1]*Zoom - Shift));
        }
    }
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    panel_change = false;
}


void RenderArea::mousePressEvent(QMouseEvent *event) {
    PressPont = event->pos();
    search(event->pos());
}

void RenderArea::search(QPoint pos){
//    pos = (Shift + pos) / Zoom;
    int vert = (Ymin + Ymax) / 199, hor = (Xmin + Xmax) / 199;
    int i = (Shift + pos).y() / vert / Zoom, j = (Shift + pos).x() / hor / Zoom;
    if (i > 200 || j > 200)
        std::cout << "Error, i: " << i << " j: " << j << "\n";
    else{
        std::cout << "!!! : " << i << " " << j << "\n";
        for (std::list<std::pair<QPoint, QPoint>>::iterator it=addr[i][j].begin(); it != addr[i][j].end(); ++it)
            if ((*it).first == (*it).second){
                if ((pos - (*it).first*Zoom + Shift).manhattanLength() < 3){
                    LOOK[0] = (*it).first;
                    LOOK[1].rx() = -1;
                    std::cout << "dot: " << pos.x() << " " << pos.y() << std::endl;
                    update();
                    /*QPainter painter(this);
                    painter.setPen(QColor(0, 255, 0, 255));
                    painter.setBrush(QColor(255, 0, 0, 170));
                    painter.drawEllipse((*it).first * Zoom - Shift, 5, 5);*/
                    return;
                }
            }
            else if (((*it).first.y() == (*it).second.y() &&
abs((*it).first.x()*Zoom - Shift.x() - pos.x()) +  abs((*it).second.x()*Zoom - Shift.x() - pos.x()) <=
                      5 + abs(((*it).first.x() - (*it).second.x())*Zoom))||
                     (pos.y() >= (*it).first.y()*Zoom - Shift.y() && pos.y() <= (*it).second.y()*Zoom - Shift.y())){
                int ty = -1;
                if ((*it).first.x() != (*it).second.x()){
                    ty = ((Shift.x() + pos.x())*((*it).first.y() - (*it).second.y()) / Zoom +(*it).first.x()*(*it).second.y() -
                      (*it).second.x()*(*it).first.y())/((*it).first.x() - (*it).second.x());
                    std::cout << "----: " << (*it).first.y() << " " << (*it).second.y() << std::endl;
                }
                std::cout << ty*Zoom - Shift.y() << " " << pos.y()<< std::endl;
                if ((abs(ty - (Shift.y() + pos.y()) / Zoom) < 5 || ty == -1) && ty >= (*it).first.y() && ty <= (*it).second.y()){
                    LOOK[0] = (*it).second;
                    LOOK[1] = (*it).first;
                    std::cout << "edge: " << pos.x() << " " << pos.y() << std::endl;
                    update();
                    /*QPainter painter(this);
                    painter.setPen(QColor(0, 255, 0, 255));
                    painter.setBrush(QColor(255, 0, 0, 170));
                    painter.drawEllipse((*it).first * Zoom - Shift, 5, 5);
                    painter.drawEllipse((*it).second * Zoom - Shift, 5, 5);
                    painter.setPen(QColor(150, 150, 0, 255));
                    painter.drawRect(QRect(LOOK[0], LOOK[1]));*/
                    return;
                }
            }
            //std::cout << (*it).first.x();
            //std::cout << " - " << (*it).first.y() << "\n";
    }
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
    Shift = getShift(event->pos(), lZ);
    update();
}
