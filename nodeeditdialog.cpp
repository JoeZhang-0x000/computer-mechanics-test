#include "nodeeditdialog.h"
#include "ui_nodeeditdialog.h"

NodeEditDialog::NodeEditDialog(std::vector<Node*> nodes,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NodeEditDialog)
{
    ui->setupUi(this);
    if(nodes.size() == 0)
    {
        std::cout<<"nodes is empty!"<<std::endl;
        return;
    }
    this->nodes = nodes;
    ui->uValue->setValue(nodes[0]->u);
    ui->vValue->setValue(nodes[0]->v);
    ui->fxValue->setValue(nodes[0]->fx);
    ui->fyValue->setValue(nodes[0]->fy);
}

NodeEditDialog::~NodeEditDialog()
{
    delete ui;
}

void NodeEditDialog::accept()
{
    QDialog::accept();
    for(auto node:nodes)
    {   
        std::cout<<"修改了节点"<<node->index<<std::endl;
        node->u = ui->uValue->text().toDouble();
        node->v = ui->vValue->text().toDouble();
        node->fx = ui->fxValue->text().toDouble();
        node->fy = ui->fyValue->text().toDouble();
        if(ui->deltaKnown->isChecked()) node->unknown = false;
        else node->unknown = true;
        std::cout<<"u:"<<node->u<<" v:"<<node->v<<" fx:"<<node->fx<<" fy:"<<node->fy<<std::endl; 
    }
}
