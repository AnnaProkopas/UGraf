
#include "renderarea.h"
#include "mainwindow.h"



const int IdRole = Qt::UserRole;

MainWindow::MainWindow() {//: ui (new MainWindow){
    renderArea = new RenderArea();
    chooseEdge = new QPlainTextEdit();
    QWidget *wid = new QWidget(this);
    QGridLayout *mainBox = new QGridLayout;
    listEdge = new QListWidget;
    timeSlider = new QSlider(Qt::Horizontal);
    timeLabel = new QLabel("1");
    //timeSlider->setValue(100);
    connect(timeSlider, SIGNAL(valueChanged(int)), this, SLOT(value(int)));
    connect(renderArea, SIGNAL(choose(int, int, int)), this, SLOT(addE(int, int, int)));
    connect(renderArea, SIGNAL(chooseD(int, int)), this, SLOT(addD(int, int)));

    mainBox->addWidget(renderArea, 0, 0, 2, 1);
    mainBox->addWidget(listEdge, 0, 1, 1, 2);
    mainBox->addWidget(timeSlider, 1, 1, -1, -1);
    mainBox->addWidget(timeLabel, 1, 1, 1, -1, Qt::AlignRight);
    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(5);
    spLeft.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    renderArea->setSizePolicy(spLeft);
    QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spRight.setHorizontalStretch(1);
    spRight.setVerticalPolicy(QSizePolicy::MinimumExpanding);
    listEdge->setSizePolicy(spRight);

    setBaseSize(QSize(1200, 1200));
    setWindowTitle(tr("UGraf"));
    setCentralWidget(wid);
    wid->setLayout(mainBox);

    createActions();
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    file.open("/home/user/UGraf/pvort.dat", std::ios::binary);
    read();
}

MainWindow::~MainWindow()
{
    file.close();
    delete listEdge;
    delete renderArea;
    delete timeLabel;
    delete timeSlider;
    //delete ui;
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
      if (!file){
          QMessageBox messageBox;
          messageBox.critical(0,"Error","Файл не найден!");
          messageBox.setFixedSize(500,200);
          return;
      }
      if (field != NULL) delete field;
      field = new t_field(file);
      //fid.seekg();
      t_real min_x, min_y, max_x, max_y;
      uint32_t num_t, num_p, num_c, num_s;
      file.read((char *)&min_x, sizeof(double));
      file.read((char *)&min_y, sizeof(double));
      file.read((char *)&max_x, sizeof(double));
      file.read((char *)&max_y, sizeof(double));
      file.read((char *)&num_t, sizeof(uint32));

      field->setMax(max_x, max_y);
      field->setMin(min_x, min_y);

      renderArea->setMin(min_x, min_y);
      renderArea->setMax(max_x, max_y);

      field->setNumT(num_t);
      shiftInFile = new uint32[num_t + 1];
      shiftInFile[0] = file.tellg();
    for (uint32 N = 0; N < num_t; N++){
              file.read((char *)&num_p, sizeof(uint32));
              field->create_node(num_p);
              t_real temp;

              for(uint32 i = 0; i < num_p; i++){
                  file.read((char *)&temp, sizeof(double));
                  field->node()[0](i) = temp;
              }
              for(uint32 i = 0; i < num_p; i++){
                  file.read((char *)&temp, sizeof(double));
                  field->node()[1](i) = temp;
              }
              file.read((char *)&num_c, sizeof(uint32));
              field->create_cont(num_c);
              for(uint32 i = 0, t1; i < num_c; i++){
                  file.read((char *)&t1, sizeof(uint32));
                  field->cont()[0](i) = t1;
              }
              for(uint32 i = 0, t1; i < num_c; i++){
                  file.read((char *)&t1, sizeof(uint32));
                  field->cont()[1](i) = t1;
              }
              for(uint32 i = 0, c; i < num_c; i++){
                  file.read((char *)&c, sizeof(char));
                  field->cont()(i) = c;
              }
              file.read((char *)&num_s, sizeof(uint32));
              field->create_step(num_s);
              for(uint32 i = 0, t1; i < num_s; i++){
                  file.read((char *)&t1, sizeof(uint32));
                  field->step()[0](i) = t1;
              }
              for(uint32 i = 0, t1; i < num_s; i++){
                  file.read((char *)&t1, sizeof(uint32));
                  field->step()[1](i) = t1;
              }
              shiftInFile[N+1] = file.tellg();
    }
    timeLabel->setText("1");
    timeSlider->setValue(1);
    readTo(0);
              //renderArea->plot(field->node(), field->cont(), field->step());
      //}
}

