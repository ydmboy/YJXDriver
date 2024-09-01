#pragma once
#include "CommonHeader.h"

typedef struct PROCESS_LIST
{
	PROCESS_LIST* ls;
	HANDLE UniqueProcessId;
	PVOID InheritedFromUniqueProcessId;

}*PPROCESS_LIST;
