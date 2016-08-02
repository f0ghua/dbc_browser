#include "dbwindow.h"
#include "ui_dbwindow.h"
#include "gbldata.h"
#include "dialog_network.h"
#include <QStatusBar>

DbWindow::DbWindow(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::DbWindow)
{
    // Intialize the variables
    ui->setupUi(this);

    ui->m_splMain->setContentsMargins(0, 0, 0, 0);
    ui->m_tvMain->setStyleSheet("::item {padding: 2px}");
    ui->m_tblMain->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    // resize will make the speed slow, so we can disable it in old pc
    //ui->m_tblMain->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->m_tblMain->verticalHeader()->setDefaultSectionSize(16);

    m_network       = new Vector::DBC::Network();
    m_modelTree     = new QStandardItemModel(ui->m_tvMain);
    m_modelTable    = new QStandardItemModel(ui->m_tblMain);
}

DbWindow::~DbWindow()
{
    delete ui;
    delete m_network;
#if 0
    delete m_modelTree;
    delete m_modelTable;
#endif
}

void DbWindow::tvAddNodeNetworks()
{
    QString dbcName;
    QStandardItem *item;

    dbcName = GblData::dbcGetDBName(m_curFile, m_network);

#ifndef F_NO_DEBUG
    qDebug() << "dbcName = " << dbcName << endl;
#endif

    QStandardItem *itemNetworks = new QStandardItem(
        GblData::m_publicIconMap[ITEM_ICON_Network],
        QStringLiteral("Networks"));
    itemNetworks->setData(MARK_CATEGORY_NETWORK, ROLE_DBC_MARK);
    m_modelTree->appendRow(itemNetworks);

    QStandardItem *itemNetwork = new QStandardItem(
        GblData::m_publicIconMap[ITEM_ICON_Network],
        dbcName);
    itemNetwork->setData(MARK_OBJECT_NETWORK, ROLE_DBC_MARK);
    itemNetworks->appendRow(itemNetwork);

    item = new QStandardItem(
        GblData::m_publicIconMap[ITEM_ICON_Message],
        QStringLiteral("Tx Messages"));
    item->setData(MARK_ITEM_CAT_NETWORK_TXMESSAGE, ROLE_DBC_MARK);
    itemNetwork->appendRow(item);

    item = new QStandardItem(
        GblData::m_publicIconMap[ITEM_ICON_Signal],
        QStringLiteral("Signals"));
    item->setData(MARK_ITEM_CAT_NETWORK_SIGNALS, ROLE_DBC_MARK);
    itemNetwork->appendRow(item);

    for (auto node : m_network->nodes)
    {
        QString sName = QString::fromStdString(node.second.name);

        item = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_Node],
            sName);
        item->setData(MARK_ITEM_CAT_NETWORK_NODE, ROLE_DBC_MARK);
        itemNetwork->appendRow(item);
    }
}

void DbWindow::tvAddNodeECUs()
{
    QStandardItem *item;

    QStandardItem *itemECUs = new QStandardItem(
        GblData::m_publicIconMap[ITEM_ICON_ECU],
        QStringLiteral("ECUs"));
    itemECUs->setData(MARK_CATEGORY_ECU, ROLE_DBC_MARK);
    m_modelTree->appendRow(itemECUs);

    std::map<std::string, Vector::DBC::Node> &nodes = m_network->nodes;
    std::map<std::string, Vector::DBC::Node>::iterator it;

    for (it = nodes.begin(); it != nodes.end(); it++)
    {
        QString sName = QString::fromStdString((*it).second.name);

        QStandardItem *itemECU = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_ECU],
            sName);
        itemECU->setData(MARK_ITEM_CAT_ECU_ECU, ROLE_DBC_MARK);
        itemECUs->appendRow(itemECU);

        item = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_ENV],
            QStringLiteral("Environment Variables"));
        itemECU->appendRow(item);

        item = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_ECU],
            sName);
        item->setData(MARK_ITEM_CAT_NETWORK_NODE, ROLE_DBC_MARK);
        itemECU->appendRow(item);
    }
}

void DbWindow::tvAddNodeENVVar()
{
    QStandardItem *item;

    item = new QStandardItem(
        GblData::m_publicIconMap[ITEM_ICON_ENV],
        QStringLiteral("Environment Variables"));
    m_modelTree->appendRow(item);
}

