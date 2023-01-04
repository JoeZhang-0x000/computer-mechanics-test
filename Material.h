//
// Created by zhangx on 2022/3/18.
//

#ifndef COMPUTER_MECHANICS_MATERIAL_H
#define COMPUTER_MECHANICS_MATERIAL_H
#include "iostream"


class Material {
public:
    double E; //弹性模量
    double mi; //泊松比
    double g; //重力加速度
    double ro; //密度
    double t;//厚度
    double getT() const;

    void setT(double t);

    explicit Material(double E = 1E8,double mi = 0.23,double ro = 7E3,double g = 10,double t = 0.01);

    virtual ~Material();

    void toString(){
        std::cout<<"material:"<<" E:"<<E<<" mi:"<<mi<<" ro:"<<ro<<" g:"<<g<<" t:"<<t<<std::endl;
    }

    double getE() const;

    double getMi() const;

    double getG() const;

    double getRo() const;

    void setE(double e);

    void setMi(double mi);

    void setG(double g);

    void setRo(double ro);
};


#endif //COMPUTER_MECHANICS_MATERIAL_H
