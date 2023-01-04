#include "method.h"
double calDet(Element *e) {
    Eigen::MatrixXd t(3,3);
    t<<1,e->nodes[0]->x,e->nodes[0]->y,
        1,e->nodes[1]->x,e->nodes[1]->y,
        1,e->nodes[2]->x,e->nodes[2]->y;
//    std::cout<<"t:"<<"\n"<<t<<std::endl;
//    std::cout<<t.determinant()<<std::endl;
    return -t.determinant();
}

MatrixXd calSF(Element *e) {
    Eigen::Matrix<double,3,3> Tem;
    // Tem<<1,e->x[0],e->y[0],
    //     1,e->x[1],e->y[1],
    //     1,e->x[2],e->y[2];
    Tem<<1,e->nodes[0]->x,e->nodes[0]->y,
        1,e->nodes[1]->x,e->nodes[1]->y,
        1,e->nodes[2]->x,e->nodes[2]->y;
    Eigen::Matrix<double,3,3> Ans;//伴随矩阵的转置
    //求代数余子式
    Eigen::Matrix<double,2,2> Ai,Aj,Ak,Bi,Bj,Bk,Ci,Cj,Ck;
    Ai<<Tem(1,1),Tem(1,2),
        Tem(2,1),Tem(2,2);
    Aj<<Tem(0,1),Tem(0,2),
        Tem(2,1),Tem(2,2);
    Ak<<Tem(0,1),Tem(0,2),
        Tem(1,1),Tem(1,2);
    Bi<<Tem(1,0),Tem(1,2),
        Tem(2,0),Tem(2,2);
    Bj<<Tem(0,0),Tem(0,2),
        Tem(2,0),Tem(2,2);
    Bk<<Tem(0,0),Tem(0,2),
        Tem(1,0),Tem(1,2);
    Ci<<Tem(1,0),Tem(1,1),
        Tem(2,0),Tem(2,1);
    Cj<<Tem(0,0),Tem(0,1),
        Tem(2,0),Tem(2,1);
    Ck<<Tem(0,0),Tem(0,1),
        Tem(1,0),Tem(1,1);

    Ans<< Ai.determinant(),-Bi.determinant(),Ci.determinant(),
          -Aj.determinant(),Bj.determinant(),-Cj.determinant(),
          Ak.determinant(),-Bk.determinant(),Ck.determinant();
    Ans.transposeInPlace();
//    std::cout<<"Tem:\n"<<Tem<<"\nAns:\n"<<Ans<<std::endl;
    return Ans;

}

MatrixXd calB(Element *e) {
    Eigen::Matrix<double,3,3> T = calSF(e);
    Eigen::Matrix<double,3,6> B;
    B<< T(1,0),0,T(1,1),0,T(1,2),0,
            0,T(2,0),0,T(2,1),0,T(2,2),
            T(2,0),T(1,0),T(2,1),T(1,1),T(2,2),T(1,2);
    B /=calDet(e);
    return  B;

}

MatrixXd calS(Element *e) {
    Eigen::Matrix<double,3,6> S;
    Eigen::Matrix<double,3,6> B = calB(e);
    Eigen::Matrix<double,3,3> D;
    double mi = e->material->mi;
    D<< 1,mi,0,
            mi,1,0,
            0,0,(1-mi)/2;
    D *= e->material->E/(1-mi*mi);
    S = D*B;
//    std::cout<<"S:\n"<<S<<std::endl;
    return S;

}
MatrixXd calK(Element *e) {
    Eigen::Matrix<double,6,6> K;
    K = calB(e).transpose()*calS(e)*e->material->t*calDet(e)/2;
//    std::cout<<"K:\n"<<K<<std::endl;
    return K;
}

// MatrixXd calK(Element *e) {
//     Eigen::Matrix<double,6,6> K;
//     Eigen::Matrix<double,3,3> T;
//     Eigen::Matrix<double,3,6> B;
//     Eigen::Matrix<double,3,3> D;
//     Eigen::Matrix<double,3,6> S;

//     //1.计算 T
//     Eigen::Matrix<double,3,3> Tem;
//     Tem<<1,e->x[0],e->y[0],
//         1,e->x[1],e->y[1],
//         1,e->x[2],e->y[2];
//     //求代数余子式
//     Eigen::Matrix<double,2,2> Ai,Aj,Ak,Bi,Bj,Bk,Ci,Cj,Ck;
//     Ai<<Tem(1,1),Tem(1,2),
//         Tem(2,1),Tem(2,2);
//     Aj<<Tem(0,1),Tem(0,2),
//         Tem(2,1),Tem(2,2);
//     Ak<<Tem(0,1),Tem(0,2),
//         Tem(1,1),Tem(1,2);
//     Bi<<Tem(1,0),Tem(1,2),
//         Tem(2,0),Tem(2,2);
//     Bj<<Tem(0,0),Tem(0,2),
//         Tem(2,0),Tem(2,2);
//     Bk<<Tem(0,0),Tem(0,2),
//         Tem(1,0),Tem(1,2);
//     Ci<<Tem(1,0),Tem(1,1),
//         Tem(2,0),Tem(2,1);
//     Cj<<Tem(0,0),Tem(0,1),
//         Tem(2,0),Tem(2,1);
//     Ck<<Tem(0,0),Tem(0,1),
//         Tem(1,0),Tem(1,1);

