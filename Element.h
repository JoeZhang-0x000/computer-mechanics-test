//
// Created by zhangx on 2022/3/18.
//

#ifndef COMPUTER_MECHANICS_NODE_H
#define COMPUTER_MECHANICS_NODE_H

#include <QString>
#include "Material.h"
#include "node.h"
#include "iostream"
#include "Eigen/Sparse"
#include "cfloat"
class Element {
public:
    int index; //单元的序号
    Node* nodes[3]; //组成这单元的三个节点点 i,j,k
    Material *material = new Material();
    double sigma[3] = {0, 0, 0}; //单元的压力
public:
    Element();
    QString toString();
    Element(int index, Node *nodes);

    // Eigen::MatrixXd calAlpha(); //计算Alpha

    // Eigen::MatrixXd calSF();//计算形函数

    // Eigen::MatrixXd calS();//计算S矩阵

    // Eigen::MatrixXd calB();//计算B矩阵

    // Eigen::MatrixXd calK();//计算刚度矩阵

    // Eigen::MatrixXd calF();//计算受力

    // Eigen::MatrixXd calSigma();//计算位移矩阵

    // int calDet();//计算行列式D

};


#endif //COMPUTER_MECHANICS_NODE_H
