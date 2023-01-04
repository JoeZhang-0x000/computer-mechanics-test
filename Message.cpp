//
// Created by zhangx on 22-5-22.
//

#include "Message.h"
void Message::send(QString message) {
    QFile file("output.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << message<<"\n";
    file.close();
}