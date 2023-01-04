#include <sstream>
#include"node.h"

Node::Node(){

}

Node::Node(int index, double x, double y){
    this->index = index;
    this->x = x;
    this->y = y;
}
QString Node::toString(){
    std::stringstream ss;
    ss << "Node " << this->index << " : (" << this->x << ", " << this->y << ")\n"<<"fx: "<<this->fx<<" fy: "<<this->fy<<"\nu: "<<this->u<<"v: "<<this->v<<"\n";
    return QString::fromStdString(ss.str());
}