void DbWindow::tvAddNodeNetworkNodes()
{
    QStandardItem *item;

    QStandardItem *itemNNode = new QStandardItem(
        GblData::m_publicIconMap[ITEM_ICON_Network],
        QStringLiteral("Network nodes"));
    itemNNode->setData(MARK_CATEGORY_NODE, ROLE_DBC_MARK);
    m_modelTree->appendRow(itemNNode);

    for (auto node : m_network->nodes)
    {
        QString sName = QString::fromStdString(node.second.name);

        QStandardItem *itemNode = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_Node],
            sName);
        itemNode->setData(MARK_ITEM_CAT_NODES_NODE, ROLE_DBC_MARK);
        itemNNode->appendRow(itemNode);

        QStandardItem *itemTxMsg = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_Message],
            QStringLiteral("Tx Messages"));
        itemTxMsg->setData(MARK_ITEM_CAT_NODES_TXMESSAGES, ROLE_DBC_MARK);
        itemNode->appendRow(itemTxMsg);

        QStandardItem *itemRxMsg = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_Message],
            QStringLiteral("Rx Messages"));
        itemRxMsg->setData(MARK_ITEM_CAT_NODES_RXMESSAGES, ROLE_DBC_MARK);
        itemNode->appendRow(itemRxMsg);

        QStandardItem *itemTxMapSignal = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_Signal],
            QStringLiteral("Mapped Tx Signals"));
        itemTxMapSignal->setData(MARK_ITEM_CAT_NODES_TXMAPSIGS, ROLE_DBC_MARK);
        itemNode->appendRow(itemTxMapSignal);

        QStandardItem *itemRxMapSignal = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_Signal],
            QStringLiteral("Mapped Rx Signals"));
        itemRxMapSignal->setData(MARK_ITEM_CAT_NODES_RXMAPSIGS, ROLE_DBC_MARK);
        itemNode->appendRow(itemRxMapSignal);

        std::set<Vector::DBC::Message *> setMsg;
        setMsg = GblData::dbcGetTxMessages4Node(node.second.name, m_network);
        for (auto txMsg : setMsg)
        {
            QString msgStr = QString::fromStdString(txMsg->name) +
                QString("(0x") + QString::number(txMsg->id, 16).toUpper() + QString(")");
            item = new QStandardItem(
                GblData::m_publicIconMap[ITEM_ICON_Message],
                msgStr);
            itemTxMsg->appendRow(item);

            for (auto signalPair : txMsg->signal)
            {
                item = new QStandardItem(
                    GblData::m_publicIconMap[ITEM_ICON_Signal],
                    QString::fromStdString(signalPair.second.name));
                item->setData(MARK_ITEM_CAT_NODES_TXMAPSIGS_SIG, ROLE_DBC_MARK);
                itemTxMapSignal->appendRow(item);
            }
        }

        setMsg = GblData::dbcGetRxMessages4Node(node.second.name, m_network);
        for (auto rxMsg : setMsg)
        {
            QString msgStr = QString::fromStdString(rxMsg->name) +
                QString("(0x") + QString::number(rxMsg->id, 16).toUpper() + QString(")");
            item = new QStandardItem(
                GblData::m_publicIconMap[ITEM_ICON_Message],
                msgStr);
            itemRxMsg->appendRow(item);

            for (auto signalPair : rxMsg->signal)
            {
                item = new QStandardItem(
                    GblData::m_publicIconMap[ITEM_ICON_Signal],
                    QString::fromStdString(signalPair.second.name));
                item->setData(MARK_ITEM_CAT_NODES_RXMAPSIGS_SIG, ROLE_DBC_MARK);
                itemRxMapSignal->appendRow(item);
            }
        }

    }
}

void DbWindow::tvAddNodeMessages()
{
    QStandardItem *item;

    QStandardItem *itemMessages = new QStandardItem(
        GblData::m_publicIconMap[ITEM_ICON_Message],
        QStringLiteral("Messages"));
    itemMessages->setData(MARK_CATEGORY_MESSAGE, ROLE_DBC_MARK);
    m_modelTree->appendRow(itemMessages);

    for (auto message : m_network->messages)
    {
        QString sName = QString::fromStdString(message.second.name);

        item = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_Message],
            sName);
        itemMessages->appendRow(item);
    }
}

void DbWindow::tvAddNodeSignals()
{
    QStandardItem *item;

    QStandardItem *itemSignals = new QStandardItem(
        GblData::m_publicIconMap[ITEM_ICON_Signal],
        QStringLiteral("Signals"));
    itemSignals->setData(MARK_CATEGORY_SIGNAL, ROLE_DBC_MARK);
    m_modelTree->appendRow(itemSignals);

    for (auto message : m_network->messages)
    {
        for (auto signal : message.second.signal)
        {
            QString sName = QString::fromStdString(signal.second.name);

            item = new QStandardItem(
                GblData::m_publicIconMap[ITEM_ICON_Signal],
                sName);
            itemSignals->appendRow(item);
        }
    }
}

void DbWindow::buildTreeView()
{
    tvAddNodeNetworks();
    tvAddNodeECUs();
    tvAddNodeENVVar();
    tvAddNodeNetworkNodes();
    tvAddNodeMessages();
    tvAddNodeSignals();

    ui->m_tvMain->setModel(m_modelTree);
}

void DbWindow::prebuildTableView()
{
    m_modelTable->removeRows(0, m_modelTable->rowCount());
    ui->m_tblMain->reset();
}

void DbWindow::postbuildTableView()
{
    ui->m_tblMain->setModel(m_modelTable);
    ui->m_tblMain->resizeColumnsToContents();
    ui->m_tblMain->setColumnWidth(0,120);
    ui->m_tblMain->setColumnWidth(1,120);
}

void DbWindow::updateTableViewNetworks()
{
    int row = 0, col = 0, count = 0;
    QMap<QString, Vector::DBC::Attribute *> mapNA;
    QString strAttrValue;
    QStandardItem *item;

    prebuildTableView();

    count = GblData::dbcGetNetworkAttributes(mapNA, m_network);
    count += 3; // add Name, Protocol and Comment
    m_modelTable->setColumnCount(count);

    m_modelTable->setHeaderData(col, Qt::Horizontal, QStringLiteral("Name"));
    QString dbcName = GblData::dbcGetDBName(m_curFile, m_network);
    item = new QStandardItem(GblData::m_publicIconMap[ITEM_ICON_Network], dbcName);
    m_modelTable->setItem(row, col++, item);

    m_modelTable->setHeaderData(col, Qt::Horizontal, QStringLiteral("Protocol"));
    item = new QStandardItem(QStringLiteral("CAN"));
    m_modelTable->setItem(row, col++, item);

    m_modelTable->setHeaderData(col, Qt::Horizontal, QStringLiteral("Comment"));
    item = new QStandardItem(QStringLiteral(""));
    m_modelTable->setItem(row, col++, item);

    QMap<QString, Vector::DBC::Attribute *>::const_iterator pairNA;
    for( pairNA=mapNA.constBegin(); pairNA!=mapNA.constEnd(); ++pairNA)
    {
        strAttrValue = GblData::dbcGetStrAttributeValue(
                pairNA.value(),
                Vector::DBC::AttributeDefinition::ObjectType::Network,
                m_network);

        m_modelTable->setHeaderData(col, Qt::Horizontal, pairNA.key());
        item = new QStandardItem(strAttrValue);
        m_modelTable->setItem(row, col++, item);
    }

    postbuildTableView();
}

