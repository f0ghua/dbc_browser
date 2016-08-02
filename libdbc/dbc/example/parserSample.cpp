#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <sstream>

#include "DBC.h"
#include "AttributeValueType.h"
#include "AttributeDefinition.h"

using namespace std;

#define ATTR_DBNAME "DBName"

string getDBName(Vector::DBC::Network network)
{
	string dbName = "";

	std::map<std::string, Vector::DBC::Attribute>::iterator iterAttrVal;
	for (iterAttrVal = network.attributeValues.begin();
		iterAttrVal != network.attributeValues.end();
		iterAttrVal++)
	{
		//cout << "DB Attribute = " << (*iterAttrVal).second.name << endl;
		if ((*iterAttrVal).second.name == ATTR_DBNAME)
		{
			//cout << "found DBName: " << (*iterAttrVal).second.stringValue << endl;
			dbName = (*iterAttrVal).second.stringValue;
		}
	}

	return dbName;
}

vector<string> getMessageReceivers(Vector::DBC::Network network, unsigned int id)
{
	vector<string> receivers;

	/* loop over messages */
	for (auto message : network.messages)
	{
		// cout << "Message id(" << message.second.id << "), name(" << message.second.name << ")" << endl;

		if (message.second.id == id)
		{
			for (auto subSignal : message.second.signal)
			{
				for (auto receiver : subSignal.second.receivers)
				{
					size_t i;

					cout << "Signal(" << subSignal.second.name << ").receiver = " << receiver << endl;
					for (i = 0; i < receivers.size(); i++)
					{
						if (receiver == receivers[i])
							break;
					}
					if (i == receivers.size())
					{
						receivers.push_back(receiver);
					}
				}
			}
		}
	}

	//cout << "Total TxMessages are " << count << endl;

	return receivers;
}

void dumpMessageSignals(Vector::DBC::Network network)
{
    int count = 0;
	/* loop over messages */
	for (auto message : network.messages) {
		std::cout << "Message " << message.second.name << std::endl;

		/* loop over signals of this messages */
		for (auto signal : message.second.signal) {
            char byteOrder = '0';
            if (signal.second.byteOrder == Vector::DBC::ByteOrder::BigEndian)
                byteOrder = '1';
            std::cout << "  Signal " << signal.second.name << ", byteOrder:" << byteOrder << std::endl;
            count++;
            for (auto valDescription : signal.second.valueDescriptions)
                cout << "    Value Description " << valDescription.first << ":" << valDescription.second << endl;
		}
	}
    cout << "Total Signals are " << count << endl;
}

void dumpMessages(Vector::DBC::Network network)
{
	int count = 0;

	/* loop over messages */
	for (auto message : network.messages)
	{
		cout << "Message id(" << message.second.id << "), name(" <<
			message.second.name << "), size(" << message.second.size <<
			"), transmitter(" << message.second.transmitter << ")" <<
			endl;

		std::cout << "Message(" << message.second.id << ") attribute: ";

		/* loop over attributes */
		std::map<std::string, Vector::DBC::Attribute>::iterator iter;
		for (iter = message.second.attributeValues.begin();
			iter != message.second.attributeValues.end();
			iter++)
		{
			std::cout << "[" << (*iter).second.name << "," << (*iter).second.enumValue << "], ";
		}
		std::cout << endl;

		/* loop over transmitters */

		std::cout << "transmitters: ";
		std::set<std::string>::iterator iterTxer;

		for (iterTxer = message.second.transmitters.begin();
			iterTxer != message.second.transmitters.end();
			iterTxer++)
		{
			std::cout << "[" << (*iterTxer) << "], ";
		}
		std::cout << endl;

		count++;
	}

	cout << "Total Messages are " << count << endl;
}

