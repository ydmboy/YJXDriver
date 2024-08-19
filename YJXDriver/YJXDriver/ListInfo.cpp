#include "ListInfo.h"

// 输出单个 OBJECT_TYPE 结构体的内容
void ListInfo::PrintSingleObjectType(POBJECT_TYPE objectType)
{
	DbgPrint("Object Type: %wZ\n", &objectType->Name);
	DbgPrint("  Default Object: %p\n", objectType->DefaultObject);
	DbgPrint("  Index: %u\n", objectType->Index);
	DbgPrint("  Total Number of Objects: %u\n", objectType->TotalNumberOfObjects);
	DbgPrint("  Total Number of Handles: %u\n", objectType->TotalNumberOfHandles);
	DbgPrint("  High Watermark Objects: %u\n", objectType->HighWaterNumberOfObjects);
	DbgPrint("  High Watermark Handles: %u\n", objectType->HighWaterNumberOfHandles);
    // 根据需要输出更多字段
}

// 遍历 TypeList 并输出每个 OBJECT_TYPE 的内容
void ListInfo::PrintObjectTypeInfo()
{
    //extern POBJECT_TYPE ObpTypeObjectType;  // 假设我们可以访问 ObpTypeObjectType，它是对象类型链表的头
	UNICODE_STRING ObpTypeObTyStr = { 0 };
	RtlInitUnicodeString(&ObpTypeObTyStr, L"ObpTypeObjectType");


	POBJECT_TYPE ObpTypeObjectType =  (POBJECT_TYPE)MmGetSystemRoutineAddress(&ObpTypeObTyStr);

	if (!ObpTypeObjectType)
		return;

    PLIST_ENTRY head = &(ObpTypeObjectType->TypeList);  // 链表头
    PLIST_ENTRY current = head->Flink;  // 指向第一个对象类型

    // 遍历 TypeList 链表
    while (current != head)
    {
        // 通过 CONTAINING_RECORD 宏获取包含 TypeList 链表节点的 OBJECT_TYPE 结构体
        POBJECT_TYPE objectType = CONTAINING_RECORD(current, OBJECT_TYPE, TypeList);

        // 锁定类型列表，防止并发修改
        //ExAcquirePushLockExclusive(&objectType->TypeLock);

        // 输出该 OBJECT_TYPE 的信息
        PrintSingleObjectType(objectType);

        // 解锁类型列表
        //ExReleasePushLockExclusive(&objectType->TypeLock);

        // 移动到下一个对象类型
        current = current->Flink;
    }
}


PVOID ListInfo::GetObTypeIndexTable()
{
	UNICODE_STRING usObGetObjectType = RTL_CONSTANT_STRING(L"ObGetObjectType");
	PVOID pGetObTypeIndexTable = NULL;
	PVOID pObGetObjectType = (PVOID)MmGetSystemRoutineAddress(&usObGetObjectType);
	do
	{
		if (!pObGetObjectType)
		{
			DbgPrint("【PrintObjectTypeList】", "MmGetSystemRoutineAddress Failed! \r\n");
			break;
		}
 
		PUCHAR pStartAddress = (PUCHAR)pObGetObjectType;
		PUCHAR pTempAddress = pStartAddress;
		for (; pTempAddress < pStartAddress + PAGE_SIZE; pTempAddress++)
		{
			if ((*(pTempAddress - 3) == 0x48) &&
				(*(pTempAddress - 2) == 0x8d) &&
				(*(pTempAddress - 1) == 0x0d) &&
				(*(pTempAddress + 4) == 0x48) &&
				(*(pTempAddress + 5) == 0x8b) &&
				(*(pTempAddress + 6) == 0x04) &&
				(*(pTempAddress + 7) == 0xc1))
			{
				LONG lOffset = *(PLONG)(pTempAddress);
				pGetObTypeIndexTable = pTempAddress + 4 + lOffset;
				break;
			}
		}
 
	} while (false);
	if (pGetObTypeIndexTable)
	{
		DbgPrint("【ObRegisterCallback】", "Found ObTypeIndexTable Address:0x%p \r\n", pGetObTypeIndexTable);
	}
	else
	{
		DbgPrint("【PrintObjectTypeList】", "ObTypeIndexTable Not Found!\r\n");
	}
	return pGetObTypeIndexTable;
}


void ListInfo::PrintObTypeIndexList(PVOID pObTypeIndexTable)
{
	if (pObTypeIndexTable)
	{
		PUCHAR pStartAddress = ((PUCHAR)pObTypeIndexTable + 8 * 2); //从第2个开始
		POBJECT_TYPE_EX *pTempObjectType = (POBJECT_TYPE_EX*)(pStartAddress);
		ULONG ulIndex = 0;
		while(*pTempObjectType != NULL)
		{
			DbgPrint("【PrintObjectTypeList】", "Index:%02ld  Address:0x%p Name:%wZ\r\n", 
				ulIndex,
				*pTempObjectType,
				&(*pTempObjectType)->Name);
			pTempObjectType++;
			ulIndex++;
		}
 
	}
}