#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "QPainter"
#include "input.h"
#include <QPushButton>
#include "elementItem.h"
#include "nodeItem.h"
#include "TotalNode.h"
#include "QThread"
#include "QTextBrowser"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ThreadOutPut : public QThread { //continue to refresh the output
Q_OBJECT
public:
    bool isRunning = true;
    QMutex mlock;
    ThreadOutPut();
    ~ThreadOutPut();
    void run();
    void threadStop();
    void threadContinue();
signals:
    void textRead(QString);
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ThreadOutPut *threadOutPut;
    QGraphicsScene *scene,*strainScene;
    TotalNode* totalNode = NULL;
    int nodeSize = 2;
    int lineWidth = 1;
    int magnify = 10;
    int selectMode = 0; //0:select node, 1:select element
    vector<string> cmd;
    vector<ElementItem*> ElementItems;
    vector<NodeItem*> NodeItems;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paint();
    void wheelEvent(QWheelEvent *event);
    void send(QString);
    void edit();// set the boundary restrict
    void save();// save the photo
    void showDisplace();// show the displacement
    void fitView();// fit the view
    void chooseCmd();// choose the command
    void executeCmd();// execute the command
    void CreateSystemTrayIcon();// create the tray icon
    void closeEvent(QCloseEvent *event);// close the window



private:
    Ui::MainWindow *ui;
    
    
private slots:
    void zoomIn();
    void zoomOut();
    void rotateLeft();
    void rotateRight();
    void toggleMode(); //更改选中模时和拖动模时
    void loadFile(); //加载文件
    void setNodeSize(int size); //设置节点大小
    void setLineWidth(int width); //设置线宽
    void setMagnify(int magnify); //设置放大倍数
    void designView(); //设计视图
    void simulateView(); //仿真视图
    void setViewNo(int no); //设置视图号
    void setMagDis(int dis); //设置夸张位移的倍数
    void updateStrainGraph(); //更新应力graph
    void result(); //结果
    void reset(); //重置
    void textRefresh(QString); //刷新文本
    void showOutput(); //显示输出
    void switchSelectMode(); //切换选择模式是节点还是单元
};


class MyThread : public QThread {
Q_OBJECT
public:
    TotalNode* totalNode;
    int begin;
    int end;
    vector<ElementItem*> *ElementItems;
    vector<NodeItem*> *NodeItems;
    QGraphicsScene *scene;
    int magnify;
    int nodeSize;
    int lineWidth;
    MyThread(QObject *parent = 0);
    ~MyThread();
    void run();
signals:
    void isDone();

};





#endif // MAINWINDOW_H