void DbWindow::updateTableViewTxMessages()
{
    int row = 0, col = 0, colCount = 0;
    QString qTempStr;
    std::set<std::string> setAttrsName;
    std::map<std::string, QString> mapAttr; // store attribute default value
    std::set<std::string>::iterator itSet;
    std::map<std::string, Vector::DBC::Attribute>::iterator itMap;
    QStandardItem *item;

    prebuildTableView();

    GblData::dbcGetAttributeDefinitions(
        setAttrsName,
        Vector::DBC::AttributeDefinition::ObjectType::Message,
        m_network
        );

    for(itSet=setAttrsName.begin(); itSet!=setAttrsName.end(); ++itSet)
    {
#ifndef F_NO_DEBUG
        qDebug() << QString::fromStdString(*itSet) << endl;
#endif
        itMap = m_network->attributeDefaults.find((*itSet));

        if ((itMap) != m_network->attributeDefaults.end())
        {
            qTempStr = GblData::dbcGetStrAttributeValue(
                        &itMap->second,
                        Vector::DBC::AttributeDefinition::ObjectType::Message,
                        m_network);

            mapAttr.insert(
                std::map<std::string, QString>::value_type(
                    (*itSet), qTempStr));
        }
        else
        {
            mapAttr.insert(
                std::map<std::string, QString>::value_type((*itSet), ""));
        }
    }

    colCount = 6 + setAttrsName.size();

#ifndef F_NO_DEBUG
    qDebug() << "colCount = " << colCount << endl;
#endif

    m_modelTable->setColumnCount(colCount);

    col = 0;
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Message");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Node");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "ID");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "ID-Format");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "DLC");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Transmitter");
    for(auto attrName : setAttrsName)
    {
        m_modelTable->setHeaderData(col++, Qt::Horizontal,
            QString::fromStdString(attrName));
    }

    /* loop over messages */
    for (auto message : m_network->messages)
    {
        if (!message.second.transmitter.empty())
        {
            col = 0;

            item = new QStandardItem(GblData::m_publicIconMap[ITEM_ICON_Message],
                QString::fromStdString(message.second.name));
            m_modelTable->setItem(row, col++, item);

            item = new QStandardItem(QString::fromStdString(message.second.transmitter));
            m_modelTable->setItem(row, col++, item);

            QString sId = QString("0x") + QString::number(message.second.id, 16).toUpper();
            item = new QStandardItem(sId);
            m_modelTable->setItem(row, col++, item);

            item = new QStandardItem(QStringLiteral("CAN Standard"));
            m_modelTable->setItem(row, col++, item);

            item = new QStandardItem(QString::number(message.second.size, 10));
            m_modelTable->setItem(row, col++, item);

            item = new QStandardItem(QString::fromStdString(message.second.transmitter));
            m_modelTable->setItem(row, col++, item);

            for(itSet=setAttrsName.begin(); itSet!=setAttrsName.end(); ++itSet)
            {
                itMap = message.second.attributeValues.find((*itSet));
                if ((itMap) != message.second.attributeValues.end())
                {
                    qTempStr = GblData::dbcGetStrAttributeValue(
                        &itMap->second,
                        Vector::DBC::AttributeDefinition::ObjectType::Message,
                        m_network
                        );
                    item = new QStandardItem(qTempStr);
                    m_modelTable->setItem(row, col++, item);
                }
                else
                {
                    item = new QStandardItem(mapAttr[*itSet]);
                    m_modelTable->setItem(row, col++, item);
                }
            }

            row++;
        }
        else
        {
            for (auto transmitter : message.second.transmitters)
            {
                col = 0;

                item = new QStandardItem(GblData::m_publicIconMap[ITEM_ICON_Message],
                    QString::fromStdString(message.second.name));
                m_modelTable->setItem(row, col++, item);

                item = new QStandardItem(QString::fromStdString(transmitter));
                m_modelTable->setItem(row, col++, item);

                QString sId = QString("0x") + QString::number(message.second.id, 16).toUpper();
                item = new QStandardItem(sId);
                m_modelTable->setItem(row, col++, item);

                item = new QStandardItem(QStringLiteral("CAN Standard"));
                m_modelTable->setItem(row, col++, item);

                item = new QStandardItem(QString::number(message.second.size, 10));
                m_modelTable->setItem(row, col++, item);

                item = new QStandardItem(QString::fromStdString(transmitter));
                m_modelTable->setItem(row, col++, item);

                for(itSet=setAttrsName.begin(); itSet!=setAttrsName.end(); ++itSet)
                {
                    itMap = message.second.attributeValues.find((*itSet));
                    if ((itMap) != message.second.attributeValues.end())
                    {
                        qTempStr = GblData::dbcGetStrAttributeValue(
                            &itMap->second,
                            Vector::DBC::AttributeDefinition::ObjectType::Message,
                            m_network
                            );
                        item = new QStandardItem(qTempStr);
                        m_modelTable->setItem(row, col++, item);
                    }
                    else
                    {
                        item = new QStandardItem(mapAttr[*itSet]);
                        m_modelTable->setItem(row, col++, item);
                    }
                }

                row++;
            }
        }
    }

    postbuildTableView();

    statusBar()->showMessage(tr("%1 Messages").arg(row));
}

