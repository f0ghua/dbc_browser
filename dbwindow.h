#ifndef DBWINDOW_H
#define DBWINDOW_H

#include <QtWidgets>
//#include <QWidget>
#include "dbcport.h"

#define TYPE_TX_OBJECT              0
#define TYPE_RX_OBJECT              1

#define ROLE_DBC_MARK               Qt::UserRole + 1
#define ROLE_DBC_VALUE			    Qt::UserRole + 2
//#define DBC_CATEGORY_MARK           DBC_ROLE_MARK
//#define DBC_OBJECT_MARK             Qt::UserRole + 2
//#define DBC_ITEM_MARK               Qt::UserRole + 3

#define MARK_CATEGORY_NETWORK       1
#define MARK_CATEGORY_ECU           2
#define MARK_CATEGORY_ENV           3
#define MARK_CATEGORY_NODE          4
#define MARK_CATEGORY_MESSAGE       5
#define MARK_CATEGORY_SIGNAL        6
#define MARK_CATEGORY_END           MARK_CATEGORY_SIGNAL

#define MARK_OBJECT_NETWORK         (MARK_CATEGORY_END+1)
#define MARK_OBJECT_ECU             (MARK_CATEGORY_END+2)
#define MARK_OBJECT_ENV             (MARK_CATEGORY_END+3)
#define MARK_OBJECT_NODE            (MARK_CATEGORY_END+4)
#define MARK_OBJECT_MESSAGE         (MARK_CATEGORY_END+5)
#define MARK_OBJECT_SIGNAL          (MARK_CATEGORY_END+6)
#define MARK_OBJECT_END             MARK_OBJECT_SIGNAL

#define MARK_ITEM_CAT_NETWORK_TXMESSAGE     (MARK_OBJECT_END+1)
#define MARK_ITEM_CAT_NETWORK_SIGNALS       (MARK_OBJECT_END+2)
#define MARK_ITEM_CAT_NETWORK_NODE          (MARK_OBJECT_END+3)
#define MARK_ITEM_CAT_ECU_ECU               (MARK_OBJECT_END+4)
#define MARK_ITEM_CAT_NODES_NODE            (MARK_OBJECT_END+5)
#define MARK_ITEM_CAT_NODES_TXMESSAGES      (MARK_OBJECT_END+6)
#define MARK_ITEM_CAT_NODES_RXMESSAGES      (MARK_OBJECT_END+7)
#define MARK_ITEM_CAT_NODES_TXMAPSIGS       (MARK_OBJECT_END+8)
#define MARK_ITEM_CAT_NODES_RXMAPSIGS       (MARK_OBJECT_END+9)
#define MARK_ITEM_CAT_NODES_TXMESSAGES_MSG	(MARK_OBJECT_END+10)
#define MARK_ITEM_CAT_NODES_RXMESSAGES_MSG	(MARK_OBJECT_END+11)
#define MARK_ITEM_CAT_NODES_TXMAPSIGS_SIG   (MARK_OBJECT_END+12)
#define MARK_ITEM_CAT_NODES_RXMAPSIGS_SIG   (MARK_OBJECT_END+13)
#define MARK_ITEM_CAT_MESSAGES_MSG          (MARK_OBJECT_END+14)
#define MARK_ITEM_CAT_MESSAGES_MSG_SIGNAL   (MARK_OBJECT_END+15)
#define MARK_ITEM_CAT_SIGNALS_SIG           (MARK_OBJECT_END+16)

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
    Vector::DBC::Network* currentNetwork() { return m_network; }

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
    void insertSignalAttributesHeader2Model(std::map<std::string, QString> &mapAttr);
    void insertSignalAttributesValue2Model(int row, std::map<std::string, QString> &mapAttr,Vector::DBC::Message *pMessage, Vector::DBC::Signal *pSignal);
    void prebuildTableView();
    void postbuildTableView();
    void updateTableViewNetworks();
    void updateTableViewECUs();
    void updateTableViewECUsECU(QString ecuName);
    void updateTableViewNodes();
    void updateTableViewNodesMessages(QString nodeName, int msgType);
    void updateTableViewNodesMessagesMsg(unsigned int msgId);
    void updateTableViewNodesMappedSignals(QString nodeName, int type);
    void updateTableViewNodesMappedSignalsSignal(QString nodeName, QString sigName, int type);
    void updateTableViewMessages();
    void updateTableViewSignals();
    void updateTableViewTxMessages();
    void updateTableViewNetworkSignals();
    void updateTableViewNetworkNode(QString nodeName);
	void updateTableViewSignalsSignal(QString sigName, unsigned int msgId);
    void buildTableView();
    void popupDialog(const int itemType, const QModelIndex &index);
};

#endif // DBWINDOW_H
