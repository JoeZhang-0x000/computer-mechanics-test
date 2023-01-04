#include "nodeItem.h"
#include "iostream"
#include "nodeeditdialog.h"
#include <QStyleOptionGraphicsItem>

NodeItem::NodeItem(QPointF x,Node *node,int d):QGraphicsEllipseItem(x.x()-d/2,x.y()-d/2,d,d)
{
    this->x = x;
    this->node = node;
}

NodeItem::~NodeItem()
{
    
}

void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    QString str = node->toString();
    send(str);

    NodeEditDialog *dialog = new NodeEditDialog(std::vector<Node*>{node});
    dialog->exec();
    


}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) 
{
    // QStyleOptionGraphicsItem op;
    // op.initFrom(widget);

    // this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setPen(QPen(Qt::red,1));
    this->setZValue(999); //保证节点在最上层绘制
    if(!this->isSelected()){ //没有被选中 红色
        this->setBrush(Qt::red);
    }
    else{ //被选中了 绿色
        this->setBrush(Qt::green);
    }
    if(this->node->unknown== false){
        this->setBrush(Qt::blue);
    }

    QGraphicsEllipseItem::paint(painter, option, widget);
}


