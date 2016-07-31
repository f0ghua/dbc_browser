#include <QFileInfo>

#include "gbldata.h"

QMap<QString, QIcon> GblData::m_publicIconMap;

GblData::GblData()
{

}

void GblData::initVariables()
{
    // init global class
    GblData::m_publicIconMap.insert(
        ITEM_ICON_Network, QIcon(QStringLiteral(":network"))
            );
    GblData::m_publicIconMap.insert(
        ITEM_ICON_ECU, QIcon(QStringLiteral(":ecu"))
            );
    GblData::m_publicIconMap.insert(
        ITEM_ICON_ENV, QIcon(QStringLiteral(":env"))
            );
    GblData::m_publicIconMap.insert(
        ITEM_ICON_Node, QIcon(QStringLiteral(":node"))
            );
    GblData::m_publicIconMap.insert(
        ITEM_ICON_Message, QIcon(QStringLiteral(":message"))
            );
    GblData::m_publicIconMap.insert(
        ITEM_ICON_Signal, QIcon(QStringLiteral(":signal"))
            );
}

QString GblData::dbcGetDBName(const QString &fileName, Vector::DBC::Network *network)
{
    std::string dbName = "";

    std::map<std::string, Vector::DBC::Attribute>::iterator iterAttrVal;
    for (iterAttrVal = network->attributeValues.begin();
        iterAttrVal != network->attributeValues.end();
        iterAttrVal++)
    {
        //cout << "DB Attribute = " << (*iterAttrVal).second.name << endl;
        if ((*iterAttrVal).second.name == DBC_ATTR_DBNAME)
        {
            //cout << "found DBName: " << (*iterAttrVal).second.stringValue << endl;
            dbName = (*iterAttrVal).second.stringValue;
        }
    }

    if (dbName == "")
    {
        QFileInfo fileinfo = QFileInfo(fileName);
        return fileinfo.fileName();
    }
    else
    {
        return QString::fromStdString(dbName);
    }
}

int GblData::dbcGetNetworkAttributes(QMap<QString, Vector::DBC::Attribute *> &mapNA, Vector::DBC::Network *network)
{
    int count = 0;
    QString strAttrName;

    for (auto attributeDefinition: network->attributeDefinitions)
    {
        if (attributeDefinition.second.objectType != Vector::DBC::AttributeDefinition::ObjectType::Network)
            continue;

        if (attributeDefinition.second.name == DBC_ATTR_DBNAME)
            continue;

        strAttrName = QString::fromStdString(attributeDefinition.second.name);

        std::map<std::string, Vector::DBC::Attribute>::iterator itAttrDefault;
        for (itAttrDefault = network->attributeDefaults.begin();
            itAttrDefault != network->attributeDefaults.end();
            itAttrDefault++
            )
        {
            if ((*itAttrDefault).second.name == attributeDefinition.second.name)
            {
                //cout << attributeDefinition.second.name << ", addr = " << &(network->attributeDefaults[attributeDefault.second.name]) << endl;
                mapNA.insert(strAttrName, &(*itAttrDefault).second);
            }
        }

        std::map<std::string, Vector::DBC::Attribute>::iterator itAttrValue;
        for (itAttrValue = network->attributeValues.begin();
            itAttrValue != network->attributeValues.end();
            itAttrValue++
            )
        {
            if ((*itAttrValue).second.name == attributeDefinition.second.name)
            {
                //cout << attributeValue.second.name << ", addr = " << &(network->attributeValues[attributeValue.second.name]) << endl;
                mapNA.insert(strAttrName, &(*itAttrValue).second);
            }
        }

        count++;
    }

    return count;
}


void GblData::dbcGetAttributeDefinitions(std::set<std::string> &setAttrsName,
                    Vector::DBC::AttributeDefinition::ObjectType objectType,
                    Vector::DBC::Network *network)
{
    std::string strAttrName;

    for (auto attributeDefinition: network->attributeDefinitions)
    {
        if (attributeDefinition.second.objectType != objectType)
            continue;

        strAttrName = attributeDefinition.second.name;
        setAttrsName.insert(strAttrName);
    }

    return;
}

