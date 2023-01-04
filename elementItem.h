#ifndef ELEMNTITEM_H
#define ELEMNTITEM_H

#include "QPointF"
#include "QPainter"
#include "QGraphicsPathItem"
#include "Element.h"
#include "TotalNode.h"
#include "Message.h"
class ElementItem : public QGraphicsPathItem,public Message
{
public:
    int lineWidth = 1;
    int hue = 240;
    QPointF x;
    QPointF y;
    QPointF z;
    QPainterPath path;
    TotalNode *totalNode;
    Element *element_; //这个指向的是总刚里面的element，不是elementItem里面的element
    int index;
    void triangle();
    explicit ElementItem(QPointF x, QPointF y, QPointF z,TotalNode* totalNode,Element* element_,int lineWidth);
    ~ElementItem();
    // void setDelta(TotalNode* totalNode);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
};

#endif