void DbWindow::updateTableViewNetworkSignals()
{
    int row = 0, col = 0, colCount = 0;
    QString qTempStr;
    std::set<std::string> setAttrsName;
    std::map<std::string, QString> mapAttr; // store attribute default value
    std::set<std::string>::iterator itSet;
    std::map<std::string, Vector::DBC::Attribute>::iterator itMap;

    prebuildTableView();

    GblData::dbcGetAttributesDefault(
        mapAttr,
        Vector::DBC::AttributeDefinition::ObjectType::Signal,
        m_network);

    colCount = 9 + mapAttr.size();

#ifndef F_NO_DEBUG
    qDebug() << "colCount = " << colCount << endl;
#endif

    m_modelTable->setColumnCount(colCount);

    col = 0;
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Name");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Message");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Multiplexing/Group");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Startbit");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Transmitter");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Length");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Byte Order");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Value Type");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Comment");

    for(auto attrPair : mapAttr)
    {
        m_modelTable->setHeaderData(col++, Qt::Horizontal,
            QString::fromStdString(attrPair.first));
    }

    /* loop over signals */
    for (auto message : m_network->messages)
    for (auto signalPair : message.second.signal)
    {
        insertSignalAttributesValue2Model(row, mapAttr, &message.second, &signalPair.second);
        row++;
    }

    postbuildTableView();

    statusBar()->showMessage(tr("%1 Signals").arg(row));

}

void DbWindow::insertSignalAttributesHeader2Model(std::map<std::string, QString> &mapAttr)
{
    int col = 0;

    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Name");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Message");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Multiplexing/Group");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Startbit");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Length");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Byte Order");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Value Type");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Factor");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Offset");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Minimum");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Maximum");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Unit");
    //m_modelTable->setHeaderData(col++, Qt::Horizontal, "Value Table");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Comment");

    for(auto attrPair : mapAttr)
    {
        m_modelTable->setHeaderData(col++, Qt::Horizontal,
            QString::fromStdString(attrPair.first));
    }
}

void DbWindow::insertSignalAttributesValue2Model(
    int row,
    std::map<std::string, QString> &mapAttr,
    Vector::DBC::Message *pMessage,
    Vector::DBC::Signal *pSignal
    )
{
    int col = 0;
    QStandardItem *item;
    QString qTempStr;
    std::map<std::string, QString>::iterator it;
    std::map<std::string, Vector::DBC::Attribute>::iterator itMap;

    item = new QStandardItem(GblData::m_publicIconMap[ITEM_ICON_Signal],
        QString::fromStdString(pSignal->name));
    m_modelTable->setItem(row, col++, item);

    item = new QStandardItem(QString::fromStdString(pMessage->name));
    m_modelTable->setItem(row, col++, item);

    std::string tempStr = GblData::dbcGetMessageSignalGroup(
        pSignal->name,
        pMessage
        );
    if (tempStr.empty())
    {
        item = new QStandardItem(QStringLiteral("-"));
    }
    else
    {
        item = new QStandardItem(QString::fromStdString(tempStr));
    }
    m_modelTable->setItem(row, col++, item);

    item = new QStandardItem(QString::number(pSignal->startBit, 10));
    m_modelTable->setItem(row, col++, item);
#if 0
    if (pMessage->transmitter.empty())
    {
        item = new QStandardItem(QStringLiteral("<<multiple>>"));
    }
    else
    {
        item = new QStandardItem(QString::fromStdString(pMessage->transmitter));
    }
    m_modelTable->setItem(row, col++, item);
#endif
    item = new QStandardItem(QString::number(pSignal->bitSize, 10));
    m_modelTable->setItem(row, col++, item);

    if (pSignal->byteOrder == Vector::DBC::ByteOrder::BigEndian)
        qTempStr = QObject::tr("Motorola");
    else
        qTempStr = QObject::tr("Intel");
    item = new QStandardItem(qTempStr);
    m_modelTable->setItem(row, col++, item);


    if (pSignal->valueType == Vector::DBC::ValueType::Unsigned)
        qTempStr = QObject::tr("Unsigned");
    else
        qTempStr = QObject::tr("Signed");
    item = new QStandardItem(qTempStr);
    m_modelTable->setItem(row, col++, item);

    item = new QStandardItem(QString::number(pSignal->factor, 10, 0));
    m_modelTable->setItem(row, col++, item);

    item = new QStandardItem(QString::number(pSignal->offset, 10, 0));
    m_modelTable->setItem(row, col++, item);

    item = new QStandardItem(QString::number(pSignal->minimumPhysicalValue, 10, 0));
    m_modelTable->setItem(row, col++, item);

    if (pSignal->maximumPhysicalValue < 65535)
        item = new QStandardItem(QString::number(pSignal->maximumPhysicalValue, 10, 0));
    else
        item = new QStandardItem(QObject::tr("%1").arg(pSignal->maximumPhysicalValue,0,'e',3));
    m_modelTable->setItem(row, col++, item);

    item = new QStandardItem(QString::fromStdString(pSignal->unit));
    m_modelTable->setItem(row, col++, item);

    item = new QStandardItem(QString::fromStdString(pSignal->comment));
    m_modelTable->setItem(row, col++, item);

    for(it=mapAttr.begin(); it!=mapAttr.end(); ++it)
    {
        itMap = pSignal->attributeValues.find((*it).first);
        if ((itMap) != pSignal->attributeValues.end())
        {
            qTempStr = GblData::dbcGetStrAttributeValue(
                &itMap->second,
                Vector::DBC::AttributeDefinition::ObjectType::Signal,
                m_network
                );
            item = new QStandardItem(qTempStr);
            m_modelTable->setItem(row, col++, item);
        }
        else
        {
            item = new QStandardItem(mapAttr[(*it).first]);
            m_modelTable->setItem(row, col++, item);
        }
    }
}

