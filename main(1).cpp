#include "mainwindow.h"
#include <QApplication>
#include "data.h"
#include <iostream>
#include <string> // подключаем строки
#include <fstream> // подключаем файлы


int main(int argc, char *argv[])
{
     FILE *fid = fopen("cont.dat", "r");
     //fid = fopen(fname, 'rb');

     //Считываем заголовок:
     double min_x;
     //fid.in((double*)&min_x,1); //>> min_x, min_y, max_x, max_y, num_t;
     std::fread(&min_x, sizeof(double), 1, fid);
     /*min_x = fread(fid, 1, 'double');
    QApplication a(argc, argv);
    MainWindow w;
    w.show();*/
    std::fclose(fid);

    return 0;//a.exec();
}
