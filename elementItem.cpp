#include "elementItem.h"
#include "QInputDialog"
#include "iostream"
ElementItem::ElementItem(QPointF x, QPointF y, QPointF z,TotalNode *totalNode,Element *element_,int lineWidth)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->index = element_->index;
    this->totalNode = totalNode;
    this->element_ = element_;
    this->lineWidth = lineWidth;
    triangle(); 
}

ElementItem::~ElementItem()
{

}

void ElementItem::triangle()
{
    path.moveTo(x);
    path.lineTo(y);
    path.lineTo(z);
    path.lineTo(x);
    setPath(path);
}


void ElementItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
//    std::cout<<"点击了单元："<<element_->toString()<<std::endl;
    QString string = "clicked the element:"+element_->toString();
    send(string);
}

void ElementItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
        double maxSigma = totalNode->getMaxSigma(totalNode->viewNo);
        double minSigma = totalNode->getMinSigma(totalNode->viewNo);
        setPen(QPen(Qt::black,lineWidth));

        //根据单元的应力设置颜色

        if(maxSigma == 0&&minSigma == 0) setBrush(Qt::white);
        else
            setBrush(QColor::fromHsl(hue,255,125));
        if(isSelected())  setBrush(QColor::fromHsl(300,255,90));
    QGraphicsPathItem::paint(painter, option, widget);
}

