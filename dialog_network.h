#ifndef DIALOG_NETWORK_H
#define DIALOG_NETWORK_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include "dbcport.h"

namespace Ui {
class Dialog_Network;
}

class Dialog_Network : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_Network(QWidget *parent = 0);
    ~Dialog_Network();
    void setOperateItemValue(QString s) {m_operateItemValue = s;}
    void setOperateItemIndex(const QModelIndex &index) {m_operateItemIndex = index;}
    void updateUIContents();

private slots:
    void on_m_btnOK_clicked();

    void on_m_btnCancel_clicked();

private:
    Ui::Dialog_Network *ui;

    QString m_operateItemValue;
    QModelIndex m_operateItemIndex;
    QStandardItemModel* m_modelTree;
    QStandardItemModel* m_modelTable;
    QString m_curFile;
    Vector::DBC::Network *m_network;

    void prebuildTableView(QTableView *tv);
    void postbuildTableView(QTableView *tv);
    void updateTableViewNodes();
    void buildTreeViewAttributes();
};

#endif // DIALOG_NETWORK_H
