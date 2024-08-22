#include "ListInfo.h"



ListInfo::ListInfo()
{

// 定义对象类型的映射数组
	m_ObjectTypeMap[0] = { 2, &m_ObpTypeObjectType};
	m_ObjectTypeMap[1] = { 3, &m_ObpDirectoryObjectType };
	m_ObjectTypeMap[2] = { 4, &m_IoSymbolicLinkObjectType };
	m_ObjectTypeMap[3] = { 5, &m_SeTokenObjectType};
	m_ObjectTypeMap[4] = { 6, &m_PsJobType };
	m_ObjectTypeMap[5] = { 7, &m_PsProcessType};
	m_ObjectTypeMap[6] = { 8, &m_PsThreadType };
	m_ObjectTypeMap[7] = { 9, &m_UserApcReserveType};
	m_ObjectTypeMap[8] = { 10, &m_IoCompletionReserveType};
	m_ObjectTypeMap[9] = { 11, &m_ActivityReferenceType };
	m_ObjectTypeMap[10] = { 12, &m_PsSiloContextPagedType };
	m_ObjectTypeMap[11] = { 13, &m_PsSiloContextNonPagedType };
	m_ObjectTypeMap[12] = { 14, &m_DbgkDebugObjectType };
	m_ObjectTypeMap[13] = { 15, &m_EventObjectType };
	m_ObjectTypeMap[14] = { 16, &m_MutantObjectType };
	m_ObjectTypeMap[15] = { 17, &m_CallbackObjectType };
	m_ObjectTypeMap[16] = { 18, &m_SemaphoreObjectType };
	m_ObjectTypeMap[17] = { 19, &m_TimerObjectType };
	m_ObjectTypeMap[18] = { 20, &m_IRTimerObjectType };
	m_ObjectTypeMap[19] = { 21, &m_ProfileObjectType };
	m_ObjectTypeMap[20] = { 22, &m_KeyedEventObjectType };
	m_ObjectTypeMap[21] = { 23, &m_WindowStationObjectType };
	m_ObjectTypeMap[22] = { 24, &m_DesktopObjectType };
	m_ObjectTypeMap[23] = { 25, &m_CompositionObjectType };
	m_ObjectTypeMap[24] = { 26, &m_RawInputManagerObjectType };
	m_ObjectTypeMap[25] = { 27, &m_CoreMessagingObjectType };
	m_ObjectTypeMap[26] = { 28, &m_TpWorkerFactoryObjectType };
	m_ObjectTypeMap[27] = { 29, &m_AdapterObjectType };
	m_ObjectTypeMap[28] = { 30, &m_ControllerObjectType };
	m_ObjectTypeMap[29] = { 31, &m_DeviceObjectType };
	m_ObjectTypeMap[30] = { 32, &m_DriverObjectType };
	m_ObjectTypeMap[31] = { 33, &m_IoCompletionObjectType };
	m_ObjectTypeMap[32] = { 34, &m_WaitCompletionPacketObjectType};
	m_ObjectTypeMap[33] = { 35, &m_FileObjectType };
	m_ObjectTypeMap[34] = { 36, &m_TmTmObjectType };
	m_ObjectTypeMap[35] = { 37, &m_TmTxObjectType };
	m_ObjectTypeMap[36] = { 38, &m_TmRmObjectType };
	m_ObjectTypeMap[37] = { 39, &m_TmEnObjectType };
	m_ObjectTypeMap[38] = { 40, &m_SectionObjectType };
	m_ObjectTypeMap[39] = { 41, &m_SessionObjectType };
	m_ObjectTypeMap[40] = { 42, &m_PartitionObjectType };
	m_ObjectTypeMap[41] = { 43, &m_KeyObjectType };
	m_ObjectTypeMap[42] = { 44, &m_RegistryTransactionObjectType };
	m_ObjectTypeMap[43] = { 45, &m_ALPCPortObjectType };
	m_ObjectTypeMap[44] = { 46, &m_PowerRequestObjectType };
	m_ObjectTypeMap[45] = { 47, &m_WmiGuidObjectType };
	m_ObjectTypeMap[46] = { 48, &m_EtwRegistrationObjectType };
	m_ObjectTypeMap[47] = { 49, &m_EtwSessionDemuxEntryObjectType };
	m_ObjectTypeMap[48] = { 50, &m_EtwConsumerObjectType };
	m_ObjectTypeMap[49] = { 51, &m_DmaAdapterObjectType };
	m_ObjectTypeMap[50] = { 52, &m_DmaDomainObjectType };
	m_ObjectTypeMap[51] = { 53, &m_PcwObjectObjectType};
	m_ObjectTypeMap[52] = { 54, &m_FilterConnectionPortObjectType };
	m_ObjectTypeMap[53] = { 55, &m_FilterCommunicationPortObjectType };
	m_ObjectTypeMap[54] = { 56, &m_NdisCmStateObjectType };
	m_ObjectTypeMap[55] = { 57, &m_DxgkSharedResourceObjectType };
	m_ObjectTypeMap[56] = { 58, &m_DxgkSharedSyncObjectObjectType };
	m_ObjectTypeMap[57] = { 59, &m_DxgkSharedSwapChainObjectObjectType };
	m_ObjectTypeMap[58] = { 60, &m_DxgkCurrentDxgProcessObjectObjectType };
	m_ObjectTypeMap[59] = { 61, &m_VRegConfigurationContextObjectType };



}
// 输出单个 OBJECT_TYPE 结构体的内容
void ListInfo::PrintSingleObjectType(POBJECT_TYPE objectType)
{
	DbgPrint("_________________________________");

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
	DbgPrint("ObpTypeObjectType:%0x\n",ObpTypeObjectType);
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
			DbgPrint("【PrintObjectTypeList】MmGetSystemRoutineAddress Failed! \r\n");
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
		DbgPrint("【ObRegisterCallback】Found ObTypeIndexTable Address:0x%p \r\n", pGetObTypeIndexTable);
	}
	else
	{
		DbgPrint("【PrintObjectTypeList】ObTypeIndexTable Not Found!\r\n");
	}
	return pGetObTypeIndexTable;
}
void ListInfo::PrintObTypeIndexList(PVOID pObTypeIndexTable)
{
	DbgPrint("_________________________________\n");
	DbgPrint("PsProcessType:0x%p", PsProcessType);
	DbgPrint("*PsProcessType:0x%p",*PsProcessType);
	//DbgPrint("ObpTypeObject:0x%p",*ObpTypeObjectType);


	if (pObTypeIndexTable)
	{
		PUCHAR pStartAddress = ((PUCHAR)pObTypeIndexTable + 8 * 2); //从第2个开始
		POBJECT_TYPE *pTempObjectType = (POBJECT_TYPE*)(pStartAddress);
	

		ULONG ulIndex = 0;
		while(*pTempObjectType != NULL)
		{
			//DbgPrint("【PrintObjectTypeList】 Index:%d  Address:0x%p Name:%wZ\r\n", 
			//	(*pTempObjectType)->Index,
			//	*pTempObjectType,
			//	&(*pTempObjectType)->Name);
			m_ObjectTypeMap[ulIndex].Index = (*pTempObjectType)->Index;
			m_ObjectTypeMap[ulIndex].Name = &(*pTempObjectType)->Name;
			m_ObjectTypeMap[ulIndex].ObjectType = pTempObjectType;

			DbgPrint("PrintObjectTypeList Index:%d  Address:0x%p Name:%wZ\r\n", m_ObjectTypeMap[ulIndex].Index, *m_ObjectTypeMap[ulIndex].ObjectType, m_ObjectTypeMap[ulIndex].Name);
			if (&(*m_ObjectTypeMap[ulIndex].ObjectType)->CallbackList != (*m_ObjectTypeMap[ulIndex].ObjectType)->CallbackList.Blink ||
				&(*m_ObjectTypeMap[ulIndex].ObjectType)->CallbackList != (*m_ObjectTypeMap[ulIndex].ObjectType)->CallbackList.Flink
				)
			{
				ListTypeObjectTypeCallbacks(m_ObjectTypeMap[ulIndex].ObjectType);
			}
			ulIndex++;
			pTempObjectType++;
		}
 
	}
}

NTSTATUS ListInfo::ListTypeObjectTypeCallbacks(POBJECT_TYPE* ObpTypeObjectType)
{
    OBJECT_TYPE* processObjectType = *PsProcessType;
    PLIST_ENTRY head = (PLIST_ENTRY)&(processObjectType->CallbackList);
    PLIST_ENTRY current = head->Flink;

    while (current != head)
    {
        CALLBACK_ENTRY_ITEM* item = (CALLBACK_ENTRY_ITEM*)current;
        CALLBACK_ENTRY* entry = item->CallbackEntry;

        if (entry)
        {
            // 处理或打印回调信息
            DbgPrint("Callback found: %ws,Address:%p\n", entry->AltitudeString,entry);
			//DbgPrint("gsHande:%p",gs_handleCallback);
			//DbgPrint("Func:%p",my_pre_callback);
        }

        current = current->Flink;
    }
    return STATUS_SUCCESS;
}