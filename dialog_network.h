#ifndef DIALOG_NETWORK_H
#define DIALOG_NETWORK_H

#include <QDialog>

namespace Ui {
class Dialog_Network;
}

class Dialog_Network : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Network(QWidget *parent = 0);
    ~Dialog_Network();

private:
    Ui::Dialog_Network *ui;
};

#endif // DIALOG_NETWORK_H