void dumpTxMessages(Vector::DBC::Network network)
{
	int count = 0;

	/* loop over messages */
	for (auto message : network.messages)
	{
		if (message.second.transmitter == "")
		{
			for (auto transmitter : message.second.transmitters)
			{
				cout << "TxMessage id(" << message.second.id << "), name(" <<
				message.second.name << "), size(" << message.second.size <<
				"), transmitter(" << transmitter << ")" <<
				endl;

				count++;
			}
		}
		else
		{
			cout << "TxMessage id(" << message.second.id << "), name(" <<
			message.second.name << "), size(" << message.second.size <<
			"), transmitter(" << message.second.transmitter << ")" <<
			endl;

			count++;
		}
	}

	cout << "Total TxMessages are " << count << endl;
}

void dumpVersion(Vector::DBC::Network network)
{
	cout << "Version: " << network.version << endl;
}

void dumpNetSymbols(Vector::DBC::Network network)
{
	int count = 0;
	std::list<std::string> &newSs = network.newSymbols;

	/* loop over messages */
	std::list<std::string>::iterator it;

	for (it = newSs.begin(); it != newSs.end(); it++)
	{
		std::cout << "NewSymbol " << *it << std::endl;
		count++;
	}

	cout << "Total NewSymbols are " << count << endl;
}

std::vector<std::string> *getAttributeDefinition_Enum(Vector::DBC::AttributeDefinition::ObjectType objType, std::string sName, Vector::DBC::Network *network)
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

void dumpNodes(Vector::DBC::Network network)
{
	int count = 0;
	std::map<std::string, Vector::DBC::Node> &nodes = network.nodes;

	/* loop over messages */
	std::map<std::string, Vector::DBC::Node>::iterator it;

	for (it = nodes.begin(); it != nodes.end(); it++)
	{
		std::cout << "Nodes " << (*it).second.name << std::endl;
		count++;

		cout << "    Network attributeValues list:" << endl;
		int countAttr = 0;
		for (auto attribute: (*it).second.attributeValues)
		{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
			switch(attribute.second.valueType)
			{
				case Vector::DBC::AttributeValueType::Int:
					std::cout << "    Attr Name: " << attribute.second.name << ", Value: " << attribute.second.integerValue << std::endl;
					break;
				case Vector::DBC::AttributeValueType::Hex:
					std::cout << "    Attr Name: " << attribute.second.name << ", Value: " << attribute.second.hexValue << std::endl;
					break;
				case Vector::DBC::AttributeValueType::Float:
					std::cout << "    Attr Name: " << attribute.second.name << ", Value: " << attribute.second.floatValue << std::endl;
					break;
				case Vector::DBC::AttributeValueType::String:
					std::cout << "    Attr Name: " << attribute.second.name << ", Value: " << attribute.second.stringValue << std::endl;
					break;
				case Vector::DBC::AttributeValueType::Enum:
				{
					std::vector<std::string> *enumStr;

					enumStr = getAttributeDefinition_Enum(Vector::DBC::AttributeDefinition::ObjectType::Node,
						attribute.second.name,
						&network
						);
					if (enumStr != nullptr)
					{
						std::cout << "    Attr Name: " << attribute.second.name << ", Value: " << (*enumStr)[attribute.second.enumValue] << std::endl;
					}

					break;
				}
			}
#pragma GCC diagnostic pop
			countAttr++;
		}

		cout << "    Total Network Attribute are " << countAttr << endl;
	}

	cout << "Total Nodes are " << count << endl;
}

