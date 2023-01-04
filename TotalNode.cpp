//
// Created by zhangx on 2022/3/29.
//

#include "TotalNode.h"
#include "chrono"
#include "Eigen/Sparse"
#include "Eigen/SparseCholesky"


TotalNode::TotalNode(int sum, std::vector<Element*> elements,std::vector<Node*> nodes ) : sum(sum) {
    this->elements = elements;
    this->nodes = nodes;
    std::cout<<"construct new TotalNode sum:"<<sum<<std::endl;
    Delta.resize(sum);
    Delta.setZero();
    F.resize(2*sum);
    F.setZero();
    K.resize(2*sum,2*sum);
    K.setZero();
}

TotalNode::TotalNode() {
    std::cout<<"ok!"<<std::endl;
}

Eigen::MatrixXd TotalNode::geneK() {
    auto start = std::chrono::system_clock::now();
    Eigen::MatrixXd K(2*sum,2*sum);
    K.setZero(); //把总刚矩阵初始化
    for(int i=0;i<elements.size();i++) { //对每一个单元操作
        auto eK = calK(elements[i]); //计算该单元的单刚
        for(int j=0;j<3;j++)
            for(int k=0;k<3;k++){
                
                K.block<2,2>(2*(elements[i]->nodes[j]->index-1),2*(elements[i]->nodes[k]->index-1)) += eK.block<2,2>(2*j,2*k);
            }
    }
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout<<"K time:"<<duration.count()<<"ms"<<std::endl;
    // std::ofstream fout;
    // fout.open("KT.txt");
    // fout<<K;
    // fout.close();
    this->K = K;
    return K;
}

Eigen::MatrixXd TotalNode::sym(Eigen::MatrixXd m,int n) {
    for(int i=n-1;i>0;i--)
        for(int j=0;j<i;j++)
            m(i,j) = m(j,i);
    return m;
}
Eigen::Vector<double,Eigen::Dynamic> TotalNode::geneDelta() {
    Eigen::Vector<double,Eigen::Dynamic> Delta(2*sum);
    Delta.setZero();
    for(int i=0;i<sum;i++){
        Delta(2*i) = nodes[i]->u;
        Delta(2*i+1) = nodes[i]->v;
    }
    this->Delta = Delta;
    return Delta;
}

void TotalNode::geneF() {
    for(int i=0;i<elements.size();i++) {
        double s = calDet(elements[i])/2;
        double ro = elements[i]->material->ro;
        double t = elements[i]->material->t;
        double g = elements[i]->material->g;
        double Fy = s*ro*t*g/3;
        for(int j=0;j<3;j++) {
            elements[i]->nodes[j]->fy += Fy;
        }
    }
}

Eigen::Vector<double,Eigen::Dynamic> TotalNode::geneSigma(){
    // Sigma.setZero();
    Eigen::MatrixXd K(2*sum,2*sum);

    auto start = std::chrono::system_clock::now();
    K = this->K;
    auto finish =std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout<<"geneK time:"<<elapsed.count()<<std::endl;
    start = std::chrono::system_clock::now();
    Eigen::Vector<double,Eigen::Dynamic> F(2*sum);
    Eigen::Vector<double,Eigen::Dynamic> Sigma(2*sum);
    Eigen::Vector<double,Eigen::Dynamic> Delta(2*sum);
    for(int i=0;i<elements.size();i++){
        F(2*elements[i]->nodes[0]->index-2) = elements[i]->nodes[0]->fx;
        F(2*elements[i]->nodes[0]->index-1) = elements[i]->nodes[0]->fy;
        F(2*elements[i]->nodes[1]->index-2) = elements[i]->nodes[1]->fx;
        F(2*elements[i]->nodes[1]->index-1) = elements[i]->nodes[1]->fy;
        F(2*elements[i]->nodes[2]->index-2) = elements[i]->nodes[2]->fx;
        F(2*elements[i]->nodes[2]->index-1) = elements[i]->nodes[2]->fy;
    }
    this->F = F;
    for(int i=0;i<elements.size();i++){
        Delta(2*elements[i]->nodes[0]->index-2) = elements[i]->nodes[0]->u;
        Delta(2*elements[i]->nodes[0]->index-1) = elements[i]->nodes[0]->v;
        Delta(2*elements[i]->nodes[1]->index-2) = elements[i]->nodes[1]->u;
        Delta(2*elements[i]->nodes[1]->index-1) = elements[i]->nodes[1]->v;
        Delta(2*elements[i]->nodes[2]->index-2) = elements[i]->nodes[2]->u;
        Delta(2*elements[i]->nodes[2]->index-1) = elements[i]->nodes[2]->v;
    }
    finish = std::chrono::system_clock::now();
    elapsed = finish - start;
    std::cout<<"geneF time:"<<elapsed.count()<<std::endl;
    start = std::chrono::system_clock::now();
    this->Delta = Delta;
    //对角元素改一

    for(int i=0;i<2*sum;i++){
        if(!nodes[i/2]->unknown){
            //改变F矩阵
            for(int j=0;j<2*sum;j++)
                if(nodes[i/2]->unknown)
                    F(j,0) -= K(j,i) * Delta(i, 0);
            F(i,0) = Delta(i, 0);
            //改变k矩阵
            for(int j=0;j<2*sum;j++)
                K(i,j) = 0,K(j,i) = 0;
            K(i,i) = 1;
        }
    }
    finish = std::chrono::system_clock::now();
    elapsed = finish - start;
    std::cout<<"geneSigma time:"<<elapsed.count()<<std::endl;

    start = std::chrono::system_clock::now();
//    cout<<"strat :"<<Delta<<endl;
// 先把K转换为稀疏矩阵
    Eigen::SparseMatrix<double> K_sparse;
    K_sparse = K.sparseView();
    SimplicialLDLT<SparseMatrix<double>> solver(K_sparse);
    Delta = solver.solve(F);
    // Delta = K.ldlt.solve(F);
//    cout<<"ldlt :"<<Delta<<endl;
    finish = std::chrono::system_clock::now();
    elapsed = finish - start;
    std::cout<<"ldlt time:"<<elapsed.count()<<std::endl;
    this->Delta = Delta;
    start = std::chrono::system_clock::now();
    //把每一个delta 带入原来的单元中进行计算F
    //sigma = S * Delta
    for(int i=0;i<elements.size();i++){
        //更新单元中每个节点的Delta
        for(int j=0;j<3;j++){
            elements[i]->nodes[j]->u = Delta(2*elements[i]->nodes[j]->index-2);
            elements[i]->nodes[j]->v = Delta(2*elements[i]->nodes[j]->index-1);
        }
        Eigen::Vector<double,3> eSigma;
        eSigma.setZero();
        eSigma = calS(elements[i])*getDelta(elements[i]);
        for(int j=0;j<3;j++){
            Sigma(elements[i]->nodes[j]->index-1) = eSigma(j);
//            elements[i]->nodes[j]->sigma = eSigma(j);
            elements[i]->sigma[j] = eSigma(j);
        }
    }
    finish = std::chrono::system_clock::now();
    elapsed = finish - start;
    std::cout<<"sigma time:"<<elapsed.count()<<std::endl;
    // this->Sigma = Sigma;
    cout<<"max:"<<Sigma.maxCoeff()<<endl;
    cout<<"min:"<<Sigma.minCoeff()<<endl;
    return Sigma;
}

