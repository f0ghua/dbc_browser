#ifndef DBWINDOW_H
#define DBWINDOW_H

#include <QtWidgets>
//#include <QWidget>
#include "dbcport.h"

#define DBC_CATEGORY_MARK           Qt::UserRole + 1
#define DBC_OBJECT_MARK             Qt::UserRole + 2

#define MARK_CATEGORY_NETWORK       1
#define MARK_CATEGORY_ECU           2
#define MARK_CATEGORY_ENV           3
#define MARK_CATEGORY_NODE          4
#define MARK_CATEGORY_MESSAGE       5
#define MARK_CATEGORY_SIGNAL        6

#define MARK_OBJECT_NETWORK         1
#define MARK_OBJECT_ECU             2
#define MARK_OBJECT_ENV             3
#define MARK_OBJECT_NODE            4
#define MARK_OBJECT_MESSAGE         5
#define MARK_OBJECT_SIGNAL          6
#define MARK_OBJECT_TXMESSAGE       7

namespace Ui {
class DbWindow;
}

class DbWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DbWindow(QMainWindow *parent = 0);
    ~DbWindow();
    bool loadFile(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return m_curFile; }

signals:
    void statusEvent(const QString&);

private slots:
    void on_m_tvMain_clicked(const QModelIndex &index);
    void on_m_tvMain_doubleClicked(const QModelIndex &index);

private:
    Ui::DbWindow *ui;
    QStandardItemModel* m_modelTree;
    QStandardItemModel* m_modelTable;

    bool m_isUntitled;
    QString m_curFile;
    Vector::DBC::Network *m_network;

    QString strippedName(const QString &fullFileName);
    void setCurrentFile(const QString &fileName);
    void tvAddNodeNetworks();
    void tvAddNodeECUs();
    void tvAddNodeENVVar();
    void tvAddNodeNetworkNodes();
    void tvAddNodeMessages();
    void tvAddNodeSignals();
    void buildTreeView();
    void prebuildTableView();
    void postbuildTableView();
    void updateTableViewNetworks();
    void updateTableViewECUs();
    void updateTableViewNodes();
    void updateTableViewMessages();
    void updateTableViewSignals();
    void buildTableView();
    void popupDialog(const int objectType, const QModelIndex &index);
};

#endif // DBWINDOW_H
