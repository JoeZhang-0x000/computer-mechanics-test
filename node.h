#ifndef NODE_H
#define NODE_H
#include "QString"
class Node{
public:
    bool unknown = true; //默认这个节点的是未知的
    int index = 0; //节点编号
    double x = 0; //x坐标
    double y = 0; //y坐标
    double u = 0; //x方向位移
    double v = 0; //y方向位移
    double fx = 0; //x方向受力
    double fy = 0; //y方向受力

public:
    Node();
    Node(int index, double x, double y);
    QString toString();
};

#endif