void DbWindow::updateTableViewNetworkNode(QString nodeName)
{
    int row = 0, colCount = 0;
    QString qTempStr;
    std::set<std::string> setAttrsName;
    std::map<std::string, QString> mapAttr; // store attribute default value
    std::set<std::string>::iterator itSet;
    std::map<std::string, Vector::DBC::Attribute>::iterator itMap;
//    QStandardItem *item;

    prebuildTableView();

    GblData::dbcGetAttributesDefault(
        mapAttr,
        Vector::DBC::AttributeDefinition::ObjectType::Signal,
        m_network);

    colCount = 13 + mapAttr.size();

#ifndef F_NO_DEBUG
    qDebug() << "colCount = " << colCount << endl;
#endif

    m_modelTable->setColumnCount(colCount);

    insertSignalAttributesHeader2Model(mapAttr);

    std::set<Vector::DBC::Message *> setMsg;
    setMsg = GblData::dbcGetTxMessages4Node(nodeName.toStdString(), m_network);
    for (auto txMsg : setMsg)
    {
        for (auto signalPair : txMsg->signal)
        {
            insertSignalAttributesValue2Model(row, mapAttr, txMsg, &signalPair.second);
            row++;
        }
    }

    setMsg = GblData::dbcGetRxMessages4Node(nodeName.toStdString(), m_network);
    for (auto rxMsg : setMsg)
    {
        for (auto signalPair : rxMsg->signal)
        {
            insertSignalAttributesValue2Model(row, mapAttr, rxMsg, &signalPair.second);
            row++;
        }
    }

    postbuildTableView();

    statusBar()->showMessage(tr("%1 Signals").arg(row));
}

// TABLE ECUs ------------------------------------------------------------

void DbWindow::updateTableViewECUs()
{
    int row = 0;

    prebuildTableView();

    m_modelTable->setColumnCount(3);
    m_modelTable->setHeaderData(0, Qt::Horizontal, "Name");
    m_modelTable->setHeaderData(1, Qt::Horizontal, "Comment");
    m_modelTable->setHeaderData(2, Qt::Horizontal, "");

    for (auto node: m_network->nodes)
    {
        QString nodeName = QString::fromStdString(node.second.name);

        QStandardItem *item = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_Network], nodeName);
        m_modelTable->setItem(row, 0, item);

        item = new QStandardItem(QStringLiteral(""));
        m_modelTable->setItem(row, 1, item);

        row++;
    }

    postbuildTableView();
}

void DbWindow::updateTableViewECUsECU(QString ecuName)
{
    int row = 0, col = 0, colCount = 0;
    QString qTempStr;
    std::map<std::string, QString> mapAttr; // store attribute default value
    std::map<std::string, Vector::DBC::Node>::iterator itNodes;
    QStandardItem *item;

    prebuildTableView();

    if ((itNodes = m_network->nodes.find(ecuName.toStdString())) == m_network->nodes.end())
        return;

    colCount = 2 + (*itNodes).second.attributeValues.size();
    m_modelTable->setColumnCount(colCount);

    m_modelTable->setHeaderData(col, Qt::Horizontal, "Name");
    QString nodeName = QString::fromStdString((*itNodes).second.name);
    item = new QStandardItem(
        GblData::m_publicIconMap[ITEM_ICON_Network], nodeName);
    m_modelTable->setItem(row, col++, item);

    m_modelTable->setHeaderData(col, Qt::Horizontal, "ECU");
    item = new QStandardItem(nodeName);
    m_modelTable->setItem(row, col++, item);

    m_modelTable->setHeaderData(col, Qt::Horizontal, "Comment");
    item = new QStandardItem(QStringLiteral(""));
    m_modelTable->setItem(row, col++, item);

    for (auto attr : (*itNodes).second.attributeValues)
    {
        m_modelTable->setHeaderData(col, Qt::Horizontal, QString::fromStdString(attr.second.name));

        qTempStr = GblData::dbcGetStrAttributeValue(
                &attr.second,
                Vector::DBC::AttributeDefinition::ObjectType::Node,
                m_network
            );
        item = new QStandardItem(qTempStr);
        m_modelTable->setItem(row, col++, item);
    }

    postbuildTableView();
}

// TABLE Network Nodes ------------------------------------------------------------

void DbWindow::updateTableViewNodes()
{
    int row = 0, col = 0, colCount = 0;
    int attributeCount = 0;
    std::map<std::string, Vector::DBC::Node>::iterator iterNodePair;
    std::map<std::string, Vector::DBC::Attribute>::iterator iterAttrPair;
    QStandardItem *item;

    prebuildTableView();

    iterNodePair = m_network->nodes.begin();
    if (iterNodePair == m_network->nodes.end())
        return;
    attributeCount = (*iterNodePair).second.attributeValues.size();
    colCount = 3 + attributeCount;

#ifndef F_NO_DEBUG
    qDebug() << "colCount = " << colCount << endl;
#endif

    m_modelTable->setColumnCount(colCount);

    col = 0;
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Name");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Address");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Comment");
    for(auto attribute : (*iterNodePair).second.attributeValues)
    {
        m_modelTable->setHeaderData(col++, Qt::Horizontal,
            QString::fromStdString(attribute.second.name));
    }

    for(auto node : m_network->nodes)
    {
        col = 0;

        item = new QStandardItem(GblData::m_publicIconMap[ITEM_ICON_Node],
            QString::fromStdString(node.second.name));
        m_modelTable->setItem(row, col++, item);

        item = new QStandardItem(QStringLiteral("0x00"));
        m_modelTable->setItem(row, col++, item);

        item = new QStandardItem(QStringLiteral(" "));
        m_modelTable->setItem(row, col++, item);

        for(auto attribute : node.second.attributeValues)
        {
            QString strValue = GblData::dbcGetStrAttributeValue(
                &attribute.second,
                Vector::DBC::AttributeDefinition::ObjectType::Node,
                m_network
                );
            item = new QStandardItem(strValue);
            m_modelTable->setItem(row, col++, item);
        }

        row++;
    }

    postbuildTableView();

    statusBar()->showMessage(tr("%1 Nodes").arg(row));
}

