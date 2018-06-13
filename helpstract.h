#ifndef HELPSTRACT
#define HELPSTRACT

#include "data.h"
#include <QPoint>
#include <QColor>

struct Circuit {\
private:
    QColor colorC;
    int sizeC;
public:
    QPoint *dots;
    Circuit(t_size s, QColor c) : sizeC(s), colorC(c) { dots = new QPoint[sizeC]; }
    QColor color() { return colorC; }
    t_size size() { return sizeC; }
};

struct Cell{
private:
    QPoint pair[2];
    //t_field &circuit;
    size_t p1, p2, cont, step;
    QColor rgb;
    //cell(QPoint b, QPoint e, /*t_field &f,*/ int _p1, int _p2, int c, int s, uint32_t col) : pair({b, e}),// circuit(f),
    //    p1(_p1), p2(_p2), cont(c), step(s), rgb(col) { }
public:
    Cell(QPoint b, QPoint e, size_t _p1, size_t _p2, size_t c, size_t s, QColor col) : pair({b, e}),
        p1(_p1), p2(_p2), cont(c), step(s), rgb(col) {}
    Cell(QPoint b, QPoint e) : pair({b, e}),
        p1(0), p2(0), cont(0), step(0), rgb(QColor()) {}
    QPoint first() { return pair[0]; }
    QPoint second() { return pair[1]; }
    QColor color() { return rgb; }
    t_size n1() { return p1; }
    t_size n2() { return p2; }
    t_size nc() { return cont; }
    t_size ns() { return step; }
};

#endif // HELPSTRACT

