//
// Created by zhangx on 2022/3/29.
//

#ifndef COMPUTER_MECHANICS_TOTALNODE_H
#define COMPUTER_MECHANICS_TOTALNODE_H
#include "Element.h"
#include "vector"
#include "fstream"
#include "method.h"
//总刚
class TotalNode {
public:
    int sum; //节点总数
    bool isMove = false; //if ture show the displacement
    int viewNo = 0; //see the sigma x,sigma y,sigma xy
    double magDis = 1; //夸张位移的倍数
    double maxSigma = 0;
    double minSigma = 0;
    double sigmaMean = 0; //average of sigma
    double sigmaVar = 0; //variance of sigma
    void updateSigmaUS(); //更新sigmaU和SigmaS
    int geneHue(double s); //according to the sigma to generate the hue of a element,use the 0-240
    std::vector<Element*> elements; //存储单元的数组
    std::vector<Node*> nodes; //存储节点的数组
    Eigen::MatrixXd K; //总刚矩阵
    Eigen::Vector<double,Eigen::Dynamic> Delta; //位移矩阵
    Eigen::Vector<double,Eigen::Dynamic> F; //力矩阵
    // Eigen::Vector<double,Eigen::Dynamic> Sigma; //应力矩阵
public:
    TotalNode();

    TotalNode(int sum, std::vector<Element*> elements, std::vector<Node*> nodes);

    Eigen::MatrixXd geneK(); //生成总纲矩阵

    Eigen::MatrixXd sym(Eigen::MatrixXd m,int n); // 对称矩阵

    void geneF(); //add gravity force

    Eigen::Vector<double,Eigen::Dynamic> geneDelta(); //生成位移矩阵

    Eigen::Vector<double,Eigen::Dynamic> geneSigma(); //计算应力矩阵

    double getMaxSigma(int n); //获取最大应力

    double getMinSigma(int n); //获取最小应力





};


#endif //COMPUTER_MECHANICS_TOTALNODE_H