void DbWindow::updateTableViewNodesMessages(QString nodeName, int msgType)
{
    int row = 0, col = 0, colCount = 0;
    QString qTempStr;
    std::map<std::string, QString> mapAttr;
    std::map<std::string, QString>::iterator it;
    std::map<std::string, Vector::DBC::Attribute>::iterator itMap;
    QStandardItem *item;

    prebuildTableView();

    GblData::dbcGetAttributesDefault(
        mapAttr,
        Vector::DBC::AttributeDefinition::ObjectType::Message,
        m_network);

    colCount = 6 + mapAttr.size();
    m_modelTable->setColumnCount(colCount);

    col = 0;
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Message");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Node");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "ID");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "ID-Format");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "DLC");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Comment");
    for(auto attrPair : mapAttr)
    {
        m_modelTable->setHeaderData(col++, Qt::Horizontal,
            QString::fromStdString(attrPair.first));
    }

    std::set<Vector::DBC::Message *> setMsg;

    if (msgType == TYPE_TX_OBJECT)
        setMsg = GblData::dbcGetTxMessages4Node(nodeName.toStdString(), m_network);
    else
        setMsg = GblData::dbcGetRxMessages4Node(nodeName.toStdString(), m_network);
    for (auto txMsg : setMsg)
    {
        col = 0;

        item = new QStandardItem(GblData::m_publicIconMap[ITEM_ICON_Message],
            QString::fromStdString(txMsg->name));
        m_modelTable->setItem(row, col++, item);

        item = new QStandardItem(nodeName);
        m_modelTable->setItem(row, col++, item);

        QString sId = QString("0x") + QString::number(txMsg->id, 16).toUpper();
        item = new QStandardItem(sId);
        m_modelTable->setItem(row, col++, item);

        item = new QStandardItem(QStringLiteral("CAN Standard"));
        m_modelTable->setItem(row, col++, item);

        item = new QStandardItem(QString::number(txMsg->size, 10));
        m_modelTable->setItem(row, col++, item);

        item = new QStandardItem(QStringLiteral(""));
        m_modelTable->setItem(row, col++, item);

        for(it=mapAttr.begin(); it!=mapAttr.end(); ++it)
        {
            itMap = txMsg->attributeValues.find((*it).first);
            if ((itMap) != txMsg->attributeValues.end())
            {
                qTempStr = GblData::dbcGetStrAttributeValue(
                    &itMap->second,
                    Vector::DBC::AttributeDefinition::ObjectType::Message,
                    m_network
                    );
                item = new QStandardItem(qTempStr);
                m_modelTable->setItem(row, col++, item);
            }
            else
            {
                item = new QStandardItem(mapAttr[(*it).first]);
                m_modelTable->setItem(row, col++, item);
            }
        }

        row++;
    }
}

void DbWindow::updateTableViewNodesMappedSignals(QString nodeName, int type)
{
    int row = 0, colCount = 0;
    QString qTempStr;
    std::map<std::string, QString> mapAttr;
    std::map<std::string, QString>::iterator it;
    std::map<std::string, Vector::DBC::Attribute>::iterator itMap;

    prebuildTableView();

    GblData::dbcGetAttributesDefault(
        mapAttr,
        Vector::DBC::AttributeDefinition::ObjectType::Signal,
        m_network);

    colCount = 12 + mapAttr.size();
    m_modelTable->setColumnCount(colCount);

    insertSignalAttributesHeader2Model(mapAttr);

    std::set<Vector::DBC::Message *> setMsg;

    if (type == TYPE_TX_OBJECT)
        setMsg = GblData::dbcGetTxMessages4Node(nodeName.toStdString(), m_network);
    else
        setMsg = GblData::dbcGetRxMessages4Node(nodeName.toStdString(), m_network);

    for (auto xMsg : setMsg)
    {
        for (auto signalPair : xMsg->signal)
        {
            insertSignalAttributesValue2Model(row, mapAttr, xMsg, &signalPair.second);
            row++;
        }
    }
}

void DbWindow::updateTableViewNodesMappedSignalsSignal(QString nodeName, QString sigName, int type)
{
    int row = 0, colCount = 0;
    std::map<std::string, QString> mapAttr;
    Vector::DBC::Signal *pSignal;
    Vector::DBC::Message *pMessage;

    if (type == TYPE_TX_OBJECT)
        pSignal = GblData::dbcGetTxSignal4Node(nodeName.toStdString(), sigName.toStdString(), &pMessage, m_network);
    else
        pSignal = GblData::dbcGetRxSignal4Node(nodeName.toStdString(), sigName.toStdString(), &pMessage, m_network);

    if (pSignal == nullptr)
        return;

    prebuildTableView();

    GblData::dbcGetAttributesDefault(
        mapAttr,
        Vector::DBC::AttributeDefinition::ObjectType::Signal,
        m_network);

    colCount = 12 + mapAttr.size();
    m_modelTable->setColumnCount(colCount);

    insertSignalAttributesHeader2Model(mapAttr);
    insertSignalAttributesValue2Model(row, mapAttr, pMessage, pSignal);
}

// TABLE Network Messages ------------------------------------------------------------