void dumpNetworkAttributes(Vector::DBC::Network network)
{
    int count = 0;

    cout << "Network attributeDefinitions list:" << endl;
	for (auto attribute: network.attributeDefinitions)
	{
		if (attribute.second.objectType != Vector::DBC::AttributeDefinition::ObjectType::Network)
			continue;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
		switch(attribute.second.valueType)
		{
			case Vector::DBC::AttributeValueType::Int:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.name << std::endl;
				break;
			case Vector::DBC::AttributeValueType::Hex:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.name << std::endl;
				break;
			case Vector::DBC::AttributeValueType::Float:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.name << std::endl;
				break;
			case Vector::DBC::AttributeValueType::String:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.name << std::endl;
				break;
		}
#pragma GCC diagnostic pop
		count++;
	}
	cout << "Total Network attributeDefinitions are " << count << endl;

    cout << "Network attributeDefaults list:" << endl;
    count = 0;
	for (auto attribute: network.attributeDefaults)
	{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
		switch(attribute.second.valueType)
		{
			case Vector::DBC::AttributeValueType::Int:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.integerValue << std::endl;
				break;
			case Vector::DBC::AttributeValueType::Hex:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.hexValue << std::endl;
				break;
			case Vector::DBC::AttributeValueType::Float:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.floatValue << std::endl;
				break;
			case Vector::DBC::AttributeValueType::String:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.stringValue << std::endl;
				break;
		}
#pragma GCC diagnostic pop
		count++;
	}
	cout << "Total Network attributeDefaults are " << count << endl;

	cout << "Network attributeValues list:" << endl;
	count = 0;
	for (auto attribute: network.attributeValues)
	{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"
		switch(attribute.second.valueType)
		{
			case Vector::DBC::AttributeValueType::Int:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.integerValue << std::endl;
				break;
			case Vector::DBC::AttributeValueType::Hex:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.hexValue << std::endl;
				break;
			case Vector::DBC::AttributeValueType::Float:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.floatValue << std::endl;
				break;
			case Vector::DBC::AttributeValueType::String:
				std::cout << "Attr Name: " << attribute.second.name << ", Value: " << attribute.second.stringValue << std::endl;
				break;
		}
#pragma GCC diagnostic pop
		count++;
	}

	cout << "Total Network Attribute are " << count << endl;
}

void dbcGetObjectAttributesName(std::set<std::string> &setAttrsName,
                    Vector::DBC::AttributeDefinition::ObjectType objectType,
                    Vector::DBC::Network *network)
{
    int count = 0;
    std::string strAttrName;
    //std::map<std::string, Vector::DBC::Attribute>::iterator it;

    for (auto attributeDefinition: network->attributeDefinitions)
    {
        if (attributeDefinition.second.objectType != objectType)
            continue;

        strAttrName = attributeDefinition.second.name;
        setAttrsName.insert(strAttrName);

/*
        it = network->attributeDefaults.find(strAttrName);
        if ((it) != network->attributeDefaults.end())
        {
        	setAttrsName.insert(strAttrName);
        }
*/
    }

    return;
}

template <class Type>
std::string num2string(Type v)
{
	std::string s;
	stringstream ss;

	ss << v;

	return ss.str();
}

std::vector<std::string> *getAttributeDefinitionEnum(Vector::DBC::AttributeDefinition::ObjectType objType, std::string sName, Vector::DBC::Network *network)
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

std::string dbcGetStrAttributeValue(Vector::DBC::Attribute *attr,Vector::DBC::Network *network)
{
    std::string strAttrValue = "";

    if (attr->valueType == Vector::DBC::AttributeValueType::String)
    {
        strAttrValue = attr->stringValue;
    }
    else
    if (attr->valueType == Vector::DBC::AttributeValueType::Int)
    {
        strAttrValue = num2string(attr->integerValue);
    }
    else
    if (attr->valueType == Vector::DBC::AttributeValueType::Hex)
    {
        strAttrValue = num2string(attr->hexValue);
    }
    else
    if (attr->valueType == Vector::DBC::AttributeValueType::Float)
    {
        strAttrValue = num2string(attr->floatValue);
    }
    else
    if (attr->valueType == Vector::DBC::AttributeValueType::Enum)
    {
    	std::vector<std::string> *enumStr;

    	enumStr = getAttributeDefinitionEnum(
    		Vector::DBC::AttributeDefinition::ObjectType::Message,
    		attr->name,
    		network
    		);
    	if (enumStr != nullptr)
    	{
    		strAttrValue = ((*enumStr)[attr->enumValue]);
    	}
    }

    return strAttrValue;
}

