#ifndef __INCLUDE_DATA_H
#define __INCLUDE_DATA_H

#include <algorithm>
#include <valarray>
#include <QPoint>
#include <iostream>

typedef uint32_t uint32;
typedef unsigned char uchar;

typedef double t_real; typedef uint32_t t_size; typedef bool t_bool;

//Класс-обертка над массивом одномерных данных:
template <typename TYPE>
struct t_data: private std::valarray<TYPE> {
    inline explicit t_data(TYPE* vals, t_size _num): std::valarray<TYPE>(vals, _num) {}
    inline explicit t_data(t_size _num): std::valarray<TYPE>(_num) {}

    inline const TYPE &operator() (t_size i) const {
        return (*this)[i];
    }
    inline TYPE &operator() (t_size i) {
        return (*this)[i];
    }
    using std::valarray<TYPE>::size;
};

//Структура массива узлов (точек контуров):
//Все узлы, принадлежащие одному контуру, хранятся в виде последовательного блока (подмассива);
//DATA[0](i) -- x_{i};//NODEX
//DATA[1](i) -- y_{i};//NODEY
struct t_node {
    inline explicit t_node(t_real* valX, t_real* valY, t_size _num):
        num(_num), DATA{t_data<t_real>(valX, _num), t_data<t_real>(valY, _num)} {}
    inline explicit t_node(t_size _num):
        num(_num), DATA{t_data<t_real>(_num), t_data<t_real>(_num)} {}
    inline const t_data<t_real> &operator[] (int i) const {
        return DATA[i];
    }
    inline t_data<t_real> &operator[] (int i) {
        return DATA[i];
    }
    inline t_size size() const {
        return num;
    }
protected:
    t_data<t_real> DATA[2];
    t_size num;
};

//Структура уровней:
//DATA[0](i) -- индекс начального контура на i-м уровне;//STEPI
//DATA[1](i) -- число контуров на i-м уровне;//STEPN
struct t_step {
    inline explicit t_step(t_size* stepI, t_size* stepN, t_size _num):
        num(_num), DATA{t_data<t_size>(stepI, _num), t_data<t_size>(stepN, _num)} {}

    inline explicit t_step(t_size _num):
        num(_num), DATA{t_data<t_size>(_num), t_data<t_size>(_num)} {}

    inline const t_data<t_size> &operator[] (int i) const {
        return DATA[i];
    }
    inline t_data<t_size> &operator[] (int i) {
        return DATA[i];
    }
    inline t_size size() const {
        return num;
    }
protected:
    t_data<t_size> DATA[2];
    t_size num;
};

//Структура контуров:
//Все контуры, принадлежащие одному уровню, хранятся в виде последовательного блока (подмассива);
//DATA[0](i) -- индекс начального узла i-го контура;//CONTI
//DATA[1](i) -- число узлов на i-м контуре;//CONTN
//DATA(i) -- флаг замкнутого контура;//CONTS
struct t_cont:
public t_step {
    inline explicit t_cont(t_size* contI, t_size* contN, t_bool* contS, t_size _num):
        t_step(contI, contN, _num), STAT(std::valarray<t_bool>(contS, _num)) {}
    inline explicit t_cont(t_size _num): t_step(_num),
        STAT(std::valarray<t_bool>(_num)) {}
    inline const t_bool &operator() (int i) const {
        return STAT[i];
    }
    inline t_bool &operator() (int i) {
        return STAT[i];
    }
protected:
    std::valarray<t_bool> STAT;
};

struct t_field {
private:
    std::ifstream &file;
    t_node NODE;
    t_cont CONT;
    t_step STEP;
    bool fullN = false, fullS = false, fullC = false;
    t_real min_x, min_y, max_x, max_y;
    t_size num_t = -1, now_t = -1;
public:
    inline t_field(std::ifstream &f): file(f), NODE(t_node(0)), CONT(t_cont(0)), STEP(t_step(0))  {}
    /*inline explicit t_field(t_size _noden, t_size _stepn, t_size _contn):
        NODE(t_node(_noden)), STEP(t_step(_stepn)), CONT(t_cont(_contn)) {}
    inline t_field(): NODE(t_node(0)), CONT(t_cont(0)), STEP(t_step(0)) {}*/
    inline void setNumT(t_size n) {
        if (num_t == -1 && n >= 0)
            num_t = n; }
    inline void create_node(t_real *valX, t_real *valY, t_size _num) {
        NODE = t_node (valX, valY, _num);
        fullN = true;
    }
    inline void create_step(t_size* stepI, t_size* stepN, t_size _num){
        STEP = t_step(stepI, stepN, _num);
        fullS = true;
    }
    inline void create_cont(t_size* contI, t_size* contN, t_bool* contS, t_size _num){
        CONT = t_cont(contI, contN, contS, _num);
        fullC = true;
    }
    inline void create_node(t_size _num) { if (!fullN) NODE = t_node (_num);  }
    inline void create_cont(t_size _num) { if (!fullC) CONT = t_cont (_num);  }
    inline void create_step(t_size _num) { if (!fullS) STEP = t_step (_num);  }
    inline const t_node& node() const{ return NODE; }
    inline const t_step& step() const{ return STEP; }
    inline const t_cont& cont() const{ return CONT; }
    inline t_node& node() { return NODE; }
    inline t_step& step() { return STEP; }
    inline t_cont& cont() { return CONT; }
    inline void clear() { fullN = fullS = fullC = false; }
    inline void setMin(t_real x, t_real y) {min_x = x; min_y = y;}
    inline t_real minX() { return min_x; }
    inline t_real minY() { return min_y; }
    inline QPointF min() { return QPointF(min_x, min_y); }
    inline void setMax(t_real x, t_real y) {max_x = x; max_y = y;}
    inline t_real maxX() { return max_x; }
    inline t_real maxY() { return max_y; }
    inline QPointF max() { return QPointF(max_x, max_y); }
    inline t_size getT() { return now_t; }
    inline t_size getNumT() { return num_t; }
    inline void setT(t_size newT) { if (0 < newT && newT < num_t) now_t = newT; }
    //inline const std::ifstream& getFile() const{ return file; }
};
#endif //__INCLUDE_DATA_H*/