void DbWindow::updateTableViewMessages()
{
    int row = 0, col = 0, colCount = 0;
    QString qTempStr;
    std::set<std::string> setAttrsName;
    std::map<std::string, QString> mapAttr; // store attribute default value
    std::set<std::string>::iterator itSet;
    std::map<std::string, Vector::DBC::Attribute>::iterator itMap;
    QStandardItem *item;

    prebuildTableView();

    GblData::dbcGetAttributeDefinitions(
        setAttrsName,
        Vector::DBC::AttributeDefinition::ObjectType::Message,
        m_network
        );

    for(itSet=setAttrsName.begin(); itSet!=setAttrsName.end(); ++itSet)
    {
#ifndef F_NO_DEBUG
        qDebug() << QString::fromStdString(*itSet) << endl;
#endif
        itMap = m_network->attributeDefaults.find((*itSet));

        if ((itMap) != m_network->attributeDefaults.end())
        {
            qTempStr = GblData::dbcGetStrAttributeValue(
                        &itMap->second,
                        Vector::DBC::AttributeDefinition::ObjectType::Message,
                        m_network);

            mapAttr.insert(
                std::map<std::string, QString>::value_type(
                    (*itSet), qTempStr));
        }
        else
        {
            mapAttr.insert(
                std::map<std::string, QString>::value_type((*itSet), ""));
        }
    }

    colCount = 5 + setAttrsName.size();

#ifndef F_NO_DEBUG
    qDebug() << "colCount = " << colCount << endl;
#endif

    m_modelTable->setColumnCount(colCount);

    col = 0;
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Name");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "ID");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "ID-Format");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "DLC");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Transmitter");
    for(auto attrName : setAttrsName)
    {
        m_modelTable->setHeaderData(col++, Qt::Horizontal,
            QString::fromStdString(attrName));
    }

    /* loop over messages */
    for (auto message : m_network->messages)
    {
        col = 0;

        item = new QStandardItem(GblData::m_publicIconMap[ITEM_ICON_Message],
            QString::fromStdString(message.second.name));
        m_modelTable->setItem(row, col++, item);

        QString sId = QString("0x") + QString::number(message.second.id, 16).toUpper();
        item = new QStandardItem(sId);
        m_modelTable->setItem(row, col++, item);

        item = new QStandardItem(QStringLiteral("CAN Standard"));
        m_modelTable->setItem(row, col++, item);

        item = new QStandardItem(QString::number(message.second.size, 10));
        m_modelTable->setItem(row, col++, item);

        if (message.second.transmitters.size() > 0)
        {
            item = new QStandardItem(QStringLiteral("-- Multiple Transmitters --"));
        }
        else
        {
            item = new QStandardItem(QString::fromStdString(message.second.transmitter));
        }
        m_modelTable->setItem(row, col++, item);

        for(itSet=setAttrsName.begin(); itSet!=setAttrsName.end(); ++itSet)
        {
            itMap = message.second.attributeValues.find((*itSet));
            if ((itMap) != message.second.attributeValues.end())
            {
                qTempStr = GblData::dbcGetStrAttributeValue(
                    &itMap->second,
                    Vector::DBC::AttributeDefinition::ObjectType::Message,
                    m_network
                    );
                item = new QStandardItem(qTempStr);
                m_modelTable->setItem(row, col++, item);
            }
            else
            {
                item = new QStandardItem(mapAttr[*itSet]);
                m_modelTable->setItem(row, col++, item);
            }
        }

        row++;
    }

    postbuildTableView();

    statusBar()->showMessage(tr("%1 Messages").arg(row));
}

void DbWindow::updateTableViewSignals()
{
    int row = 0, col = 0, colCount = 0;
    QString qTempStr;
    std::set<std::string> setAttrsName;
    std::map<std::string, QString> mapAttr; // store attribute default value
    std::set<std::string>::iterator itSet;
    std::map<std::string, Vector::DBC::Attribute>::iterator itMap;
    QStandardItem *item;

    prebuildTableView();

    GblData::dbcGetAttributeDefinitions(
        setAttrsName,
        Vector::DBC::AttributeDefinition::ObjectType::Signal,
        m_network
        );

    for(itSet=setAttrsName.begin(); itSet!=setAttrsName.end(); ++itSet)
    {
#ifndef F_NO_DEBUG
        qDebug() << QString::fromStdString(*itSet) << endl;
#endif
        itMap = m_network->attributeDefaults.find((*itSet));

        if ((itMap) != m_network->attributeDefaults.end())
        {
            qTempStr = GblData::dbcGetStrAttributeValue(
                        &itMap->second,
                        Vector::DBC::AttributeDefinition::ObjectType::Signal,
                        m_network);

            mapAttr.insert(
                std::map<std::string, QString>::value_type(
                    (*itSet), qTempStr));
        }
        else
        {
            mapAttr.insert(
                std::map<std::string, QString>::value_type((*itSet), ""));
        }
    }

    colCount = 5 + setAttrsName.size();

#ifndef F_NO_DEBUG
    qDebug() << "colCount = " << colCount << endl;
#endif

    m_modelTable->setColumnCount(colCount);

    col = 0;
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Name");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Length");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Byte Order");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Value Type");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Comment");
    for(auto attrName : setAttrsName)
    {
        m_modelTable->setHeaderData(col++, Qt::Horizontal,
            QString::fromStdString(attrName));
    }

    /* loop over signals */
    for (auto message : m_network->messages)
    for (auto signalPair : message.second.signal)
    {
        col = 0;

        item = new QStandardItem(GblData::m_publicIconMap[ITEM_ICON_Signal],
            QString::fromStdString(signalPair.second.name));
        m_modelTable->setItem(row, col++, item);

        item = new QStandardItem(QString::number(signalPair.second.bitSize, 10));
        m_modelTable->setItem(row, col++, item);

        if (signalPair.second.byteOrder == Vector::DBC::ByteOrder::BigEndian)
            qTempStr = QObject::tr("Motorola");
        else
            qTempStr = QObject::tr("Intel");
        item = new QStandardItem(qTempStr);
        m_modelTable->setItem(row, col++, item);


        if (signalPair.second.valueType == Vector::DBC::ValueType::Unsigned)
            qTempStr = QObject::tr("Unsigned");
        else
            qTempStr = QObject::tr("Signed");
        item = new QStandardItem(qTempStr);
        m_modelTable->setItem(row, col++, item);

        item = new QStandardItem(QString::fromStdString(signalPair.second.comment));
        m_modelTable->setItem(row, col++, item);

        for(itSet=setAttrsName.begin(); itSet!=setAttrsName.end(); ++itSet)
        {
            itMap = signalPair.second.attributeValues.find((*itSet));
            if ((itMap) != signalPair.second.attributeValues.end())
            {
                qTempStr = GblData::dbcGetStrAttributeValue(
                    &itMap->second,
                    Vector::DBC::AttributeDefinition::ObjectType::Signal,
                    m_network
                    );
                item = new QStandardItem(qTempStr);
                m_modelTable->setItem(row, col++, item);
            }
            else
            {
                item = new QStandardItem(mapAttr[*itSet]);
                m_modelTable->setItem(row, col++, item);
            }
        }

        row++;
    }

    postbuildTableView();

    statusBar()->showMessage(tr("%1 Signals").arg(row));

}

