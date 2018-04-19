
#include "renderarea.h"
#include "mainwindow.h"



const int IdRole = Qt::UserRole;

MainWindow::MainWindow()
    //: textEdit(new QPlainTextEdit)
{
    //'RGB', QPoint(width(), height()), colorname);
    renderArea = new RenderArea();

    setCentralWidget(renderArea);

    //QGridLayout *mainLayout = new QGridLayout;
    //mainLayout->addWidget(renderArea, 0, 0, 1, 4);
    //setLayout(mainLayout);
    createActions();
    fileMenu = menuBar()->addMenu(tr("&File"));
   // fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    setWindowTitle(tr("UGraf"));

/*    infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                                  "invoke a context menu</i>"));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);*/
}

MainWindow::~MainWindow()
{
    //...
}

#ifndef QT_NO_CONTEXTMENU
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    /*QMenu menu(this);
    menu.addAction(cutAct);
    menu.addAction(copyAct);
    menu.addAction(pasteAct);
    menu.exec(event->globalPos());*/
}
#endif // QT_NO_CONTEXTMENU

void MainWindow::read(){
    std::ifstream fid ("/home/user/UGraf/cont+02.dat", std::ios::binary);
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
    bool* CONTS = new bool[num_c], c;
    for(uint32 i = 0; i < num_c; i++){
        fid.read((char *)&c, sizeof(char));
        if (c*1)
            CONTS[i] = true;
        else CONTS[i] = false;
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
            renderArea->plot(NODEX, NODEY, k1, k2, k2 - k1 + 1 + (CONTS[i] ? 1:0), CONTS[i]);
        }
    }
}

void MainWindow::createActions(){
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
}

void MainWindow::open(){   
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Data file or Specific file"), "",
        tr("Data file (*.bin);;Specific file (*.sp);;All Files (*)"));
}

void MainWindow::save(){
    QString fileName = QFileDialog::getSaveFileName(this,
          tr("Save Address Book"), "",
          tr("Address Book (*.abk);;All Files (*)"));
}

bool MainWindow::keypres(QKeyEvent *keyevent){
   /* if (keyevent->key()==Qt::Key_W) {
            qDebug() << "hi";
        }*/
}
//#endif
