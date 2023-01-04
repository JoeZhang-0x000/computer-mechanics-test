#ifndef NODEEDITDIALOG_H
#define NODEEDITDIALOG_H

#include <QDialog>
#include "node.h"
#include "iostream"

namespace Ui {
class NodeEditDialog;
}

class NodeEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NodeEditDialog(std::vector<Node*> nodes,QWidget *parent = nullptr);
    ~NodeEditDialog();
    std::vector<Node*> nodes;
    void accept();
    void reject(){QDialog::reject();}


public:
    Ui::NodeEditDialog *ui;
};

#endif // NODEEDITDIALOG_H
