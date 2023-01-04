
#ifndef __NODEITEM_H
#define __NOEITEM_H
#include "QPointF"
#include "QPainter"
#include "QGraphicsEllipseItem"
#include "node.h"
#include "Message.h"


class NodeItem : public QGraphicsEllipseItem,public Message
{

public:
    Node* node;
    QPointF x; //节点坐标
    NodeItem(QPointF x,Node *node,int d);
    ~NodeItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event); //双击事件



    
    
};


#endif