double TotalNode::getMaxSigma(int n) {
    n = n%3;
    double max = 0;
    for(int i=0;i<elements.size();i++){
        if(elements[i]->sigma[n]>max)
            max = elements[i]->sigma[n];
    }
    return max;
}

double TotalNode::getMinSigma(int n) {
    n = n%3;
    double min = 0;
    for(int i=0;i<elements.size();i++){
        if(elements[i]->sigma[n]<min)
            min = elements[i]->sigma[n];
    }
    return min;
}

void TotalNode::updateSigmaUS(){
    //更新max and min sigma
    maxSigma = getMaxSigma(viewNo);
    minSigma = getMinSigma(viewNo);
    //更新均值
    double sum = 0;
    for(int i=0;i<elements.size();i++){
        sum += elements[i]->sigma[viewNo];
    }
    sigmaMean = sum/elements.size();
    //更新方差
    double sum2 = 0;
    for(int i=0;i<elements.size();i++){
        sum2 += (elements[i]->sigma[viewNo]-sigmaMean)*(elements[i]->sigma[viewNo]-sigmaMean);
    }
    sigmaVar = pow(sum2/elements.size(),0.5);
}

int TotalNode::geneHue(double s){
    int hue = 240,d = 1;
    int huelist[6] = {239,230,200,40,10,1};
    
    if(s<sigmaMean-3*sigmaVar){ // < 3var
        hue = 240;
    }
    else if(s<sigmaMean-2*sigmaVar){ // 3-2 var
        double t1 = sigmaMean-3*sigmaVar;
        double t2 = sigmaMean-2*sigmaVar;
        hue = -(huelist[0]-huelist[1])*(s-t1)/(t2-t1)+huelist[0];
    }
    else if(s<sigmaMean-sigmaVar){ //2-1 var
        double t1 = sigmaMean-2*sigmaVar;
        double t2 = sigmaMean-sigmaVar;
        hue = -(huelist[1]-huelist[2])*(s-t1)/(t2-t1)+huelist[1];

    }
    else if(s<sigmaMean+sigmaVar){ // 1- 1 var
        double t1 = sigmaMean-sigmaVar;
        double t2 = sigmaMean+sigmaVar;
        hue = -(huelist[2]-huelist[3])*(s-t1)/(t2-t1)+huelist[2];

    }
    else if(s<sigmaMean+2*sigmaVar){ // 1- 2 var
        double t1 = sigmaMean+sigmaVar;
        double t2 = sigmaMean+2*sigmaVar;
        hue = -(huelist[3]-huelist[4])*(s-t1)/(t2-t1)+huelist[3];
    }
    else if(s<sigmaMean+3*sigmaVar){ // 2- 3 var
        double t1 = sigmaMean+2*sigmaVar;
        double t2 = sigmaMean+3*sigmaVar;
        hue = -(huelist[4]-huelist[5])*(s-t1)/(t2-t1)+huelist[4];
    }
    else{ // > 3 var
        hue = huelist[5];
    }

    return hue%(360);

}