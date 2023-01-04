#include <qwindowdefs_win.h>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QGraphicsItem"
#include "QGraphicsRectItem"
#include "QGraphicsScene"
#include "QGraphicsView"
#include "QPen"
#include "QObject"
#include "input.h"
#include "QProcess"
#include "QWheelEvent"
#include "QToolButton"
#include "QFileDialog"
#include "QSlider"
#include "QMessageBox"
#include "QWindow"
#include "nodeeditdialog.h"
#include "elementeditdialog.h"
#include "QSystemTrayIcon"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{   
    auto start = std::chrono::system_clock::now();
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    strainScene = new QGraphicsScene(this);

    CreateSystemTrayIcon(); //create the tray icon

    setWindowTitle("PLANE ANALYSIS V1.0");
    //set icons
    this->setWindowIcon(QIcon(":/icon/icon/main.jpeg"));
//    ui->selectBtn->setIcon(QIcon("E:\\codes\\vscode\\cpp\\computerMechanics\\icon\\select.png"));
//    ui->fitBtn->setIcon(QIcon("E:\\codes\\vscode\\cpp\\computerMechanics\\icon\\home.png"));
//    ui->zoomBtn->setIcon(QIcon("E:\\codes\\vscode\\cpp\\computerMechanics\\icon\\search.png"));
//    ui->btn_rotate_left->setIcon(QIcon("E:\\codes\\vscode\\cpp\\computerMechanics\\icon\\rotateL.png"));
//    ui->btn_rotate_right->setIcon(QIcon("E:\\codes\\vscode\\cpp\\computerMechanics\\icon\\rotateR.png"));
//    ui->editDialogBtn->setIcon(QIcon("E:\\codes\\vscode\\cpp\\computerMechanics\\icon\\edit.png"));

    ui->selectBtn->setChecked(true);
    ui->selectBtn->setCheckable(true);
    ui->zoomBtn->setChecked(true);
    ui->zoomBtn->setCheckable(true);
    ui->showoutBtn->setCheckable(true);
    ui->showoutBtn->setChecked(true);
    ui->showDisBtn->setCheckable(true);
    ui->showDisBtn->setChecked(false);


    ui->graphicsView->setScene(scene);
    ui->strainView->setScene(strainScene);

    // design the slider items
    ui->lineWidthSlider->setRange(0,100);
    ui->nodeSizeSlider->setRange(0,100);
    ui->viewSlider->setRange(0,2);
    ui->magnifySilder->setRange(1,600);

    // set the name of toolbox's pages
    ui->toolBox->setItemText(0,"OUTPUT");
    ui->toolBox->setItemText(1,"VIEW");
    ui->toolBox->setItemText(2,"GRAPH");

    // set a thread to continue refresh the output
    threadOutPut = new ThreadOutPut();
    connect(threadOutPut,SIGNAL(finished()),threadOutPut,SLOT(deleteLater()));
    threadOutPut->start();

    QObject::connect(ui->btn_rotate_left, &QPushButton::clicked, this, &MainWindow::rotateLeft);
    QObject::connect(ui->btn_rotate_right, &QPushButton::clicked, this, &MainWindow::rotateRight);
    QObject::connect(ui->btn_result,&QPushButton::clicked,this,&MainWindow::result);
    connect(ui->selectBtn,&QToolButton::toggled,this,&MainWindow::toggleMode);
    connect(ui->actionload,&QAction::triggered,this,&MainWindow::loadFile);
    connect(ui->lineWidthSlider,&QSlider::valueChanged,this,&MainWindow::setLineWidth);
    connect(ui->nodeSizeSlider,&QSlider::valueChanged,this,&MainWindow::setNodeSize);
    connect(ui->magnifySilder,&QSlider::valueChanged,this,&MainWindow::setMagnify);
    connect(ui->configureBtn,&QPushButton::clicked,this,&MainWindow::paint);
    connect(ui->resetBtn,&QPushButton::clicked,this,&MainWindow::reset);
    connect(ui->viewSlider,&QSlider::valueChanged,this,&MainWindow::setViewNo);
    connect(ui->designBtn,&QPushButton::clicked,this,&MainWindow::designView);
    connect(threadOutPut,&ThreadOutPut::textRead,this,&MainWindow::textRefresh);
    connect(ui->showoutBtn,&QToolButton::toggled,this,&MainWindow::showOutput);
    connect(ui->editDialogBtn,&QPushButton::clicked,this,&MainWindow::edit);
    connect(ui->actionsave,&QAction::triggered,this,&MainWindow::save);
    connect(ui->showDisBtn,&QToolButton::toggled,this,&MainWindow::showDisplace);
    connect(ui->actiontray,&QAction::triggered,this,[=](){
        this->hide();
    });
    connect(ui->fitBtn,&QPushButton::clicked,this,&MainWindow::fitView);
    connect(ui->elementSelect,&QRadioButton::clicked,this,&MainWindow::switchSelectMode);
    connect(ui->nodeSelect,&QRadioButton::clicked,this,&MainWindow::switchSelectMode);
    connect(ui->displacementSlider,&QSlider::valueChanged,this,&MainWindow::setMagDis);
    connect(ui->chooseBtn,&QToolButton::clicked,this,&MainWindow::chooseCmd);
    connect(ui->executeBtn,&QPushButton::clicked,this,&MainWindow::executeCmd);


    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);

    send("everything is ok!\nyou can load the .inp to simulate the model\nor you can design the view by clicking the design button!");
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
}

