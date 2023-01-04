#include "mainwindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{   

    std::cout<<"Hello World"<<std::endl;
    auto start = std::chrono::system_clock::now();
    QApplication a(argc, argv);
    MainWindow w;
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout<<"main time:"<<duration.count()<<"ms"<<std::endl;
    start = std::chrono::system_clock::now();
    w.show();
    end = std::chrono::system_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout<<"show time:"<<duration.count()<<"ms"<<std::endl;
    return a.exec();
}
