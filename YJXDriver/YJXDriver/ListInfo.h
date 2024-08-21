#pragma once
#include "CommonHeader.h"
class ListInfo
{
public:
	void PrintSingleObjectType(POBJECT_TYPE objectType);
	void PrintObjectTypeInfo();
	PVOID GetObTypeIndexTable();
	void PrintObTypeIndexList(PVOID pObTypeIndexTable);
	ListInfo();
private:
	ObjectTypeInfo m_ObjectTypeMap[MAX_TYPE_OBJECT_TYPE_DEPTH];
	POBJECT_TYPE m_ObpTypeObjectType = nullptr;
	POBJECT_TYPE m_ObpDirectoryObjectType = nullptr;
	POBJECT_TYPE m_IoSymbolicLinkObjectType = nullptr;
	POBJECT_TYPE m_SeTokenObjectType = nullptr;
	POBJECT_TYPE m_PsJobType = nullptr;
	POBJECT_TYPE m_PsProcessType = nullptr;
	POBJECT_TYPE m_PsThreadType = nullptr;
	POBJECT_TYPE m_UserApcReserveType = nullptr;
	POBJECT_TYPE m_IoCompletionReserveType = nullptr;
	POBJECT_TYPE m_ActivityReferenceType = nullptr;
	POBJECT_TYPE m_PsSiloContextPagedType = nullptr;
	POBJECT_TYPE m_PsSiloContextNonPagedType = nullptr;
	POBJECT_TYPE m_DbgkDebugObjectType = nullptr;
	POBJECT_TYPE m_EventObjectType = nullptr;
	POBJECT_TYPE m_MutantObjectType = nullptr;
	POBJECT_TYPE m_CallbackObjectType = nullptr;
	POBJECT_TYPE m_SemaphoreObjectType = nullptr;
	POBJECT_TYPE m_TimerObjectType = nullptr;
	POBJECT_TYPE m_IRTimerObjectType = nullptr;
	POBJECT_TYPE m_ProfileObjectType = nullptr;
	POBJECT_TYPE m_KeyedEventObjectType = nullptr;
	POBJECT_TYPE m_WindowStationObjectType = nullptr;
	POBJECT_TYPE m_DesktopObjectType = nullptr;
	POBJECT_TYPE m_CompositionObjectType = nullptr;
	POBJECT_TYPE m_RawInputManagerObjectType = nullptr;
	POBJECT_TYPE m_CoreMessagingObjectType = nullptr;
	POBJECT_TYPE m_TpWorkerFactoryObjectType = nullptr;
	POBJECT_TYPE m_AdapterObjectType = nullptr;
	POBJECT_TYPE m_ControllerObjectType = nullptr;
	POBJECT_TYPE m_DeviceObjectType = nullptr;
	POBJECT_TYPE m_DriverObjectType = nullptr;
	POBJECT_TYPE m_IoCompletionObjectType = nullptr;
	POBJECT_TYPE m_WaitCompletionPacketObjectType = nullptr;
	POBJECT_TYPE m_FileObjectType = nullptr;
	POBJECT_TYPE m_TmTmObjectType = nullptr;
	POBJECT_TYPE m_TmTxObjectType = nullptr;
	POBJECT_TYPE m_TmRmObjectType = nullptr;
	POBJECT_TYPE m_TmEnObjectType = nullptr;
	POBJECT_TYPE m_SectionObjectType = nullptr;
	POBJECT_TYPE m_SessionObjectType = nullptr;
	POBJECT_TYPE m_PartitionObjectType = nullptr;
	POBJECT_TYPE m_KeyObjectType = nullptr;
	POBJECT_TYPE m_RegistryTransactionObjectType = nullptr;
	POBJECT_TYPE m_ALPCPortObjectType = nullptr;
	POBJECT_TYPE m_PowerRequestObjectType = nullptr;
	POBJECT_TYPE m_WmiGuidObjectType = nullptr;
	POBJECT_TYPE m_EtwRegistrationObjectType = nullptr;
	POBJECT_TYPE m_EtwSessionDemuxEntryObjectType = nullptr;
	POBJECT_TYPE m_EtwConsumerObjectType = nullptr;
	POBJECT_TYPE m_DmaAdapterObjectType = nullptr;
	POBJECT_TYPE m_DmaDomainObjectType = nullptr;
	POBJECT_TYPE m_PcwObjectObjectType = nullptr;
	POBJECT_TYPE m_FilterConnectionPortObjectType = nullptr;
	POBJECT_TYPE m_FilterCommunicationPortObjectType = nullptr;
	POBJECT_TYPE m_NdisCmStateObjectType = nullptr;
	POBJECT_TYPE m_DxgkSharedResourceObjectType = nullptr;
	POBJECT_TYPE m_DxgkSharedSyncObjectObjectType = nullptr;
	POBJECT_TYPE m_DxgkSharedSwapChainObjectObjectType = nullptr;
	POBJECT_TYPE m_DxgkCurrentDxgProcessObjectObjectType = nullptr;
	POBJECT_TYPE m_VRegConfigurationContextObjectType = nullptr;

    // 构造函数和其他方法可以在这里定义
};