void DbWindow::buildTableView()
{
    updateTableViewNetworks();
}

bool DbWindow::loadFile(const QString &fileName)
{
    Vector::DBC::File file;
    std::string sName = fileName.toStdString();

    if ((fileName.isEmpty()) || (!QFile::exists(fileName)))
    {
#ifndef F_NO_DEBUG
        qDebug() << "Filename is empty or file not exist " << endl;
#endif
        return false;
    }

    setCurrentFile(fileName);

    if (file.load((*m_network), sName) != Vector::DBC::Status::Ok)
    {
#ifndef F_NO_DEBUG
        qDebug() << "Fail to open file " << m_curFile << endl;
#endif
        return false;
    }

    buildTreeView();
    buildTableView();

    return true;
}

QString DbWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QString DbWindow::userFriendlyCurrentFile()
{
    return strippedName(m_curFile);
}

void DbWindow::setCurrentFile(const QString &fileName)
{
    m_curFile = QFileInfo(fileName).canonicalFilePath();
    m_isUntitled = false;
    //document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

void DbWindow::popupDialog(const int itemType, const QModelIndex &index)
{
    QString s;

    switch(itemType)
    {
        case MARK_OBJECT_NETWORK:
        {
#ifndef F_NO_DEBUG
            qDebug() << "MARK_OBJECT_NETWORK selected\n";
#endif
            Dialog_Network *dialog = new Dialog_Network(this);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            s = QString("Network\"") + index.data().toString() + QString("\"");
            dialog->setWindowTitle(s);
            //dialog->setOperateItemValue(index.data().toString());
            dialog->setOperateItemIndex(index);
            dialog->updateUIContents();
            dialog->exec();
            break;
        }
    }
}

void DbWindow::on_m_tvMain_clicked(const QModelIndex &index)
{
    QString str;
    QStandardItem* currentItem = m_modelTree->itemFromIndex(index);
    QVariant var = currentItem->data(ROLE_DBC_MARK);

#ifndef F_NO_DEBUG
    qDebug() << "TREE ITEM" << var << " selected\n";
#endif
    switch(var.value<int>())
    {
        case MARK_CATEGORY_NETWORK:
            updateTableViewNetworks();
            break;
        case MARK_CATEGORY_ECU:
            updateTableViewECUs();
            break;
        case MARK_CATEGORY_NODE:
            updateTableViewNodes();
            break;
        case MARK_CATEGORY_MESSAGE:
            updateTableViewMessages();
            break;
        case MARK_CATEGORY_SIGNAL:
            updateTableViewSignals();
            break;
        case MARK_ITEM_CAT_NETWORK_TXMESSAGE:
            updateTableViewTxMessages();
            break;
        case MARK_ITEM_CAT_NETWORK_SIGNALS:
            updateTableViewNetworkSignals();
            break;
        case MARK_ITEM_CAT_NETWORK_NODE:
        case MARK_ITEM_CAT_NODES_NODE:
            updateTableViewNetworkNode(index.data().toString());
            break;
        case MARK_ITEM_CAT_ECU_ECU:
            updateTableViewECUsECU(index.data().toString());
            break;
        case MARK_ITEM_CAT_NODES_TXMESSAGES:
            updateTableViewNodesMessages(index.parent().data().toString(), TYPE_TX_OBJECT);
            break;
        case MARK_ITEM_CAT_NODES_RXMESSAGES:
            updateTableViewNodesMessages(index.parent().data().toString(), TYPE_RX_OBJECT);
            break;
        case MARK_ITEM_CAT_NODES_TXMAPSIGS:
            updateTableViewNodesMappedSignals(index.parent().data().toString(), TYPE_TX_OBJECT);
            break;
        case MARK_ITEM_CAT_NODES_RXMAPSIGS:
            updateTableViewNodesMappedSignals(index.parent().data().toString(), TYPE_RX_OBJECT);
            break;
        case MARK_ITEM_CAT_NODES_TXMAPSIGS_SIG:
            updateTableViewNodesMappedSignalsSignal(
                index.parent().parent().data().toString(),
                index.data().toString(),
                TYPE_TX_OBJECT);
        case MARK_ITEM_CAT_NODES_RXMAPSIGS_SIG:
            updateTableViewNodesMappedSignalsSignal(
                index.parent().parent().data().toString(),
                index.data().toString(),
                TYPE_RX_OBJECT);
            break;
    }
}

void DbWindow::on_m_tvMain_doubleClicked(const QModelIndex &index)
{
    QString str;
    QStandardItem* currentItem = m_modelTree->itemFromIndex(index);
    QVariant varObject = currentItem->data(ROLE_DBC_MARK);

    int objectType = varObject.value<int>();

#ifndef F_NO_DEBUG
    qDebug() << "doubleClicked on treeView\n";
#endif
    popupDialog(objectType, index);
}
