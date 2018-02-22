
#include "renderarea.h"
#include "mainwindow.h"



const int IdRole = Qt::UserRole;

MainWindow::MainWindow()
{

    renderArea = new RenderArea();

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    setLayout(mainLayout);

    setWindowTitle(tr("Basic Drawing"));
}

MainWindow::~MainWindow()
{
    //...
}

void MainWindow::read(){
    std::ifstream fid ("/home/user/dU/UGraf/cont+03.dat", std::ios::binary);
    if (!fid){
        std::cout << "Файл не найден";
        return;
    }
    t_real min_x, min_y, max_x, max_y;
    uint32 num_t, num_p, num_c, num_s;
        fid.read((char *)&min_x, sizeof(double));
        fid.read((char *)&min_y, sizeof(double));
        fid.read((char *)&max_x, sizeof(double));
        fid.read((char *)&max_y, sizeof(double));
        fid.read((char *)&num_t, sizeof(uint32));
        fid.read((char *)&num_p, sizeof(uint32));
    double *NODEX = new double[num_p], *NODEY = new double[num_p];
    t_real temp;
    for(uint32 i = 0; i < num_p; i++){
        fid.read((char *)&temp, sizeof(double));
        NODEX[i] = temp;
    }
    for(uint32 i = 0; i < num_p; i++){
        fid.read((char *)&temp, sizeof(double));
        NODEY[i] = temp;
    }
    fid.read((char *)&num_c, sizeof(char)*4);
    uint32* CONTI = new uint32[num_c], * CONTN = new uint32[num_c];
    for(uint32 i = 0; i < num_c; i++){
        fid.read((char *)&CONTI[i], sizeof(char)*4);
    }
    for(uint32 i = 0; i < num_c; i++){
        fid.read((char *)&CONTN[i], sizeof(uint32));
    }
    uchar* CONTS = new uchar[num_c], c;
    for(uint32 i = 0; i < num_c; i++){
        fid.read((char *)&c, sizeof(char));
        CONTS[i] = c*1;
    }
    fid.read((char *)&num_s, sizeof(uint32));
    uint32* STEPI = new uint32[num_s], *STEPN = new uint32[num_s];
    for(uint32 i = 0; i < num_s; i++){
        fid.read((char *)&STEPI[i], sizeof(uint32));
    }
    for(uint32 i = 0; i < num_s; i++){
        fid.read((char *)&STEPN[i], sizeof(uint32));
    }
    //Выполняем отрисовку контуров (по уровням):
    //int RGB[3] = {0, 0, 0};
    //double dc[3] = {0, 0.5*1, 0.5*2}, hc = 1.0 / num_s;
    //std::cout << num_s << "-\n";
    renderArea->getMin(min_x, min_y);
    renderArea->getMax(max_x, max_y);
    for (uint32 l = 0; l < num_s; l++){
        uint32 c1 = STEPI[l] + 1, c2 = (c1 + STEPN[l]) - 1;
        for (uint32 i = c1 - 1; i < c2; i++){
            uint32 k1 = CONTI[i], k2 = (k1 + CONTN[i]) - 2;
            renderArea->plot(NODEX, NODEY, k1, k2, k2 - k1 + 1 + ((CONTS[i]*1) ? 1:0), CONTS[i]*1);
        }
    }
}

