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
    //look.push_back(Cell(QPoint(-1, 0), QPoint(0, 0)));
    //addr = new std::list<std::pair<QPoint, QPoint>>[200][200];
}

QSize RenderArea::minimumSizeHint() const{
    return QSize(200, 200);
}

QSize RenderArea::sizeHint() const{
    return QSize(400, 400);
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

void RenderArea::chooseCZoom(size_t size){
    if (cZoom < CONST_ZOOM)
        cZoom = width()/(gMax - gMin).x();
}
void RenderArea::clearField(){
//    typedef std::list<std::pair<QPoint, QPoint>>(*ptr)[200][200];
//    addr = new ptr;
}

void RenderArea::plot(t_node &node, t_cont &cont, t_step &step){
    chooseCZoom(cont.size());
    for (int i = 0; i < 200; ++i)
        for (int j = 0; j < 200; ++j)
            addr[i][j].clear();
    look.clear();
    points.clear();
    double hc = 1.0 / step.size();
    for (uint32 l = 0; l < step.size(); l++){
       //uint32 c1 = circ.step()[0](l);// c2 = c1 + STEP[1](l);
        QColor rgb = QColor(255*l*hc, 255*(l*hc >= 0.5)*(l*hc - 0.5), 255*(l*hc >= 1)*(l*hc - 1));
        for (uint32 i1 = step[0](l); i1 < step[0](l) + step[1](l); i1++){
            points.push_back(Circuit(cont[1](i1)+cont(i1), rgb));
            int j = 1; bool z = cont(i1);
            points.back().dots[0] = QPoint(cZoom*node[0](cont[0](i1)), cZoom*node[1](cont[0](i1))) - QPoint(xMin, yMin);
            for(uint32 i = cont[0](i1)+1; i < cont[0](i1) + cont[1](i1); i++, j++){
                if (fabs(fabs(node[0](i) - node[0](i-1)) - (gMax - gMin).x()) < 1 ||
                        fabs(fabs(node[1](i) - node[1](i-1)) - (gMax - gMin).y()) < 1){
                    std::cout << j <<  " " <<cont[1](i1) << std::endl;
                    points.back().mSize(); z = false;
                    if (j == 1){
                        j = 0;
                    }else{
                        points.push_back(Circuit(cont[1](i1)+cont(i1) - j, rgb));
                        j = 0;
                    }
                    //continue;
                    //std::cout << node[0](i-1) << " " << node[0](i) << std::endl;
                }
                points.back().dots[j] = QPoint(cZoom*node[0](i) - xMin, cZoom*node[1](i) - yMin);
                if (j > 0)
                    addEdge(points.back().dots[j - 1], points.back().dots[j], i - 1, i, i1, l, rgb.value());
                }
            if (cont(i1)){
                if (z){
                points.back().dots[points.back().size() - 1] =  points.back().dots[0];
               addEdge(points.back().dots[0], points.back().dots[j], cont[0](i1), cont[0](i1) + cont[0](i1), i1, l, rgb.value());
                } else points.back().mSize();
            } //else addEdge(points.back().dots[cont[1](i1) - 1], points.back().dots[cont[1](i1) - 2],
              //      cont[0](i1) + cont[0](i1), cont[0](i1) + cont[0](i1) - 1, i1, l, rgb.value());
        }
   }
   panelChange =true;
   update();
}
/*            int j = 1;
            points.back().dots[0] = QPoint(cZoom*node[0](cont[0](i1)) - xMin, cZoom*node[1](cont[0](i1)) - yMin);
            for(uint32 i = cont[0](i1) + 1; i < cont[0](i1) + cont[1](i1); i++, j++){
                points.back().dots[j] = points.back().dots[j - 1] + QPoint(cZoom*(node[0](i-1) - node[0](i) -
                   (((node[0](i-1) - node[0](i)) > 0) ? 1 : (((node[0](i-1) - node[0](i)) < 0) ? -1 : 0)) * (gMax - gMin).x()) - xMin,
                        cZoom*(node[1](i-1) - node[1](i) -
                   (((node[1](i-1) - node[1](i)) > 0) ? 1 : (((node[1](i-1) - node[1](i)) < 0) ? -1 : 0)) * (gMax - gMin).y()) - yMin);
                    addEdge(points.back().dots[j - 1], points.back().dots[j], i - 1, i, i1, l, rgb.value());
            }*/
void RenderArea::addEdge(QPoint b, QPoint e, int _p1, int _p2, int c, int s, uint32_t col){
    //int result = 1;
    //b = (b + shift)/zoom; e = (e + shift) / zoom;
    int vert = (yMax) / 199, hor = (xMax) / 199;
    addr[b.y() / vert][b.x() / hor].push_back(Cell(b, b, _p1, _p2, c, s, col));
    if (b.y() < e.y())
        addr[b.y() / vert][b.x() / hor].push_back(Cell(b, e, _p1, _p2, c, s, col));
    else addr[e.y() / vert][e.x() / hor].push_back(Cell(e, b, _p2, _p1, c, s, col));
    QPoint move = b - e;
    if (b.y() < e.y())
        for (int i = e.y() / vert; i != b.y() / vert; i += (move.y() > 0) ? 1 : -1)
            for (int j = e.x() / hor; j != b.x() / hor; j += (move.x() > 0) ? 1 : -1)
                addr[i][j].push_back(Cell(b, e, _p1, _p2, c, s, col));
    else
        for (int i = e.y() / vert; i != b.y() / vert; i += (move.y() > 0) ? 1 : -1)
            for (int j = e.x() / hor; j != b.x() / hor; j += (move.x() > 0) ? 1 : -1)
                addr[i][j].push_back(Cell(e, b, _p2, _p1, c, s, col));
    //return result;//return 0;*/
}

void RenderArea::setMin(double mX, double mY){
    if (!antialiased){
        gMin = gMax = QPointF(mX, mY);
    } else {
        gMin = QPointF((gMin.x() > mX) ? mX : gMin.x(), (gMin.y() > mY ? mY : gMin.y()));
    }
    xMin = cZoom*mX;
    yMin = cZoom*mY;
}

void RenderArea::setMax(double mX, double mY){
    if (gMin == gMax){
        gMax = QPointF(mX, mY);
    }else
        gMax = QPointF((gMax.x() < mX) ? mX : gMax.x(), (gMax.y() < mY ? mY : gMax.y()));
    xMax = cZoom*mX - xMin;
    yMax = cZoom*mY - yMin;
    max = QPoint(xMax, yMax);
}

QPoint RenderArea::setShift(QPoint now, double last){
    return (now + shift)/last*zoom - now;
}

void RenderArea::paintEvent(QPaintEvent * /* event */){
    QPainter painter(this);         QPainter paintI(&panel);
    int x = 0, y = 0;
    QColor black = QColor(0, 0, 0);
    if (panelChange){
        paintI.setPen(pen);
        paintI.setBrush(brush);
        paintI.save();
        paintI.translate(x, y);
        paintI.restore();
    }
    if (antialiased)
           painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.save();
    painter.translate(x, y);
    painter.restore();
    if (points.size() > 2){
        drawing.clear();
        painter.setPen(QColor(100, 100, 100, 0));
        int r = 0, g = 0, b = 0;
        int i = 0;
        painter.setPen(QColor(100, 100, 100, 0));
        for (std::list<Circuit>::iterator it=points.begin(); it != points.end(); i++, ++it){
            drawing.push_back(std::make_pair(new QPoint[(*it).size()], (*it).size()));
            for (int i0 = 0; i0 < (*it).size(); i0++){
                drawing.back().first[i0] = (*it).dots[i0]*zoom - shift;
            }
            painter.setPen((*it).color());
            painter.drawPolyline(drawing.back().first, drawing.back().second);
        }
    }
    if (look.size() >= 1){
         for (std::list<Cell>::iterator it=look.begin(); it != look.end(); ++it){
        painter.setPen(255 - (*it).color().value());
        painter.setBrush(QColor(255, 0, 0, 50));
        painter.drawLine((*it).first()*zoom - shift, (*it).second()*zoom - shift);
        painter.drawEllipse((*it).first()*zoom - shift, 5, 5);
        painter.drawEllipse((*it).second()*zoom - shift, 5, 5);
         }
    }
    painter.setPen(black);
    painter.drawText(0, 10, QString::number(((0 + shift.x())/zoom + xMin)/cZoom) + QString(":") + QString::number(((10 + shift.y())/zoom+yMin)/cZoom));
    painter.drawText(width() - 120, 10, QString::number(((width() - 120 + shift.x())/zoom+xMin)/cZoom) + QString(":") + QString::number(((10 + shift.y())/zoom+yMin)/cZoom));
    painter.drawText(0, height(), QString::number((10 + shift.x())/zoom/cZoom) + QString(":") + QString::number((height() + shift.y())/zoom/cZoom));
    painter.drawText(width() - 120, height(), QString::number(((width() - 120 + shift.x())/zoom+xMin)/cZoom) + QString(":") + QString::number(((height() + shift.y())/zoom+yMin)/cZoom));
    painter.setPen(QColor(0, 255, 0, 255));
    painter.setBrush(QColor(255, 0, 0, 170));
    panelChange = false;
}


void RenderArea::mousePressEvent(QMouseEvent *event) {
    pressPoint = event->pos();
    search(event->pos());
}

void RenderArea::search(QPoint pos){
    //look.clear();
    int vert = (yMax) / 199, hor = (xMax) / 199;
    int i = (shift + pos).y() / vert / zoom, j = (shift + pos).x() / hor / zoom;
        for (std::list<Cell>::iterator it = addr[i][j].begin(); it != addr[i][j].end(); ++it){
            if ((pos - (*it).first() * zoom + shift).manhattanLength() < 3){
                    look.push_back((*it));
                    choosed = true;
                    emit chooseD((*it).n1(), (*it).nc());

                    update();
                    return;
                }
            if (abs((*it).first().x() - (*it).second().x()) < 2)
                    if (abs((*it).first().x() * zoom - shift.x() - pos.x()) < 4){
                int f = (*it).first().y() * zoom - shift.y(), s = (*it).second().y() * zoom - shift.y();
                if (abs(pos.y() - f) + abs(pos.y() - s) <= 2 + abs(f - s)){
                    look.push_back((*it));
                    choosed = true;
                    emit choose((*it).n1(), (*it).n2(), (*it).nc());

                    update();
                    return;
                }
            }

            if (abs((*it).first().y() - (*it).second().y()) < 2)
                    if (abs((*it).first().y() * zoom - shift.y() - pos.y()) < 4){
                int f = (*it).first().x() * zoom - shift.x(), s = (*it).second().x() * zoom - shift.x();
                if (abs(pos.x() - f) + abs(pos.x() - s) <= 2 + abs(f - s)){
                    look.push_back((*it));
                    choosed = true;
                    emit choose((*it).n1(), (*it).n2(), (*it).nc());

                    update();
                    return;
                }
            }
            if ((pos - (*it).first() * zoom + shift).manhattanLength() +
                     (pos - (*it).second() * zoom + shift).manhattanLength() <=
                     2 + (((*it).second() - (*it).first()) * zoom).manhattanLength()) {
                    //std::cout << (*it).n1() << std::endl;
                    QPoint f = (*it).first() * zoom - shift, s = (*it).second() * zoom - shift;
                    int y = (pos.x() * (f.y() - s.y()) + f.x() * s.y() - s.x() * f.y())/(f.x() - s.x());
                    if (abs(pos.y() - y) < 7){
                        look.push_back((*it));
                        choosed = true;
                        emit choose((*it).n1(), (*it).n2(), (*it).nc());

                        update();
                        return;
                    }
            }
        }
    choosed = false;
    look.clear();
    return;
}


void RenderArea::mouseMoveEvent(QMouseEvent *event) {
    shift += pressPoint - event->pos();
    pressPoint = event->pos();
    update();
}
void RenderArea::mouseReleaseEvent(QMouseEvent *event) {

}

void RenderArea::wheelEvent(QWheelEvent *event) {
    double lZ = zoom;
    if (event->delta() > 0)
        zoom *= 1.5;
    else
        if (zoom > 0.15)
            zoom /= 1.5;
    shift = setShift(event->pos(), lZ);
    update();
}
QImage  RenderArea::getImage(){
    return panel;
}
/*
QString RenderArea::nowChoose(){
    //QLabel(this->children().first())
    if (look.back().first().x() > 0 && choosed){
            QPointF t = QPointF(look.back().first().x() + xMin + shift.x(), look.back().first().y() + yMin + shift.y()) /zoom / cZoom;
            QPointF t1 = QPointF(look.back().second().x() + shift.x() + xMin, look.back().second().y() + shift.y() +  yMin) /zoom / cZoom;
            return QString("Choose edge: \n") + QString::number(t.x()) + QString(" : ")
                + QString::number(t.y()) + QString("  -  ") + QString::number(t1.x())
                + QString(" : ") + QString::number(t1.y()) + QString("\n1: № ") + QString::number(look.back().n1())
                    + QString(" \n2: № ")+ QString::number(look.back().n2()) + QString("\n№ cont ") + QString::number(look.back().nc())
                  ;
    }
    else return QString("Nothing");
}

QString RenderArea::nowChooseD(){
    //QLabel(this->children().first())
    if (look.back().first().x() > 0 && choosed){
            QPointF t = QPointF(look.back().first().x() + xMin + shift.x(), look.back().first().y() + yMin + shift.y()) /zoom / cZoom;
            return QString("Choose dot: \n") + QString::number(t.x()) + QString(" : ") + QString::number(t.y())+ QString("\n№ ") +
                    QString::number(look.back().n1())+ QString("\n№ cont ") + QString::number(look.back().nc());
    }
    else return QString("Nothing");
}*/
