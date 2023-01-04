//
// Created by zhangx on 22-5-22.
//

#ifndef COMPUTERMECHANICS_MESSAGE_H
#define COMPUTERMECHANICS_MESSAGE_H


#include <QtCore>

class Message : public QObject {
    Q_OBJECT
public:
    void send(QString message);

};


#endif //COMPUTERMECHANICS_MESSAGE_H
