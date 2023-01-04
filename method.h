#ifndef METHOD_H
#define METHOD_H
/*
    *此方法是用于做一些矩阵运算，如求解s,b,k矩阵等，对角元素改一，求解等等
*/
#include <iostream>
#include "Eigen/Dense"
#include "Element.h"

using namespace Eigen;

using namespace std;

MatrixXd calSF(Element *e); //计算形函数

MatrixXd calS(Element *e); //计算S矩阵

MatrixXd calB(Element *e); //计算B矩阵

MatrixXd calK(Element *e); //计算刚度矩阵

MatrixXd calF(Element *e); //计算受力

MatrixXd calSigma(Element *e); //计算位移矩阵

Vector<double,6> getDelta(Element *e); //计算delta

double calDet(Element *e); //计算行列式D


#endif