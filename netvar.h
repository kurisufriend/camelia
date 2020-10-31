#pragma once
#include "miscshit.h"
#include "includes.h"

namespace netvar
{
	DWORD get(RecvTable* baseTable, std::string tableName, std::string name);
	DWORD getNetvar(std::string tableName, std::string name);
}