#if 0
void GblData::dbcGetObjectAttributes(QMap<QString, Vector::DBC::Attribute *> &mapNA,
                    Vector::DBC::AttributeDefinition::ObjectType objectType,
                    Vector::DBC::Network *network)
{
    int count = 0;
    QString strAttrName;

    for (auto attributeDefinition: network->attributeDefinitions)
    {
        if (attributeDefinition.second.objectType != objectType)
            continue;

        strAttrName = QString::fromStdString(attributeDefinition.second.name);

        std::map<std::string, Vector::DBC::Attribute>::iterator itAttrDefault;
        for (itAttrDefault = network->attributeDefaults.begin();
            itAttrDefault != network->attributeDefaults.end();
            itAttrDefault++
            )
        {
            if ((*itAttrDefault).second.name == attributeDefinition.second.name)
            {
                //cout << attributeDefinition.second.name << ", addr = " << &(network->attributeDefaults[attributeDefault.second.name]) << endl;
                mapNA.insert(strAttrName, &(*itAttrDefault).second);
            }
        }
    }

    if (objectType == Vector::DBC::AttributeDefinition::ObjectType::Message)
    {
        /* loop over messages */
        for (auto message : network->messages)
        {
            /* loop over attributes */
            std::map<std::string, Vector::DBC::Attribute>::iterator iter;
            for (iter = message.second.attributeValues.begin();
                iter != message.second.attributeValues.end();
                iter++)
            {
                mapNA.insert(strAttrName, &(*iter).second);
            }
        }
    }

    return;
}
#endif

QString GblData::dbcGetStrAttributeValue(
    Vector::DBC::Attribute *attr,
    Vector::DBC::AttributeDefinition::ObjectType objectType,
    Vector::DBC::Network *network)
{
    QString strAttrValue = "";

    if (attr->valueType == Vector::DBC::AttributeValueType::String)
    {
        strAttrValue = QString::fromStdString(attr->stringValue);
    }
    else
    if (attr->valueType == Vector::DBC::AttributeValueType::Int)
    {
        strAttrValue = QString::number(attr->integerValue, 10);
    }
    else
    if (attr->valueType == Vector::DBC::AttributeValueType::Hex)
    {
        strAttrValue = QString("0x") + QString::number(attr->hexValue,16).toUpper();
    }
    else
    if (attr->valueType == Vector::DBC::AttributeValueType::Float)
    {
        strAttrValue = QString::number(attr->floatValue, 10, 2);
    }
    else
    if (attr->valueType == Vector::DBC::AttributeValueType::Enum)
    {
        std::vector<std::string> *enumStr;

        enumStr = dbcGetAttributeDefinitionEnum(objectType, attr->name, network);
        if (enumStr != nullptr)
        {
            strAttrValue = QString::fromStdString((*enumStr)[attr->enumValue]);
        }
    }

    return strAttrValue;
}

std::vector<std::string> *GblData::dbcGetAttributeDefinitionEnum(
    Vector::DBC::AttributeDefinition::ObjectType objType,
    std::string sName,
    Vector::DBC::Network *network
)
{
    std::map<std::string, Vector::DBC::AttributeDefinition>::iterator iterAttrPair;

    for (
        iterAttrPair = network->attributeDefinitions.begin();
        iterAttrPair != network->attributeDefinitions.end();
        iterAttrPair++
        )
    {
        if ((*iterAttrPair).second.objectType != objType)
            continue;

        if (
            (sName == (*iterAttrPair).second.name) &&
            ((*iterAttrPair).second.valueType == Vector::DBC::AttributeValueType::Enum)
            )
        {
            return &((*iterAttrPair).second.enumValues);
        }
    }

    return nullptr;
}
