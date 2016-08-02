#include "dialog_network.h"
#include "ui_dialog_network.h"
#include "dbwindow.h"
#include "gbldata.h"

Dialog_Network::Dialog_Network(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_Network)
{
    ui->setupUi(this);

    DbWindow *pParent = (DbWindow *)parentWidget();
    m_curFile = pParent->currentFile();
    m_network = pParent->currentNetwork();

    m_modelTree     = new QStandardItemModel(ui->m_tvAttr);
    m_modelTree->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Attribute") << QStringLiteral("Value"));

    m_modelTable    = new QStandardItemModel(ui->m_tblNode);

    updateTableViewNodes();
    buildTreeViewAttributes();
}

Dialog_Network::~Dialog_Network()
{
    delete ui;
#if 0
    delete m_modelTree;
    delete m_modelTable;
#endif
}

void Dialog_Network::updateUIContents()
{
    ui->m_leName->setText(m_operateItemIndex.data().toString());
    //ui->m_leName->setText(m_operateItemValue);
}

void Dialog_Network::prebuildTableView(QTableView *tv)
{
    m_modelTable->removeRows(0, m_modelTable->rowCount());
    tv->reset();
}

void Dialog_Network::postbuildTableView(QTableView *tv)
{
    tv->setModel(m_modelTable);
    tv->resizeColumnsToContents();
    tv->setColumnWidth(0,120);
    tv->setColumnWidth(1,120);
}

void Dialog_Network::updateTableViewNodes()
{
    int row = 0, col = 0, colCount = 0;
    QStandardItem *item;

    prebuildTableView(ui->m_tblNode);

#ifndef F_NO_DEBUG
    qDebug() << "colCount = " << colCount << endl;
#endif

    colCount = 3;
    m_modelTable->setColumnCount(colCount);

    col = 0;
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Name");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "Address");
    m_modelTable->setHeaderData(col++, Qt::Horizontal, "");

    for(auto node : m_network->nodes)
    {
        col = 0;

        item = new QStandardItem(GblData::m_publicIconMap[ITEM_ICON_Node],
            QString::fromStdString(node.second.name));
        m_modelTable->setItem(row, col++, item);

        item = new QStandardItem(QStringLiteral("0x00"));
        m_modelTable->setItem(row, col++, item);

        row++;
    }

    postbuildTableView(ui->m_tblNode);
}

void Dialog_Network::buildTreeViewAttributes()
{
    QStandardItem *item;
    QString strAttrValue;
    QMap<QString, Vector::DBC::Attribute *> mapNA;

    GblData::dbcGetNetworkAttributes(mapNA, m_network);

    QStandardItem *itemCategory = new QStandardItem(
        GblData::m_publicIconMap[ITEM_ICON_Network],
        QStringLiteral("No category assigned"));
    m_modelTree->appendRow(itemCategory);

    QMap<QString, Vector::DBC::Attribute *>::const_iterator pairNA;
    for( pairNA=mapNA.constBegin(); pairNA!=mapNA.constEnd(); ++pairNA)
    {
        strAttrValue = GblData::dbcGetStrAttributeValue(
                pairNA.value(),
                Vector::DBC::AttributeDefinition::ObjectType::Network,
                m_network);

        QStandardItem *itemAttr = new QStandardItem(
            GblData::m_publicIconMap[ITEM_ICON_Network],
            pairNA.key());
        itemCategory->appendRow(itemAttr);
        itemCategory->setChild(itemAttr->index().row(),1,new QStandardItem(strAttrValue));
    }

    ui->m_tvAttr->setModel(m_modelTree);
    ui->m_tvAttr->expandAll();
}

void Dialog_Network::on_m_btnOK_clicked()
{
    close();
}

void Dialog_Network::on_m_btnCancel_clicked()
{
    close();
}