void MainWindow::readTo(t_size t){
    if (t < 0 || t > field->getNumT()){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Wong time!");
        messageBox.setFixedSize(500,200);
        return;
    }
    if (t == field->getT())
        return;
    else field->setT(t);
    if (!file){
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Файл не найден!");
        messageBox.setFixedSize(500,200);
        return;
    }
    //unsigned long long seekNum =2*field->node().size()*sizeof(double) + field->cont().size()*(2*sizeof(uint32) + sizeof(char)) +
     //                            field->step().size()*2*sizeof(uint32);
   // seekNum += 4*sizeof(double) + 4*sizeof(uint32);
    //std::ifstream file = field->getFile();
    file.seekg(0, std::ios_base::beg);
    file.seekg(shiftInFile[t], std::ios_base::beg);
    //file.seekg(seekNum, std::ios_base::cur);
      t_size num_p, num_c, num_s;
              file.read((char *)&num_p, sizeof(uint32));
              //field->create_node(num_p);
              field->create_node(num_p);
              t_real temp;

              for(uint32 i = 0; i < num_p; i++){
                  file.read((char *)&temp, sizeof(double));
                  field->node()[0](i) = temp;
              }
              for(uint32 i = 0; i < num_p; i++){
                  file.read((char *)&temp, sizeof(double));
                  field->node()[1](i) = temp;
              }
              file.read((char *)&num_c, sizeof(uint32));
              field->create_cont(num_c);
              for(uint32 i = 0, t1; i < num_c; i++){
                  file.read((char *)&t1, sizeof(uint32));
                  field->cont()[0](i) = t1;
              }
              for(uint32 i = 0, t1; i < num_c; i++){
                  file.read((char *)&t1, sizeof(uint32));
                  field->cont()[1](i) = t1;
              }
              for(uint32 i = 0, c; i < num_c; i++){
                  file.read((char *)&c, sizeof(char));
                  field->cont()(i) = c;
              }
              file.read((char *)&num_s, sizeof(uint32));
              field->create_step(num_s);
              for(uint32 i = 0, t1; i < num_s; i++){
                  file.read((char *)&t1, sizeof(uint32));
                  field->step()[0](i) = t1;
              }
              for(uint32 i = 0, t1; i < num_s; i++){
                  file.read((char *)&t1, sizeof(uint32));
                  field->step()[1](i) = t1;
              }
              //shiftInFile[N] = file.tellg();
              renderArea->plot(field->node(), field->cont(), field->step());
      //}
}


void MainWindow::createActions(){
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
}

void MainWindow::open(){   
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Data file or Specific file"), "",
        tr("Data file (*.dat);;Data file (*.dat);;All Files (*)"));
    file.close();
    file.open(fileName.toStdString(), std::ios::binary);
      if (!file){
          QMessageBox messageBox;
          messageBox.critical(0,"Error","Файл не найден!");
          messageBox.setFixedSize(500,200);
          return;
      }
      listEdge->addItem(QString("Open file: ") + fileName);
      read();
}

void MainWindow::save(){
    QString fileName = QFileDialog::getSaveFileName(this,
          tr("Save Image"), "",
          tr("Image (*.jpg);;All Files (*)"));
    QImageWriter writer(fileName + QString(".jpg"));
    if(!writer.write(renderArea->getImage()))
    {
        qDebug() << writer.errorString();
    }
}

bool MainWindow::keypres(QKeyEvent *keyevent){
   /* if (keyevent->key()==Qt::Key_W) {
            qDebug() << "hi";
        }*/
}
//#endif
