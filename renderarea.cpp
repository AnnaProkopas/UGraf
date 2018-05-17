#include "renderarea.h"
#include <iostream>

#include <QPainter>
#define CONST_ZOOM 350

RenderArea::RenderArea(QWidget *parent)
    : QWidget(parent){
    antialiased = false;
    transformed = false;

    panel = QImage(QSize(width(),height()),QImage::Format_RGB32);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    sizeW = QSize(2000, 2000);
    cZoom = CONST_ZOOM;
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

/*void RenderArea::plot(t_real* &nX, t_real* &nY, uint32_t k1, uint32_t k2, uint32_t _size, uint32 isCircle){
    /*points.push_back(std::make_pair(new QPoint[_size], _size));
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
}*/

void RenderArea::chooseCZoom(size_t size){
    if (cZoom < CONST_ZOOM)
        cZoom = width()/(gMax - gMin).x()*size;
}

void RenderArea::plot(t_node &node, t_cont &cont, t_step &step, t_size num_s){
    chooseCZoom(node.size());
   for (uint32 l = 0; l < step.size(); l++){
       //uint32 c1 = circ.step()[0](l);// c2 = c1 + STEP[1](l);
       for (uint32 i1 = step[0](l); i1 < step[0](l) + step[1](l); i1++){
           points.push_back(std::make_pair(new QPoint[cont[1](i1)+cont(i1)], cont[1](i1)+cont(i1)));
           bool first = true; int j = 0;
           for(uint32 i = cont[0](i1); i < cont[0](i1) + cont[1](i1); i++, j++){
               points.back().first[j] = QPoint(cZoom*node[0](i) - Xmin, cZoom*node[1](i) - Ymin);
               if (j > 0)
                   addEdge(points.back().first[j - 1], points.back().first[j], false);
           }
           //addEdge(points.back().first[j - 1], points.back().first[j], false);
           if (cont(i1)){
               points.back().first[cont[1](i1)] =  points.back().first[0];
               addEdge(points.back().first[0], points.back().first[cont[1](i1) - 1], false);
           }
       }
   }//
   panel_change =true;
}

int RenderArea::addEdge(QPoint b, QPoint e, int last){
    int result = 1;
    b = (b + Shift)/Zoom; e = (e + Shift) / Zoom;
    int vert = (Ymin + Ymax) / 199, hor = (Xmin + Xmax) / 199;
    /*if (last > 0)
       addr[b.y() / vert][b.x() / hor].push_back(std::make_pair(b, b));
    addr[e.y() / vert][e.x() / hor].push_back(std::make_pair(e, e));*/
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
    if (!antialiased){
        gMin = gMax = QPointF(mX, mY);
    } else {
        gMin = QPointF((gMin.x() > mX) ? mX : gMin.x(), (gMin.y() > mY ? mY : gMin.y()));
    }
    Xmin = (int)(cZoom*mX);
    Ymin = (int)(cZoom*mY);
}

void RenderArea::getMax(double mX, double mY){
    if (gMin == gMax){
        gMax = QPointF(mX, mY);
    }else
        gMax = QPointF((gMax.x() < mX) ? mX : gMax.x(), (gMax.y() < mY ? mY : gMax.y()));
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
    QPainter painter(this);         QPainter paintI(&panel);
    int x = 0, y = 0;
    if (panel_change){
        paintI.setPen(pen);
        paintI.setBrush(brush);
        paintI.save();
        paintI.translate(x, y);
        paintI.restore();
    }
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.save();
    painter.translate(x, y);
    painter.restore();
    if (points.size() > 2){
        //int i = 0, r = 0, g = 0, b = 0;
        drawing.clear();
        painter.setPen(QColor(100, 100, 100, 0));
//pos: 0 => wigth(); 0 => height()
        int r = 0, g = 0, b = 0;
        int vert = (Ymin + Ymax)*Zoom / 199, hor = (Xmin + Xmax)*Zoom / 199;
        int i0 = (Shift.y()) / vert - 1, i1 = (Shift.y() + height()) / vert + 1,
                j0 = (Shift.x()) / hor - 1, j1 = (Shift.x() + width()) / hor + 1;
        /*std::cout << i0 << "  " << i1 << "\n";
        std::cout << j0 << "  " << j1 << "\n";
        std::cout << " - - - - - - - - - - \n";*/
        for (int i = (i0 < 0) ? 0 : i0; i <= i1 && i < 200; i++)
            for (int j = (j0 < 0) ? 0 : j0; j <= j1 && j < 200; j++){
                /*if (i < 45) b += 5;
                else if (i < 90) g += 5;
                    else {
                        if (i == 90) g = 0;
                        if (i < 135) r += 5;
                        else {
                            if (i = 135) { r = 0; b = 0; }
                            if (i < 180) r += 5;
                            else if (i < 225) g += 5; }
                    }*/
                for (std::list<std::pair<QPoint, QPoint>>::iterator it=addr[i][j].begin(); it != addr[i][j].end(); ++it){
                //drawing.back().first[i0] = (*it).first*Zoom - Shift;
                    painter.setPen(QColor(r % 225, g % 225, b % 225, 150));
                    painter.drawLine((*it).first*Zoom - Shift, (*it).second*Zoom - Shift);
                    painter.setPen(QColor(0, 0, 0, 70));
                    painter.drawPoint((*it).first*Zoom - Shift);
                    painter.drawPoint((*it).second*Zoom - Shift);
        }
      }
        /*if (panel_change){
            int i = 0, r = 0, g = 0, b = 0;
            for (std::list<std::pair<QPoint *, int>>::iterator it=points.begin(); it != points.end(); i++, ++it){
            drawing.push_back(std::make_pair(new QPoint[(*it).second], (*it).second));
            for (int i0 = 0; i0 < (*it).second; i0++){
                drawing.back().first[i0] = (*it).first[i0]*Zoom - Shift;
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
                paintI.setPen(QColor(r % 225, g % 225, b % 225, 150));
                paintI.drawPolyline(drawing.back().first, drawing.back().second);
                paintI.setPen(QColor(0, 0, 0, 70));
                paintI.drawPoints(drawing.back().first, drawing.back().second);
            }
        }
    }*/
}
    painter.setPen(QColor(0, 255, 0, 255));
    painter.setBrush(QColor(255, 0, 0, 170));
    if (LOOK[0].x() > 0 && choosed){
        painter.drawEllipse(LOOK[0]*Zoom - Shift, 5, 5);
        if (LOOK[1].x() > 0){
            painter.drawEllipse(LOOK[1]*Zoom - Shift, 5, 5);
            painter.setPen(QColor(150, 150, 0, 255));
            painter.drawRect(QRect(LOOK[0]*Zoom - Shift, LOOK[1]*Zoom - Shift));
        }
    }
    painter.setPen(QColor(150, 150, 150, 225));
    painter.setBrush(QColor(225, 225, 225, 225));
    painter.drawRect(QRect(QPoint(0, height() - 15), QPoint(width(), height())));
    painter.drawText(2, height() - 2, QString(nowChoose().c_str()));
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
    int vert = (Ymin + Ymax) / 199, hor = (Xmin + Xmax) / 199;
    int i = (Shift + pos).y() / vert / Zoom, j = (Shift + pos).x() / hor / Zoom;
        for (std::list<std::pair<QPoint, QPoint>>::iterator it=addr[i][j].begin(); it != addr[i][j].end(); ++it)
            //if ((*it).first == (*it).second){
                if ((pos - (*it).first*Zoom + Shift).manhattanLength() < 3){
                    LOOK[0] = (*it).first;
                    LOOK[1].rx() = -1;
                    choosed = true;
                    update();
                    return;
                }
            //}
            else if (abs((*it).first.x() - (*it).second.x()) < 2 && abs((*it).first.x()*Zoom - Shift.x() - pos.x()) < 4){
                int f = (*it).first.y()*Zoom - Shift.y(), s = (*it).second.y()*Zoom - Shift.y();
                if (abs(pos.y() - f) + abs(pos.y() - s) <= 2 + abs(f - s)){
                    LOOK[0] = (*it).second;
                    LOOK[1] = (*it).first;
                    choosed = true;
                    //std::cout << "edge: " << pos.x() << " " << pos.y() << std::endl;
                    update();
                    return;
                }
            }
            else if (abs((*it).first.y() - (*it).second.y()) < 2 && abs((*it).first.y()*Zoom - Shift.y() - pos.y()) < 4){
                int f = (*it).first.x()*Zoom - Shift.x(), s = (*it).second.x()*Zoom - Shift.x();
                if (abs(pos.x() - f) + abs(pos.x() - s) <= 2 + abs(f - s)){
                    LOOK[0] = (*it).second;
                    LOOK[1] = (*it).first;
                    choosed = true;
                    //std::cout << "edge: " << pos.x() << " " << pos.y() << std::endl;
                    update();
                    return;
                }
            }
            else if ((pos - (*it).first*Zoom + Shift).manhattanLength() +
                     (pos - (*it).second*Zoom + Shift).manhattanLength() <=
                     2 + (((*it).second - (*it).first)*Zoom).manhattanLength()) {
                    QPoint f = (*it).first*Zoom - Shift, s = (*it).second*Zoom - Shift;
                    int y = (pos.x() * (f.y() - s.y()) + f.x() * s.y() - s.x() * f.y())/(f.x() - s.x());
                    if (abs(pos.y() - y) < 7){
                        LOOK[0] = (*it).second;
                        LOOK[1] = (*it).first;
                        choosed = true;
                        //std::cout << "edge: " << pos.x() << " " << pos.y() << std::endl;
                        update();
                        return;
                    }
            }
    choosed = false;
    return;
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
        if (Zoom > 0.2)
            Zoom /= 1.5;
    Shift = getShift(event->pos(), lZ);
    update();
}

std::string RenderArea::nowChoose(){
    //QLabel(this->children().first())
    if (LOOK[0].x() > 0 && choosed){
        if (LOOK[1].x() > 0) {
            QPointF t = QPointF(LOOK[0].x() + Xmin, LOOK[0].y() + Ymin) / cZoom;
            QPointF t1 = QPointF(LOOK[1].x() + Xmin, LOOK[1].y() + Ymin) / cZoom;
            return std::string("Choose edge: ") + std::to_string(t.x()) + std::string(" : ")
                + std::to_string(t.y()) + std::string("  -  ") + std::to_string(t1.x())
                + std::string(" : ") + std::to_string(t1.y());
        } else {
            QPointF t = QPointF(LOOK[0].x() + Xmin, LOOK[0].y() + Ymin) / cZoom;
            return std::string("Choose dot: ") + std::to_string(t.x()) + std::string(" : ") + std::to_string(t.y());
        }
    }
    else return std::string("Nothing");
}
