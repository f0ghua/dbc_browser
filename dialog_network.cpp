#include "dialog_network.h"
#include "ui_dialog_network.h"

Dialog_Network::Dialog_Network(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Network)
{
    ui->setupUi(this);
}

Dialog_Network::~Dialog_Network()
{
    delete ui;
}