//     T<< Ai.determinant(),-Bi.determinant(),Ci.determinant(),
//           -Aj.determinant(),Bj.determinant(),-Cj.determinant(),
//           Ak.determinant(),-Bk.determinant(),Ck.determinant();
//     T.transposeInPlace();

//     //2.计算B
//     B<< T(1,0),0,T(1,1),0,T(1,2),0,
//             0,T(2,0),0,T(2,1),0,T(2,2),
//             T(2,0),T(1,0),T(2,1),T(1,1),T(2,2),T(1,2);
//     B /=calDet(e);

//     //3.计算S
//     double mi = e->material->mi;
//     D<< 1,mi,0,
//         mi,1,0,
//         0,0,(1-mi)/2;
//     D *= e->material->E/(1-mi*mi);
//     S = D*B;

//     //4.计算K
//     K = B.transpose()*S*e->material->t*calDet(e)/2;
//     return K;
// }

MatrixXd calF(Element *e) {
    Eigen::Matrix<double,6,6> K = calK(e);
    Eigen::Matrix<double,6,1> F;
    Eigen::Matrix<double,6,1> Sigma; //位移矩阵 [ui,vi,uj,vj,uk,bk]^T
    // Sigma<< e->u[0],
    //         e->v[0],
    //         e->u[1],
    //         e->v[1],
    //         e->u[2],
    //         e->v[2];
    Sigma<< e->nodes[0]->u,
            e->nodes[0]->v,
            e->nodes[1]->u, 
            e->nodes[1]->v,
            e->nodes[2]->u,
            e->nodes[2]->v;

    F = K*Sigma;
//    std::cout<<"\nF:\n"<<F<<std::endl;
    return F;
}

MatrixXd calSigma(Element *e){
    Eigen::Matrix<double,6,6> K = calK(e);
    Eigen::Matrix<double,6,1> F;
    Eigen::Matrix<double,6,1> Delta;
    Eigen::Matrix<double,3,1> Sigma;
    // F<< e->Fx[0],
    //     e->Fy[0],
    //     e->Fx[1],
    //     e->Fy[1],
    //     e->Fx[2],
    //     e->Fy[2];
    F<< e->nodes[0]->fx,
        e->nodes[0]->fy,
        e->nodes[1]->fx,
        e->nodes[1]->fy,
        e->nodes[2]->fx,
        e->nodes[2]->fy;

    // Delta << e->u[0],
    //         e->v[0],
    //         e->u[1],
    //         e->v[1],
    //         e->u[2],
    //         e->v[2];
    Delta<< e->nodes[0]->u,
            e->nodes[0]->v,
            e->nodes[1]->u,
            e->nodes[1]->v,
            e->nodes[2]->u,
            e->nodes[2]->v;
    // //对角元素改一
    // bool unknown[6];
    // for(int i=0;i<6;i++)
    //     if(Delta(i, 0) == DBL_MIN)
    //         unknown[i] = true;
    //     else
    //         unknown[i] = false;

    for(int i=0;i<6;i++){
        if(!e->nodes[i/2]->unknown){//即Delta(i)是已知的
            //改变F矩阵
            for(int j=0;j<6;j++)
                if(e->nodes[i/2]->unknown) //如果Delta(j)是未知的，那么就要F(j)-Delta(i)
                    F(j,0) -= K(j,i) * Delta(i, 0);
            F(i,0) = Delta(i, 0);
            //改变k矩阵
            for(int j=0;j<6;j++)
                K(i,j) = 0,K(j,i) = 0;
            K(i,i) = 1;
        }
    }
//    std::cout<<"K now:\n"<<K<<"\nF now:\n"<<F<<std::endl;//对角元素改一之后的K,F
    Delta = K.householderQr().solve(F);
//    std::cout << "Delta:\n" << Delta << std::endl;
    Sigma = calS(e)*Delta;
//    std::cout<<"Sigma:\n"<<Sigma<<std::endl;
    return Sigma;

}

Vector<double,6> getDelta(Element *e){
    Vector<double,6> Delta;
    Delta<< e->nodes[0]->u,
        e->nodes[0]->v,
        e->nodes[1]->u,
        e->nodes[1]->v,
        e->nodes[2]->u,
        e->nodes[2]->v;
//    cout<<"Delta:\n"<<Delta<<endl;
    return Delta;
}