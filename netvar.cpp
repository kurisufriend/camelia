#include "netvar.h"
#include "globals.h"
/*
DWORD netvars::get(RecvTable* baseTable, std::string tableName, std::string name)
{
	//std::cout << baseTable->m_pNetTableName << std::endl;
	for (int i = 0; i < baseTable->m_nProps; i++)
	{
		RecvProp prop = baseTable->m_pProps[i];
		//std::string curPropName = prop.m_pVarName;
		if (!_stricmp(name.c_str(), prop.m_pVarName))
			return prop.m_Offset;
		if (!prop.m_pDataTable)
			return NULL;
		DWORD offset = get(prop.m_pDataTable, tableName, name);
		if (offset)
			return NULL;
		return offset + prop.m_Offset;
	}
	return NULL;
}

DWORD netvars::getNetvar(std::string tableName, std::string name)
{
	ClientClass* clientClass = interfaces::pacClient->GetAllClasses();
	if (!clientClass)
		return NULL;
	ClientClass* curClass = clientClass;
	for (; curClass; curClass = curClass->m_pNext)
	{
		//std::string curName = curClass->m_pRecvTable->m_pNetTableName; // fuck this fucking shit fucking shit fuck my life i spent 40 fucking minutes trying to find this fucking problem fuck fuck fuck // UPDATE IM STILL TRYING TO FIX IT WHAT THE FUCK? THIS IS REALLY SIMPLE
		if (!_stricmp(tableName.c_str(), curClass->m_pRecvTable->m_pNetTableName))
		{
			//std::cout << curClass->m_pRecvTable->m_pNetTableName << " " << curName << " " << "FUCKSHIT" << std::endl;
			//std::cout << curClass->m_pRecvTable;
			return get(curClass->m_pRecvTable, tableName, name);
		}
	}
	return NULL;
}*/
DWORD netvar::get(RecvTable* table, std::string tableName, std::string netvarName)
{
	for (int i = 0; i < table->m_nProps; i++)
	{
		RecvProp prop = table->m_pProps[i];

		if (!_stricmp(prop.m_pVarName, netvarName.c_str()))
		{
			return prop.m_Offset;
		}

		if (prop.m_pDataTable)
		{
			DWORD offset = get(prop.m_pDataTable, tableName, netvarName);

			if (offset)
			{
				return offset + prop.m_Offset;
			}
		}
	}
	return 0;
}

DWORD netvar::getNetvar(std::string tableName, std::string netvarName)
{
	for (auto currNode = interfaces::pacClient->GetAllClasses(); currNode; currNode = currNode->m_pNext)
	{
		if (!_stricmp(tableName.c_str(), currNode->m_pRecvTable->m_pNetTableName))
		{
			return get(currNode->m_pRecvTable, tableName, netvarName);
		}
	}

	return 0;
}