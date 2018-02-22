/*#ifndef __INCLUDE_DATA_H
#define __INCLUDE_DATA_H

#include <algorithm>
#include <valarray>

typedef uint32_t uint32;
typedef unsigned char uchar;

typedef double t_real; typedef uint32_t t_size; typedef bool t_bool;

//Класс-обертка над массивом одномерных данных:
template <typename TYPE>
struct t_data: private std::valarray<TYPE> {

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
	inline explicit t_cont(t_size _num): t_step(_num), STAT(_num) {}

    inline const t_bool &operator() (int i) const {
        return STAT[i];
	}
	inline t_bool &operator() (int i) {
		return STAT[i];
	}
protected:
    t_data<t_bool> STAT;
};

//...

#endif //__INCLUDE_DATA_H*/
