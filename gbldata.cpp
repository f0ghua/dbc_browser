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

/**
 * [GblData::dbcGetAttributesDefault get all attributes of the object and set with default value]
 *
 * @param mapAttr      [return name/value pair of attributes]
 * @param objectType   [description]
 * @param network      [description]
 */

void GblData::dbcGetAttributesDefault(std::map<std::string, QString> &mapAttr,
                    Vector::DBC::AttributeDefinition::ObjectType objectType,
                    Vector::DBC::Network *network)
{
    std::string strAttrName;
    QString qTempStr;
    std::map<std::string, Vector::DBC::Attribute>::iterator itMap;

    for (auto attributeDefinition: network->attributeDefinitions)
    {
        if (attributeDefinition.second.objectType != objectType)
            continue;

        strAttrName = attributeDefinition.second.name;
        mapAttr.insert(
            std::map<std::string, QString>::value_type(strAttrName, ""));
    }

    for(auto it : mapAttr)
    {
        itMap = network->attributeDefaults.find(it.first);
        if ((itMap) != network->attributeDefaults.end())
        {
            qTempStr = GblData::dbcGetStrAttributeValue(
                        &itMap->second,
                        Vector::DBC::AttributeDefinition::ObjectType::Signal,
                        network);

            mapAttr[it.first] = qTempStr;
        }

    }
}

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

std::set<Vector::DBC::Message *> GblData::dbcGetTxMessages4Node(
    const std::string nodeName,
    Vector::DBC::Network *network
)
{
    std::set<Vector::DBC::Message *> setTxMsg;
    std::map<unsigned int, Vector::DBC::Message>::iterator itMsg;

    for (itMsg=network->messages.begin(); itMsg!=network->messages.end(); itMsg++)
    {
        if ((*itMsg).second.transmitter.empty())
        {
            if ((*itMsg).second.transmitters.find(nodeName) != (*itMsg).second.transmitters.end())
                setTxMsg.insert(&(*itMsg).second);
        }
        else
        {
            if ((*itMsg).second.transmitter == nodeName)
            {
                setTxMsg.insert(&(*itMsg).second);
            }
        }
    }

    return setTxMsg;
}

std::set<Vector::DBC::Message *> GblData::dbcGetRxMessages4Node(
    const std::string nodeName,
    Vector::DBC::Network *network
)
{
    std::set<Vector::DBC::Message *> setRxMsg;
    std::map<unsigned int, Vector::DBC::Message>::iterator itMsg;

    for (itMsg=network->messages.begin(); itMsg!=network->messages.end(); itMsg++)
    for (auto signalPair : (*itMsg).second.signal)
    {
#ifdef F_DEBUG
        cout << "Signal:" << signalPair.second.name << ", nodeName: " << nodeName << endl;
        for (auto r : signalPair.second.receivers)
        {
            cout << "    " << r << endl;
        }
#endif
        if (signalPair.second.receivers.find(nodeName) != signalPair.second.receivers.end())
            setRxMsg.insert(&(*itMsg).second);
    }

    return setRxMsg;
}

std::string GblData::dbcGetMessageSignalGroup(
    const std::string sigName,
    Vector::DBC::Message *message
)
{
    std::set<std::string>::iterator itSg;

    for (auto sgPair : message->signalGroups)
    {
        itSg = sgPair.second.signal.find(sigName);
        if (itSg != sgPair.second.signal.end())
            return (*itSg);
    }

    return std::string();
}

Vector::DBC::Signal *GblData::dbcGetTxSignal4Node(
    const std::string nodeName,
    const std::string sigName,
    Vector::DBC::Message **message,
    Vector::DBC::Network *network
    )
{
    std::map<std::string, Vector::DBC::Signal>::iterator itSignal;
    std::set<Vector::DBC::Message *> setMsg;
    std::set<Vector::DBC::Message *>::iterator itSet;

    setMsg = dbcGetTxMessages4Node(nodeName, network);
    for (itSet = setMsg.begin(); itSet != setMsg.end(); itSet++)
    {
        for (itSignal = (*itSet)->signal.begin();
            itSignal != (*itSet)->signal.end();
            itSignal++)
        {
            if((*itSignal).second.name == sigName)
            {
                *message = ((*itSet));
                return &((*itSignal).second);
            }
        }
    }

    return nullptr;
}

Vector::DBC::Signal *GblData::dbcGetRxSignal4Node(
    const std::string nodeName,
    const std::string sigName,
    Vector::DBC::Message **message,
    Vector::DBC::Network *network
    )
{
    std::map<std::string, Vector::DBC::Signal>::iterator itSignal;
    std::set<Vector::DBC::Message *> setMsg;
    std::set<Vector::DBC::Message *>::iterator itSet;

    setMsg = dbcGetRxMessages4Node(nodeName, network);
    for (itSet = setMsg.begin(); itSet != setMsg.end(); itSet++)
    {
        for (itSignal = (*itSet)->signal.begin();
            itSignal != (*itSet)->signal.end();
            itSignal++)
        {
            if((*itSignal).second.name == sigName)
            {
                *message = ((*itSet));
                return &((*itSignal).second);
            }
        }
    }

    return nullptr;
}
