#ifndef GBLDATA_H
#define GBLDATA_H

#include <QString>
#include <QMap>
#include <QIcon>

#include "DBC.h"

#define DBC_ATTR_DBNAME "DBName"

#define ITEM_ICON_Network   QStringLiteral("itemNetwork")
#define ITEM_ICON_ECU       QStringLiteral("itemECU")
#define ITEM_ICON_Node      QStringLiteral("itemNode")
#define ITEM_ICON_ENV       QStringLiteral("itemENV")
#define ITEM_ICON_Message   QStringLiteral("itemMessage")
#define ITEM_ICON_Signal    QStringLiteral("itemSignal")

class GblData
{
public:
    static QMap<QString, QIcon> m_publicIconMap;

    static void initVariables();
    static QString dbcGetDBName(const QString &fileName, Vector::DBC::Network *network);
    static int dbcGetNetworkAttributes(QMap<QString, Vector::DBC::Attribute *> &mapNA, Vector::DBC::Network *network);
    static QString dbcGetStrAttributeValue(Vector::DBC::Attribute *attr,Vector::DBC::AttributeDefinition::ObjectType objectType,Vector::DBC::Network *network);
    static std::vector<std::string> *dbcGetAttributeDefinitionEnum(Vector::DBC::AttributeDefinition::ObjectType objType, std::string sName, Vector::DBC::Network *network);
    static void dbcGetAttributeDefinitions(std::set<std::string> &setAttrsName, Vector::DBC::AttributeDefinition::ObjectType objectType, Vector::DBC::Network *network);

    GblData();
};

#endif // GBLDATA_H