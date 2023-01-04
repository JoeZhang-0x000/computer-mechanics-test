#include "elementeditdialog.h"
#include "ui_elementeditdialog.h"
#include "Message.h"

ElementEditDialog::ElementEditDialog(std::vector<Element*> elements,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ElementEditDialog)
{
    ui->setupUi(this);
    if (elements.size() == 0)
    {
        std::cout<<"elements is empty!"<<endl;
        return;
    }
    this->elements = elements;
    ui->elastic->setValue(elements[0]->material->E);
    ui->poisson->setValue(elements[0]->material->mi);
    ui->thickness->setValue(elements[0]->material->t);
    ui->density->setValue(elements[0]->material->ro);
    ui->gravity->setValue(elements[0]->material->g);
}

ElementEditDialog::~ElementEditDialog()
{
    delete ui;
}

void ElementEditDialog::accept()
{   
    Message *msg;
    QDialog::accept();
    for(auto element:elements)
    {   
        msg->send("update element:"+QString::number(element->index)+"\n");
        element->material->E = ui->elastic->text().toDouble();
        element->material->mi = ui->poisson->text().toDouble();
        element->material->ro = ui->density->text().toDouble();
        element->material->t = ui->thickness->text().toDouble();
        element->material->g = ui->gravity->text().toDouble();
        msg->send("E:"+QString::number(element->material->E)+" poisson:"+QString::number(element->material->mi)+" ro:"+QString::number(element->material->ro)+" t:"+QString::number(element->material->t)+" g:"+QString::number(element->material->g)+"\n");
    }
    // delete(msg);
}
