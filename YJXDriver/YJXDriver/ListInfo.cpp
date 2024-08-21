#include "ListInfo.h"



ListInfo::ListInfo()
{
// 定义对象类型的映射数组
	m_ObjectTypeMap[0] = { 2, &m_ObpTypeObjectType, "Type" };
	m_ObjectTypeMap[1] = { 3, &m_ObpDirectoryObjectType, "Directory" };
	m_ObjectTypeMap[2] = { 4, &m_IoSymbolicLinkObjectType, "SymbolicLink" };
	m_ObjectTypeMap[3] = { 5, &m_SeTokenObjectType, "Token" };
	m_ObjectTypeMap[4] = { 6, &m_PsJobType, "Job" };
	m_ObjectTypeMap[5] = { 7, &m_PsProcessType, "Process" };
	m_ObjectTypeMap[6] = { 8, &m_PsThreadType, "Thread" };
	m_ObjectTypeMap[7] = { 9, &m_UserApcReserveType, "UserApcReserve" };
	m_ObjectTypeMap[8] = { 10, &m_IoCompletionReserveType, "IoCompletionReserve" };
	m_ObjectTypeMap[9] = { 11, &m_ActivityReferenceType, "ActivityReference" };
	m_ObjectTypeMap[10] = { 12, &m_PsSiloContextPagedType, "PsSiloContextPaged" };
	m_ObjectTypeMap[11] = { 13, &m_PsSiloContextNonPagedType, "PsSiloContextNonPaged" };
	m_ObjectTypeMap[12] = { 14, &m_DbgkDebugObjectType, "DebugObject" };
	m_ObjectTypeMap[13] = { 15, &m_EventObjectType, "Event" };
	m_ObjectTypeMap[14] = { 16, &m_MutantObjectType, "Mutant" };
	m_ObjectTypeMap[15] = { 17, &m_CallbackObjectType, "Callback" };
	m_ObjectTypeMap[16] = { 18, &m_SemaphoreObjectType, "Semaphore" };
	m_ObjectTypeMap[17] = { 19, &m_TimerObjectType, "Timer" };
	m_ObjectTypeMap[18] = { 20, &m_IRTimerObjectType, "IRTimer" };
	m_ObjectTypeMap[19] = { 21, &m_ProfileObjectType, "Profile" };
	m_ObjectTypeMap[20] = { 22, &m_KeyedEventObjectType, "KeyedEvent" };
	m_ObjectTypeMap[21] = { 23, &m_WindowStationObjectType, "WindowStation" };
	m_ObjectTypeMap[22] = { 24, &m_DesktopObjectType, "Desktop" };
	m_ObjectTypeMap[23] = { 25, &m_CompositionObjectType, "Composition" };
	m_ObjectTypeMap[24] = { 26, &m_RawInputManagerObjectType, "RawInputManager" };
	m_ObjectTypeMap[25] = { 27, &m_CoreMessagingObjectType, "CoreMessaging" };
	m_ObjectTypeMap[26] = { 28, &m_TpWorkerFactoryObjectType, "TpWorkerFactory" };
	m_ObjectTypeMap[27] = { 29, &m_AdapterObjectType, "Adapter" };
	m_ObjectTypeMap[28] = { 30, &m_ControllerObjectType, "Controller" };
	m_ObjectTypeMap[29] = { 31, &m_DeviceObjectType, "Device" };
	m_ObjectTypeMap[30] = { 32, &m_DriverObjectType, "Driver" };
	m_ObjectTypeMap[31] = { 33, &m_IoCompletionObjectType, "IoCompletion" };
	m_ObjectTypeMap[32] = { 34, &m_WaitCompletionPacketObjectType, "WaitCompletionPacket" };
	m_ObjectTypeMap[33] = { 35, &m_FileObjectType, "File" };
	m_ObjectTypeMap[34] = { 36, &m_TmTmObjectType, "TmTm" };
	m_ObjectTypeMap[35] = { 37, &m_TmTxObjectType, "TmTx" };
	m_ObjectTypeMap[36] = { 38, &m_TmRmObjectType, "TmRm" };
	m_ObjectTypeMap[37] = { 39, &m_TmEnObjectType, "TmEn" };
	m_ObjectTypeMap[38] = { 40, &m_SectionObjectType, "Section" };
	m_ObjectTypeMap[39] = { 41, &m_SessionObjectType, "Session" };
	m_ObjectTypeMap[40] = { 42, &m_PartitionObjectType, "Partition" };
	m_ObjectTypeMap[41] = { 43, &m_KeyObjectType, "Key" };
	m_ObjectTypeMap[42] = { 44, &m_RegistryTransactionObjectType, "RegistryTransaction" };
	m_ObjectTypeMap[43] = { 45, &m_ALPCPortObjectType, "ALPC Port" };
	m_ObjectTypeMap[44] = { 46, &m_PowerRequestObjectType, "PowerRequest" };
	m_ObjectTypeMap[45] = { 47, &m_WmiGuidObjectType, "WmiGuid" };
	m_ObjectTypeMap[46] = { 48, &m_EtwRegistrationObjectType, "EtwRegistration" };
	m_ObjectTypeMap[47] = { 49, &m_EtwSessionDemuxEntryObjectType, "EtwSessionDemuxEntry" };
	m_ObjectTypeMap[48] = { 50, &m_EtwConsumerObjectType, "EtwConsumer" };
	m_ObjectTypeMap[49] = { 51, &m_DmaAdapterObjectType, "DmaAdapter" };
	m_ObjectTypeMap[50] = { 52, &m_DmaDomainObjectType, "DmaDomain" };
	m_ObjectTypeMap[51] = { 53, &m_PcwObjectObjectType, "PcwObject" };
	m_ObjectTypeMap[52] = { 54, &m_FilterConnectionPortObjectType, "FilterConnectionPort" };
	m_ObjectTypeMap[53] = { 55, &m_FilterCommunicationPortObjectType, "FilterCommunicationPort" };
	m_ObjectTypeMap[54] = { 56, &m_NdisCmStateObjectType, "NdisCmState" };
	m_ObjectTypeMap[55] = { 57, &m_DxgkSharedResourceObjectType, "DxgkSharedResource" };
	m_ObjectTypeMap[56] = { 58, &m_DxgkSharedSyncObjectObjectType, "DxgkSharedSyncObject" };
	m_ObjectTypeMap[57] = { 59, &m_DxgkSharedSwapChainObjectObjectType, "DxgkSharedSwapChainObject" };
	m_ObjectTypeMap[58] = { 60, &m_DxgkCurrentDxgProcessObjectObjectType, "DxgkCurrentDxgProcessObject" };
	m_ObjectTypeMap[59] = { 61, &m_VRegConfigurationContextObjectType, "VRegConfigurationContext" };

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
			if((*((POBJECT_TYPE*)pTempObjectType)) == *PsProcessType)
				DbgPrint("_________________________________");
			DbgPrint("【PrintObjectTypeList】 Index:%d  Address:0x%p Name:%wZ\r\n", 
				(*pTempObjectType)->Index,
				*pTempObjectType,
				&(*pTempObjectType)->Name);
			pTempObjectType++;
			ulIndex++;
		}
 
	}
}