//
// Created by zhangx on 2022/3/18.
//

#include "Element.h"

Element::Element()
{

}

QString Element::toString() {
    std::stringstream ss;
    ss<<"Element: "<<index<<" nodes: "<<nodes[0]->index<<" "<<nodes[1]->index<<" "<<nodes[2]->index<<" "<<"Sigma: "<<sigma[0]<<" "<<sigma[1]<<" "<<sigma[2]<<"\n";
    return QString::fromStdString(ss.str());
}
Element::Element(int index, Node *nodes) : index(index){
    memccpy(this->nodes, nodes,sizeof(Node),3);
    // std::cout<<"init ok!"<<std::endl;
}


// Eigen::MatrixXd Element::calAlpha() {
//     Eigen::MatrixXd U = Eigen::Map<Eigen::MatrixXd>(this->u,3,1);
//     Eigen::MatrixXd V = Eigen::Map<Eigen::MatrixXd>(this->v,3,1);
//     Eigen::Matrix<double,3,3> T;
//     T<< 1,this->x[0],this->y[0],
//         1,this->x[1],this->y[1],
//         1,this->x[2],this->y[2];
//     Eigen::MatrixXd alp1,alp2;
//     alp1 = T.colPivHouseholderQr().solve(U);
//     alp2 = T.colPivHouseholderQr().solve(V);
//     Eigen::Matrix<double,6,1> alp;
//     alp<<alp1,
//         alp2;
// //    std::cout<<"alp1:\n"<<alp1<<std::endl;
// //    std::cout<<"alp2:\n"<<alp2<<std::endl;
// //    std::cout<<"alp:\n"<<alp<<std::endl;
//     return alp;
// }



// int  Element::calDet() {
//     Eigen::MatrixXd t(3,3);
//     t<<1,this->x[0],this->y[0],
//         1,this->x[1],this->y[1],
//         1,this->x[2],this->y[2];
// //    std::cout<<"t:"<<"\n"<<t<<std::endl;
// //    std::cout<<t.determinant()<<std::endl;
//     return t.determinant();
// }


// /**
//  * 此时的TemAdj即
//  * [[ai,aj,ak],
//  * [bi,bj,bk],
//  * [ci,cj,ck]]
//  */
// Eigen::MatrixXd Element::calSF() {
//     Eigen::Matrix<double,3,3> Tem;
//     Tem<<1,this->x[0],this->y[0],
//         1,this->x[1],this->y[1],
//         1,this->x[2],this->y[2];
//     Eigen::Matrix<double,3,3> Ans;//伴随矩阵的转置
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

//     Ans<< Ai.determinant(),-Bi.determinant(),Ci.determinant(),
//           -Aj.determinant(),Bj.determinant(),-Cj.determinant(),
//           Ak.determinant(),-Bk.determinant(),Ck.determinant();
//     Ans.transposeInPlace();
// //    std::cout<<"Tem:\n"<<Tem<<"\nAns:\n"<<Ans<<std::endl;
//     return Ans;
// }

// Eigen::MatrixXd Element::calB() {
//     Eigen::Matrix<double,3,3> T = calSF();
//     Eigen::Matrix<double,3,6> B;
//     B<< T(1,0),0,T(1,1),0,T(1,2),0,
//             0,T(2,0),0,T(2,1),0,T(2,2),
//             T(2,0),T(1,0),T(2,1),T(1,1),T(2,2),T(1,2);
//     B /=calDet();
//     return  B;
// }

// Eigen::MatrixXd Element::calS() {
//     Eigen::Matrix<double,3,6> S;
//     Eigen::Matrix<double,3,6> B = calB();
//     Eigen::Matrix<double,3,3> D;
//     double mi = this->material->mi;
//     D<< 1,mi,0,
//             mi,1,0,
//             0,0,(1-mi)/2;
//     D *= this->material->E/(1-mi*mi);
//     S = D*B;
// //    std::cout<<"S:\n"<<S<<std::endl;
//     return S;

// }
// Eigen::MatrixXd Element::calK() {
//     Eigen::Matrix<double,6,6> K;
//     K = calB().transpose()*calS()*this->material->t*calDet()/2;
// //    std::cout<<"K:\n"<<K<<std::endl;
//     return K;
// }

// Eigen::MatrixXd Element::calF() {
//     Eigen::Matrix<double,6,6> K = calK();
//     Eigen::Matrix<double,6,1> F;
//     Eigen::Matrix<double,6,1> Sigma; //位移矩阵 [ui,vi,uj,vj,uk,bk]^T
//     Sigma<< u[0],
//             v[0],
//             u[1],
//             v[1],
//             u[2],
//             v[2];
//     F = K*Sigma;
//     std::cout<<"\nF:\n"<<F<<std::endl;
//     return F;
// }

// Eigen::MatrixXd Element::calSigma() {
//     Eigen::Matrix<double,6,6> K = calK();
//     Eigen::Matrix<double,6,1> F;
//     Eigen::Matrix<double,6,1> Delta;
//     Eigen::Matrix<double,3,1> Sigma;
//     F<< Fx[0],
//         Fy[0],
//         Fx[1],
//         Fy[1],
//         Fx[2],
//         Fy[2];
//     Delta << u[0],
//             v[0],
//             u[1],
//             v[1],
//             u[2],
//             v[2];
//     //对角元素改一
//     bool unknown[6];
//     for(int i=0;i<6;i++)
//         if(Delta(i, 0) == DBL_MIN)
//             unknown[i] = true;
//         else
//             unknown[i] = false;

//     for(int i=0;i<6;i++){
//         if(!unknown[i]){//即Sigma(i)是已知量
//             //改变F矩阵
//             for(int j=0;j<6;j++)
//                 if(unknown[j])
//                     F(j,0) -= K(j,i) * Delta(i, 0);
//             F(i,0) = Delta(i, 0);
//             //改变k矩阵
//             for(int j=0;j<6;j++)
//                 K(i,j) = 0,K(j,i) = 0;
//             K(i,i) = 1;
//         }
//     }
// //    std::cout<<"K now:\n"<<K<<"\nF now:\n"<<F<<std::endl;//对角元素改一之后的K,F
//     Delta = K.householderQr().solve(F);
// //    std::cout << "Delta:\n" << Delta << std::endl;
//     Sigma = calS()*Delta;
//     std::cout<<"Sigma:\n"<<Sigma<<std::endl;
//     return Sigma;
// }
