void MainWindow::CreateSystemTrayIcon()
{
    //初始化两个项目
    QAction* showAction = new QAction(QStringLiteral("显示"));//项1
    QAction* exitAction = new QAction(QStringLiteral("退出"));//项2
    //项1的点击槽函数
    connect(showAction, &QAction::triggered, this, [=]()
    {
        this->show();
    });
    //项2的点击槽函数
    connect(exitAction , &QAction::triggered, this, [=]()
    {
        QApplication::exit(0);
    });
 
    //初始化菜单并添加项
    QMenu* trayMenu = new QMenu(this);//菜单
    trayMenu->addAction(showAction);
    trayMenu->addAction(exitAction );
 
    //创建一个系统托盘
    
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon("E:\\codes\\vscode\\cpp\\computerMechanics\\icon\\main.jpeg"));    //设置托盘图标
    trayIcon->setContextMenu(trayMenu);                                     //设置菜单
    trayIcon->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Do you want to quit?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
        case QMessageBox::Yes:
            event->accept();
            break;
        case QMessageBox::No:
            hide();
            event->ignore();
            break;
    }
}

void MainWindow::switchSelectMode(){
    cout<<"switch select mode"<<endl;
    if(ui->elementSelect->isChecked()){
        selectMode = 1;
        send("select mode: element\n");
        for(int i=0;i<ElementItems.size();i++){
            ElementItems[i]->setFlag(QGraphicsItem::ItemIsSelectable,true);
        }
        for(int i=0;i<NodeItems.size();i++){
            NodeItems[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
        }
    }

    if(ui->nodeSelect->isChecked()){
        selectMode = 0;
        send("select mode: node\n");
        for(int i=0;i<ElementItems.size();i++){
            ElementItems[i]->setFlag(QGraphicsItem::ItemIsSelectable,false);
        }
        for(int i=0;i<NodeItems.size();i++){
            NodeItems[i]->setFlag(QGraphicsItem::ItemIsSelectable,true);
        }
    }
}


void MainWindow::loadFile(){
    auto start = std::chrono::system_clock::now();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("INP Files (*.inp)"));
    cout<<"1"<<endl;
    if (fileName.isEmpty())
        return;
    totalNode = readFile(fileName.toStdString());
    cout<<"2"<<endl;
    auto start1 = std::chrono::system_clock::now();
    totalNode->geneK();
    auto end1 = std::chrono::system_clock::now();
    std::cout<<"load file geneK time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end1-start1).count()<<"ms\n";
    totalNode->geneDelta();
    auto end2 = std::chrono::system_clock::now();
    std::cout<<"load file geneDelta time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end2-end1).count()<<"ms\n";
    paint();
    auto end = std::chrono::system_clock::now();
    std::cout<<"load file paint time: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end-end2).count()<<"ms\n";
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "load file and paint time: " << elapsed_seconds.count() << "s\n";
}
void MainWindow::send(QString message) {
    QFile file("output.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << message<<"\n";
    file.close();
}

void MainWindow::chooseCmd(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("TXT Files (*.txt)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    ui->cmdText->setText(fileName);
    cmd.clear();
    QString line;
    while(!in.atEnd()){
        line = in.readLine();
        if(line.contains("#")){
            line = line.remove("#");
            line = line.trimmed();
            cmd.push_back(line.toStdString());
        }
    }
    file.close();
    send("cmd loaded!");
}

void MainWindow::executeCmd(){
    QString out;
    for(string s:cmd){
        QProcess process;
        process.start(s.c_str());
        process.waitForFinished();
        process.waitForReadyRead();
        QByteArray qba  = process.readAll();

        //解决中文乱码问题
        QTextCodec* pTextCodec = QTextCodec::codecForName("System");
        assert(pTextCodec != nullptr);
        QString str = pTextCodec->toUnicode(qba);
        out += str+"\n";
    }
    send("execute cmds successfully!"+out);
}

void MainWindow::save() {

    QPixmap pix=ui->graphicsView->grab();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("PNG Files (*.png)"));
    pix.save(fileName);
    send("the view has been saved to"+fileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::zoomIn()
{
    ui->graphicsView->scale(1.2,1.2);
}
void MainWindow::zoomOut()
{
    ui->graphicsView->scale(0.8,0.8);
}

void MainWindow::rotateLeft()
{
    ui->graphicsView->rotate(-5);
}

void MainWindow::rotateRight()
{
    ui->graphicsView->rotate(5);
}

void MainWindow::showOutput() {
    if(this->ui->showoutBtn->isChecked()) {
//        this->threadOutPut->start();
        threadOutPut->threadContinue();
    } else {
//        this->threadOutPut->quit();
        threadOutPut->threadStop();
    }
}

void MainWindow::result() {
    send("now calculating...\n");
    totalNode->geneF(); //add gravity force
//    std::cout<<"execute result function!"<<std::endl;
    send("calculating the sigma...\n");
    scene->clear();
    totalNode->geneSigma();
//    cout<<"calculate over!"<<endl;
    send("calculate over!");
    paint();

}

void MainWindow:: fitView() {

    send("fit the view to the model");
    if(scene == nullptr)
        throw "scene is null";
    scene->destroyed();
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    paint();
    ui->graphicsView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
}

void MainWindow::edit() {
    if(selectMode==0){
        vector<Node*> eNodes;
        // cout<<"nodes size:"<<NodeItems.size()<<endl;
        for(int i=0;i<NodeItems.size();i++){
            // cout<<"node "<<i<<endl;
            if(NodeItems[i]->isSelected()) {
                // cout<<"selected "<<i<<endl;
                eNodes.push_back(NodeItems[i]->node);
                // cout<<"push back "<<i<<endl;
            }
        }
        // cout<<"edit nodes size:"<<eNodes.size()<<endl;
        NodeEditDialog *editDialog = new NodeEditDialog(eNodes);
        editDialog->exec();
    }else{
        vector<Element*> eElements;
        // cout<<"elements size:"<<ElementItems.size()<<endl;
        for(int i=0;i<ElementItems.size();i++){
            // cout<<"element "<<i<<endl;
            if(ElementItems[i]->isSelected()) {
                // cout<<"selected "<<i<<endl;
                eElements.push_back(ElementItems[i]->element_);
                // cout<<"push back "<<i<<endl;
            }
        }
        // cout<<"edit elements size:"<<eElements.size()<<endl;
        ElementEditDialog *editDialog = new ElementEditDialog(eElements,this);
        editDialog->exec();
    }


}


void MainWindow::reset() {
    for(int i=0;i<totalNode->nodes.size();i++) {
        totalNode->nodes[i]->u = 0;
        totalNode->nodes[i]->v = 0;
        totalNode->nodes[i]->fx = 0;
        totalNode->nodes[i]->fy = 0;
        totalNode->nodes[i]->unknown = true;
    }
    for(int i=0;i<totalNode->elements.size();i++) {
        totalNode->elements[i]->sigma[0] = 0;
        totalNode->elements[i]->sigma[1] = 0;
        totalNode->elements[i]->sigma[2] = 0;
    }
    totalNode->Delta.setZero();
    totalNode->F.setZero();
    totalNode->geneDelta();
    paint();
}

void MainWindow::paint()
{
    auto start = std::chrono::system_clock::now();
    try{
        if(totalNode==NULL)
            return;
        scene->clear(); //clear all the picture
        NodeItems.clear();
        ElementItems.clear();
//        int n = 10,begin = 0,span = totalNode->elements.size() / n,end = begin+span;
//        vector<MyThread*> *threads = new vector<MyThread*>();
//        for(int i=0;i<n;i++){
//            MyThread *thread = new MyThread();
//            thread->begin = begin;
//            thread->end = end;
//            thread->totalNode = totalNode;
//            thread->ElementItems = &ElementItems;
//            thread->NodeItems = &NodeItems;
//            thread->lineWidth = lineWidth;
//            thread->nodeSize = nodeSize;
//            thread->magnify = magnify;
//            thread->scene = scene;
//            threads->push_back(thread);
//            begin += span;
//            end += span;
//        }
//        threads->at(n-1)->end = totalNode->elements.size();
//        for(int i=0;i<n;i++) threads->at(i)->run();
        //绘制单元和节点并染色
        for(int i = 0; i < totalNode->elements.size(); i++)
        {
            for(int j=0;j<3;j++){
                NodeItem *node;
                if(totalNode->isMove)
                     node = new NodeItem(QPointF((totalNode->elements[i]->nodes[j]->x+totalNode->magDis*totalNode->elements[i]->nodes[j]->u)*magnify,
                                                      (totalNode->elements[i]->nodes[j]->y+totalNode->magDis*totalNode->elements[i]->nodes[j]->v)*magnify),totalNode->elements[i]->nodes[j],
                                              nodeSize);
                else
                    node = new NodeItem(QPointF(totalNode->elements[i]->nodes[j]->x*magnify,
                                                          totalNode->elements[i]->nodes[j]->y*magnify),totalNode->elements[i]->nodes[j],
                                                  nodeSize);

                NodeItems.push_back(node);
                scene->addItem(node);
            }
            ElementItem *element;
            if(totalNode->isMove)
                element = new ElementItem(QPointF((totalNode->elements[i]->nodes[0]->x+totalNode->magDis*totalNode->elements[i]->nodes[0]->u)*magnify,
                                                           (totalNode->elements[i]->nodes[0]->y+totalNode->magDis*totalNode->elements[i]->nodes[0]->v)*magnify),
                                                   QPointF((totalNode->elements[i]->nodes[1]->x+totalNode->magDis*totalNode->elements[i]->nodes[1]->u)*magnify,
                                                           (totalNode->elements[i]->nodes[1]->y+totalNode->magDis*totalNode->elements[i]->nodes[1]->v)*magnify),
                                                   QPointF((totalNode->elements[i]->nodes[2]->x+totalNode->magDis*totalNode->elements[i]->nodes[2]->u)*magnify,
                                                           (totalNode->elements[i]->nodes[2]->y+totalNode->magDis*totalNode->elements[i]->nodes[2]->v)*magnify),
                                                   totalNode,
                                                   totalNode->elements[i],
                                                   lineWidth);
            else
                element = new ElementItem(QPointF(totalNode->elements[i]->nodes[0]->x*magnify,
                                                               totalNode->elements[i]->nodes[0]->y*magnify),
                                                       QPointF(totalNode->elements[i]->nodes[1]->x*magnify,
                                                               totalNode->elements[i]->nodes[1]->y*magnify),
                                                       QPointF(totalNode->elements[i]->nodes[2]->x*magnify,
                                                               totalNode->elements[i]->nodes[2]->y*magnify),
                                                       totalNode,
                                                       totalNode->elements[i],
                                                       lineWidth);
            int hue = totalNode->geneHue(totalNode->elements[i]->sigma[totalNode->viewNo]);
            element->hue = hue;
            ElementItems.push_back(element);
            scene->addItem(element);
        }
        switchSelectMode();
        updateStrainGraph();

    }
    catch (std::exception &e){
        QMessageBox msgBox;
        msgBox.setText(QString(e.what()));
        msgBox.exec();
//        std::cout<<e.what()<<std::endl;
        send(e.what());
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    send("paint time:"+QString::number(elapsed_seconds.count()));

}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(!this->ui->zoomBtn->isChecked()) //如果没有选中放大按钮，则不响应
        return;
    // this->ui->graphicsView->centerOn(event->pos());
    if(event->angleDelta().y()>0)
        zoomIn();
    else
        zoomOut();
    
    // std::cout<<"wheel event:"<<event->delta()<<std::endl;
}

void MainWindow::toggleMode()
{
    if(this->ui->selectBtn->isChecked())
        this->ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    else
        this->ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    this->ui->graphicsView->setInteractive(this->ui->selectBtn->isChecked());
}

void MainWindow::setLineWidth(int value)
{
    lineWidth = value;
}

void MainWindow::setNodeSize(int value)
{
    nodeSize = value;
}

void MainWindow::setMagnify(int magnify) {
    this->magnify = magnify;
}

void MainWindow::showDisplace() {
    if(ui->showDisBtn->isChecked()) {
        totalNode->isMove= true;
    }
    else {
        totalNode->isMove = false;
    }
}

void MainWindow::designView()
{
    //设计视图
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        QString cmd = "gmsh.exe";
    #else
        QString cmd = "gmsh";
    #endif
    QProcess *process = new QProcess(this);
    process->start(cmd);





    
    
}

void MainWindow::textRefresh(QString str) {
    this->ui->textBrowser->setText(str);
}

void MainWindow::simulateView()
{

    //模拟视图
    this->ui->graphicsView->show(); //显示主界面
}

void MainWindow:: setViewNo(int no)
{
    //设置视图
    this->totalNode->viewNo = no;
    totalNode->updateSigmaUS();
    paint();
}

void MainWindow::setMagDis(int dis)
{
    //设置放大倍数
    totalNode->magDis = dis;
    paint();
}
void MainWindow::updateStrainGraph() {
    strainScene->clear();
    double width = this->ui->strainView->width()*0.8;
    double height = this->ui->strainView->height()*0.8;
    double x = 0,y = 0;

    double mean = totalNode->sigmaMean;
    double var = totalNode->sigmaVar;
    double max = totalNode->maxSigma;
    double min = totalNode->minSigma;

    int n = 8;
    vector<double> hslList,strainList;
    hslList.resize(n);
    strainList.resize(n);
    strainList = {min,mean-3*var,mean-2*var,mean-var,mean,mean+var,mean+2*var,max};
    for(int i=0;i<n;i++) {
        hslList[i] = totalNode->geneHue(strainList[i]);
    }
    for(int i=0;i<n;i++){
        QGraphicsTextItem *text = new QGraphicsTextItem(QString::number(strainList[n-1-i], 'f', 2));
        text->setPos(x,y);
        text->setZValue(999);
        strainScene->addItem(text);
        strainScene->addRect(x,y,width,height/n,QPen(Qt::black),QBrush(QColor::fromHsl(hslList[n-1-i],255,125)));
        y += height/n;

    }

}

MyThread::MyThread(QObject *parent) : QThread(parent)
{
}

MyThread::~MyThread()
{
}

void MyThread::run(){
    //绘制单元和节点并染色
//    cout<<"begin:"<<begin<<" end:"<<end<<endl;
    for(int i = begin; i < end; i++)
    {
        for(int j=0;j<3;j++){
            NodeItem *node;
            if(totalNode->isMove)
                 node = new NodeItem(QPointF((totalNode->elements[i]->nodes[j]->x+totalNode->magDis*totalNode->elements[i]->nodes[j]->u)*magnify,
                                                  (totalNode->elements[i]->nodes[j]->y+totalNode->magDis*totalNode->elements[i]->nodes[j]->v)*magnify),totalNode->elements[i]->nodes[j],
                                          nodeSize);
            else
                node = new NodeItem(QPointF(totalNode->elements[i]->nodes[j]->x*magnify,
                                                      totalNode->elements[i]->nodes[j]->y*magnify),totalNode->elements[i]->nodes[j],
                                              nodeSize);

            NodeItems->push_back(node);
            scene->addItem(node);
        }
        ElementItem *element;
        if(totalNode->isMove)
            element = new ElementItem(QPointF((totalNode->elements[i]->nodes[0]->x+totalNode->magDis*totalNode->elements[i]->nodes[0]->u)*magnify,
                                                       (totalNode->elements[i]->nodes[0]->y+totalNode->magDis*totalNode->elements[i]->nodes[0]->v)*magnify),
                                               QPointF((totalNode->elements[i]->nodes[1]->x+totalNode->magDis*totalNode->elements[i]->nodes[1]->u)*magnify,
                                                       (totalNode->elements[i]->nodes[1]->y+totalNode->magDis*totalNode->elements[i]->nodes[1]->v)*magnify),
                                               QPointF((totalNode->elements[i]->nodes[2]->x+totalNode->magDis*totalNode->elements[i]->nodes[2]->u)*magnify,
                                                       (totalNode->elements[i]->nodes[2]->y+totalNode->magDis*totalNode->elements[i]->nodes[2]->v)*magnify),
                                               totalNode,
                                               totalNode->elements[i],
                                               lineWidth);
        else
            element = new ElementItem(QPointF(totalNode->elements[i]->nodes[0]->x*magnify,
                                                           totalNode->elements[i]->nodes[0]->y*magnify),
                                                   QPointF(totalNode->elements[i]->nodes[1]->x*magnify,
                                                           totalNode->elements[i]->nodes[1]->y*magnify),
                                                   QPointF(totalNode->elements[i]->nodes[2]->x*magnify,
                                                           totalNode->elements[i]->nodes[2]->y*magnify),
                                                   totalNode,
                                                   totalNode->elements[i],
                                                   lineWidth);
        int hue = totalNode->geneHue(totalNode->elements[i]->sigma[totalNode->viewNo]);
        element->hue = hue;
        ElementItems->push_back(element);
        scene->addItem(element);
    }

}


ThreadOutPut::ThreadOutPut()
{

}

ThreadOutPut::~ThreadOutPut()
{
}

void ThreadOutPut::run() {
    // continue to print the text from 'output.txt' to the textbrowser
    while(isRunning){
        mlock.lock();
        QFile file("output.txt");
        file.open(QIODevice::ReadOnly);
        QString text = "";
        while(!file.atEnd()) {
            text += file.readLine();
        }
        file.close();
        emit textRead(text);
        this->sleep(1); // sleep 1 second
        mlock.unlock();
    }
}

void ThreadOutPut::threadStop() {
    mlock.lock();
}

void ThreadOutPut::threadContinue() {
    mlock.unlock();
}