void dumpMessageAttributes(Vector::DBC::Network *network)
{
	std::set<std::string> setAttrsName;
	std::map<std::string, std::string> mapAttr;
	std::set<std::string>::iterator it;
	std::map<std::string, Vector::DBC::Attribute>::iterator itMap;

	dbcGetObjectAttributesName(
		setAttrsName,
		Vector::DBC::AttributeDefinition::ObjectType::Message,
		network
		);


    for( it=setAttrsName.begin(); it!=setAttrsName.end(); ++it)
    {
#ifndef F_NO_DEBUG
    	cout << (*it) << endl;
#endif


    	itMap = network->attributeDefaults.find((*it));

    	if ((itMap) != network->attributeDefaults.end())
    	{
    		mapAttr.insert(
    			map<std::string, std::string>::value_type(
    				(*it), dbcGetStrAttributeValue(&itMap->second, network)));
    	}
    	else
    	{
    		std::string nullStr("");
    		mapAttr.insert(
    			map<std::string, std::string>::value_type((*it), ""));
    	}

    }

    cout << "Message attributes count is " << setAttrsName.size() << endl;

    for (auto attrPair : mapAttr)
    {
    	cout << attrPair.first << " : " << attrPair.second << endl;
    }

    /* loop over messages */
    for (auto message : network->messages)
    {
		cout << "Message id(" << message.second.id << "), name(" <<
			message.second.name << "), size(" << message.second.size <<
			"), transmitter(" << message.second.transmitter << ")" <<
			endl;

    	for( it=setAttrsName.begin(); it!=setAttrsName.end(); ++it)
    	{

    		itMap = message.second.attributeValues.find((*it));

    		if ((itMap) != message.second.attributeValues.end())
    		{
    			cout << "Y\t" << (*it) << " : " << dbcGetStrAttributeValue(&itMap->second, network) << endl;
    		}
    		else
    		{
    			cout << "N\t" << (*it) << " : " << mapAttr[*it] << endl;
    		}
    	}

    	break;
    }

#if 0
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
#endif

}


std::set<std::string> dbcGetTxMessages4Node(
    const std::string nodeName,
    Vector::DBC::Network *network
)
{
	std::set<std::string> setTxMsg;

    for (auto message : network->messages)
    {
    	if (message.second.transmitter.empty())
    	{
    		if (message.second.transmitters.find(nodeName) != message.second.transmitters.end())
    			setTxMsg.insert(message.second.name);
    	}
    	else
    	{
    		if (message.second.transmitter == nodeName)
    		{
    			setTxMsg.insert(message.second.name);
    		}
    	}
    }

    return setTxMsg;
}

std::set<Vector::DBC::Message *> dbcGetRxMessages4Node(
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

int main(int argc, char *argv[])
{
	Vector::DBC::Network network;

	if (argc != 2) {
		std::cout << "List_Messages_Signals <database.dbc>" << std::endl;
		return -1;
	}

	/* load database file */
	Vector::DBC::File file;
	if (file.load(network, argv[1]) != Vector::DBC::Status::Ok) {
		return EXIT_FAILURE;
	}

	//dumpVersion(network);
	//dumpNetSymbols(network);
	//dumpMessages(network);
	//dumpNodes(network);
	//dumpMessages(network);

	{
		string dbName;
		vector<string> receivers;

		dbName = getDBName(network);
		cout << "DBName = " << dbName << endl;

		//dumpTxMessages(network);
		/*
		receivers = getMessageReceivers(network,0x312);
		for (size_t i = 0; i < receivers.size(); i++)
		{
			cout << "Message(0x312) has receiver " << receivers[i] << endl;
		}
        dumpMessageSignals(network);
		*/
        //dumpNetworkAttributes(network);
        //dumpNodes(network);
        //dumpMessages(network);
        //dumpMessageAttributes(&network);

        std::set<Vector::DBC::Message *> setRxMsg;
        setRxMsg = dbcGetRxMessages4Node("AHL_AFL_PB", &network);
        for (auto s : setRxMsg)
        {
        	cout << "Address = " << s << endl;
        	cout << "Rx Message: " << s->name << endl;
        }

	}

	return EXIT_SUCCESS;
}
