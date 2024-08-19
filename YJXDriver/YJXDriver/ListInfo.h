#pragma once
#include "CommonHeader.h"
class ListInfo
{
public:
	void PrintSingleObjectType(POBJECT_TYPE objectType);
	void PrintObjectTypeInfo();
	PVOID GetObTypeIndexTable();
	void PrintObTypeIndexList(PVOID pObTypeIndexTable);
};

