#ifndef ELEMENTEDITDIALOG_H
#define ELEMENTEDITDIALOG_H

#include "element.h"
#include "iostream"
#include <QDialog>

namespace Ui {
class ElementEditDialog;
}

class ElementEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ElementEditDialog(std::vector<Element*> elements,QWidget *parent = nullptr);
    ~ElementEditDialog();
    std::vector<Element*> elements;
    void accept();
    void reject(){QDialog::reject();}

private:
    Ui::ElementEditDialog *ui;
};

#endif // ELEMENTEDITDIALOG_H

