#pragma once

#ifndef __MACROUTILS__H
#define __MACROUTILS__H


#define DRIVER_ERROR(fmt, ...) DbgPrint("[File: %s, Line: %d] " fmt, __FILE__, __LINE__, __VA_ARGS__)


#define CHECK_STATUS(status, exception)                     \
	__try {                                                \
		status = (exception);                               \
		if (!NT_SUCCESS(status)) {                           \
			DRIVER_ERROR("Error: %08x\n FILE:", status);            \
			goto CLEANUP;                                  \
		}                                                 \
	} __except(EXCEPTION_EXECUTE_HANDLER) {                                          \
		DRIVER_ERROR("Error: %08x\n", GetExceptionCode());  \
		goto CLEANUP;                                     \
	}


//
//
#define IOCTL_EXAMPLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REFRESH_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)


#define PROCESS_TERMINATE (0x0001)
#define PROCESS_CREATE_THREAD (0x0002)
#define PROCESS_SET_SESSIONID (0x0004)
#define PROCESS_VM_OPERATION (0x0008)
#define PROCESS_VM_READ (0x0010)
#define PROCESS_VM_WRITE (0x0020)
#define PROCESS_DUP_HANDLE (0x0040)
#define PROCESS_CREATE_PROCESS (0x0080)
#define PROCESS_SET_QUOTA (0x0100)
#define PROCESS_SET_INFORMATION (0x0200)
#define PROCESS_QUERY_INFORMATION (0x0400)
#define PROCESS_SUSPEND_RESUME (0x0800)
#define PROCESS_QUERY_LIMITED_INFORMATION (0x1000)
#define PROCESS_SET_LIMITED_INFORMATION (0x2000)



// LOG
#define MAX_TYPE_OBJECT_TYPE_DEPTH 100


#define EX_PUSH_LOCK ULONG_PTR

typedef struct _CALLBACK_ENTRY CALLBACK_ENTRY;
typedef struct _CALLBACK_ENTRY_ITEM CALLBACK_ENTRY_ITEM;

typedef struct _KLDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	ULONG64 __Undefined1;
	ULONG64 __Undefined2;
	ULONG64 __Undefined3;
	ULONG64 NonPagedDebugInfo;
	ULONG64 DllBase; //.sys's dllbase
	ULONG64 EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG   Flags;
	USHORT  LoadCount;
	USHORT  __Undefined5;
	ULONG64 __Undefined6;
	ULONG   CheckSum;
	ULONG   __padding1;
	ULONG   TimeDateStamp;
	ULONG   __padding2;
}KLDR_DATA_TABLE_ENTRY,LDR_DATA_TABLE_ENTRY,*PKLDR_DATA_TABLE_ENTRY;


typedef struct _OBJECT_TYPE {
	LIST_ENTRY TypeList;
	UNICODE_STRING Name;
	void* DefaultObject;
	unsigned __int8 Index;
	unsigned int TotalNumberOfObjects;
	unsigned int TotalNumberOfHandles;
	unsigned int HighWaterNumberOfObjects;
	unsigned int HighWaterNumberOfHandles;
	//_OBJECT_TYPE_INITIALIZER TypeInfo;
	unsigned __int8 Placeholder[0x78];
	EX_PUSH_LOCK TypeLock;
	unsigned int Key;
	LIST_ENTRY CallbackList;
}OBJECT_TYPE;
typedef struct _CALLBACK_ENTRY_ITEM {
	LIST_ENTRY EntryItemList;
	OB_OPERATION Operations;
	CALLBACK_ENTRY* CallbackEntry;
	POBJECT_TYPE ObjectType;
	POB_PRE_OPERATION_CALLBACK PreOperation;
	POB_POST_OPERATION_CALLBACK PostOperation;
	__int64 unk;
}CALLBACK_ENTRY_ITEM;

typedef struct _CALLBACK_ENTRY {
	__int16 Version;
	char buffer1[6];
	POB_OPERATION_REGISTRATION RegistrationContext;
	__int16 AltitudeLength1;
	__int16 AltitudeLength2;
	char buffer2[4];
	WCHAR* AltitudeString;
	CALLBACK_ENTRY_ITEM Items;
}CALLBACK_ENTRY;


typedef struct _CALL_BACK_INFO
{
	ULONG64 Unknow;
	ULONG64 Unknow1;
	UNICODE_STRING AltitudeString;
	LIST_ENTRY NexEntryItemList;
	ULONG64 Operations;
	PVOID ObHandle;
	PVOID ObjectType;
	ULONG64 PreCallbackAddr;
	ULONG64 PostCallbackAddr;
}CALL_BACK_INFO,*PCALL_BACK_INFO;

typedef struct _OB_CALLBACK
{
	LIST_ENTRY ListEntry;
	ULONG64 Operations;
	PCALL_BACK_INFO ObHandle;
	ULONG64 ObjTypeAddr;
	ULONG64 PreCall;
	ULONG64 PostCall;

}OB_CALLBACK,*POB_CALLBACK;




typedef struct _OBJECT_TYPE_FLAGS {
	UCHAR CaseInsensitive : 1;
	UCHAR UnnamedObjectsOnly : 1;
	UCHAR UseDefaultObject : 1;
	UCHAR SecurityRequired : 1;
	UCHAR MaintainHandleCount : 1;
	UCHAR MaintainTypeList : 1;
	UCHAR SupportsObjectCallbacks : 1;
	UCHAR CacheAligned : 1;
}OBJECT_TYPE_FLAGS, * P_OBJECT_TYPE_FLAGS;
 
 
typedef struct _OBJECT_TYPE_INITIALIZER {
	USHORT				wLength;
	OBJECT_TYPE_FLAGS	ObjectTypeFlags;
	ULONG				ObjcetTypeCode;
	ULONG				InvalidAttributes;
	GENERIC_MAPPING		GenericMapping;
	ULONG				ValidAccessMask;
	ULONG				RetainAccess;
	ULONG				PoolType;
	ULONG				DefaultPagedPoolCharge;
	ULONG				DefaultNonPagedPoolCharge;
	PVOID				DumpProcedure;
	PVOID				OpenProcedure;
	PVOID				CloseProcedure;
	PVOID				DeleteProcedure;
	PVOID				ParseProcedure;
	PVOID				SecurityProcedure;
	PVOID				QueryNameProcedure;
	PVOID				OkayToCloseProcedure;
}OBJECT_TYPE_INITIALIZER, * POBJECT_TYPE_INITIALIZER;
 
typedef struct _OBJECT_TYPE_EX {
	LIST_ENTRY					TypeList;
	UNICODE_STRING				Name;
	ULONGLONG					DefaultObject;
	ULONG						Index;
	ULONG						TotalNumberOfObjects;
	ULONG						TotalNumberOfHandles;
	ULONG						HighWaterNumberOfObjects;
	ULONG						HighWaterNumberOfHandles;
	OBJECT_TYPE_INITIALIZER		TypeInfo;
	ULONGLONG					TypeLock;
	ULONG						Key;
	LIST_ENTRY					CallbackList;
}OBJECT_TYPE_EX, * POBJECT_TYPE_EX;


typedef struct _ObjectTypeInfo {
    ULONG Index;                // 索引
    POBJECT_TYPE* ObjectType;   // 对应的全局变量指针
    PUNICODE_STRING Name;           // 对象类型的名称
} ObjectTypeInfo;


typedef enum _SYSTEM_INFORMATION_CLASS
{
    SystemBasicInformation = 0x0,
    SystemProcessorInformation = 0x1,
    SystemPerformanceInformation = 0x2,
    SystemTimeOfDayInformation = 0x3,
    SystemPathInformation = 0x4,
    SystemProcessInformation = 0x5,
    SystemCallCountInformation = 0x6,
    SystemDeviceInformation = 0x7,
    SystemProcessorPerformanceInformation = 0x8,
    SystemFlagsInformation = 0x9,
    SystemCallTimeInformation = 0xa,
    SystemModuleInformation = 0xb,
    SystemLocksInformation = 0xc,
    SystemStackTraceInformation = 0xd,
    SystemPagedPoolInformation = 0xe,
    SystemNonPagedPoolInformation = 0xf,
    SystemHandleInformation = 0x10,
    SystemObjectInformation = 0x11,
    SystemPageFileInformation = 0x12,
    SystemVdmInstemulInformation = 0x13,
    SystemVdmBopInformation = 0x14,
    SystemFileCacheInformation = 0x15,
    SystemPoolTagInformation = 0x16,
    SystemInterruptInformation = 0x17,
    SystemDpcBehaviorInformation = 0x18,
    SystemFullMemoryInformation = 0x19,
    SystemLoadGdiDriverInformation = 0x1a,
    SystemUnloadGdiDriverInformation = 0x1b,
    SystemTimeAdjustmentInformation = 0x1c,
    SystemSummaryMemoryInformation = 0x1d,
    SystemMirrorMemoryInformation = 0x1e,
    SystemPerformanceTraceInformation = 0x1f,
    SystemObsolete0 = 0x20,
    SystemExceptionInformation = 0x21,
    SystemCrashDumpStateInformation = 0x22,
    SystemKernelDebuggerInformation = 0x23,
    SystemContextSwitchInformation = 0x24,
    SystemRegistryQuotaInformation = 0x25,
    SystemExtendServiceTableInformation = 0x26,
    SystemPrioritySeperation = 0x27,
    SystemVerifierAddDriverInformation = 0x28,
    SystemVerifierRemoveDriverInformation = 0x29,
    SystemProcessorIdleInformation = 0x2a,
    SystemLegacyDriverInformation = 0x2b,
    SystemCurrentTimeZoneInformation = 0x2c,
    SystemLookasideInformation = 0x2d,
    SystemTimeSlipNotification = 0x2e,
    SystemSessionCreate = 0x2f,
    SystemSessionDetach = 0x30,
    SystemSessionInformation = 0x31,
    SystemRangeStartInformation = 0x32,
    SystemVerifierInformation = 0x33,
    SystemVerifierThunkExtend = 0x34,
    SystemSessionProcessInformation = 0x35,
    SystemLoadGdiDriverInSystemSpace = 0x36,
    SystemNumaProcessorMap = 0x37,
    SystemPrefetcherInformation = 0x38,
    SystemExtendedProcessInformation = 0x39,
    SystemRecommendedSharedDataAlignment = 0x3a,
    SystemComPlusPackage = 0x3b,
    SystemNumaAvailableMemory = 0x3c,
    SystemProcessorPowerInformation = 0x3d,
    SystemEmulationBasicInformation = 0x3e,
    SystemEmulationProcessorInformation = 0x3f,
    SystemExtendedHandleInformation = 0x40,
    SystemLostDelayedWriteInformation = 0x41,
    SystemBigPoolInformation = 0x42,
    SystemSessionPoolTagInformation = 0x43,
    SystemSessionMappedViewInformation = 0x44,
    SystemHotpatchInformation = 0x45,
    SystemObjectSecurityMode = 0x46,
    SystemWatchdogTimerHandler = 0x47,
    SystemWatchdogTimerInformation = 0x48,
    SystemLogicalProcessorInformation = 0x49,
    SystemWow64SharedInformationObsolete = 0x4a,
    SystemRegisterFirmwareTableInformationHandler = 0x4b,
    SystemFirmwareTableInformation = 0x4c,
    SystemModuleInformationEx = 0x4d,
    SystemVerifierTriageInformation = 0x4e,
    SystemSuperfetchInformation = 0x4f,
    SystemMemoryListInformation = 0x50,
    SystemFileCacheInformationEx = 0x51,
    SystemThreadPriorityClientIdInformation = 0x52,
    SystemProcessorIdleCycleTimeInformation = 0x53,
    SystemVerifierCancellationInformation = 0x54,
    SystemProcessorPowerInformationEx = 0x55,
    SystemRefTraceInformation = 0x56,
    SystemSpecialPoolInformation = 0x57,
    SystemProcessIdInformation = 0x58,
    SystemErrorPortInformation = 0x59,
    SystemBootEnvironmentInformation = 0x5a,
    SystemHypervisorInformation = 0x5b,
    SystemVerifierInformationEx = 0x5c,
    SystemTimeZoneInformation = 0x5d,
    SystemImageFileExecutionOptionsInformation = 0x5e,
    SystemCoverageInformation = 0x5f,
    SystemPrefetchPatchInformation = 0x60,
    SystemVerifierFaultsInformation = 0x61,
    SystemSystemPartitionInformation = 0x62,
    SystemSystemDiskInformation = 0x63,
    SystemProcessorPerformanceDistribution = 0x64,
    SystemNumaProximityNodeInformation = 0x65,
    SystemDynamicTimeZoneInformation = 0x66,
    SystemCodeIntegrityInformation = 0x67,
    SystemProcessorMicrocodeUpdateInformation = 0x68,
    SystemProcessorBrandString = 0x69,
    SystemVirtualAddressInformation = 0x6a,
    SystemLogicalProcessorAndGroupInformation = 0x6b,
    SystemProcessorCycleTimeInformation = 0x6c,
    SystemStoreInformation = 0x6d,
    SystemRegistryAppendString = 0x6e,
    SystemAitSamplingValue = 0x6f,
    SystemVhdBootInformation = 0x70,
    SystemCpuQuotaInformation = 0x71,
    SystemNativeBasicInformation = 0x72,
    SystemErrorPortTimeouts = 0x73,
    SystemLowPriorityIoInformation = 0x74,
    SystemBootEntropyInformation = 0x75,
    SystemVerifierCountersInformation = 0x76,
    SystemPagedPoolInformationEx = 0x77,
    SystemSystemPtesInformationEx = 0x78,
    SystemNodeDistanceInformation = 0x79,
    SystemAcpiAuditInformation = 0x7a,
    SystemBasicPerformanceInformation = 0x7b,
    SystemQueryPerformanceCounterInformation = 0x7c,
    SystemSessionBigPoolInformation = 0x7d,
    SystemBootGraphicsInformation = 0x7e,
    SystemScrubPhysicalMemoryInformation = 0x7f,
    SystemBadPageInformation = 0x80,
    SystemProcessorProfileControlArea = 0x81,
    SystemCombinePhysicalMemoryInformation = 0x82,
    SystemEntropyInterruptTimingInformation = 0x83,
    SystemConsoleInformation = 0x84,
    SystemPlatformBinaryInformation = 0x85,
    SystemThrottleNotificationInformation = 0x86,
    SystemHypervisorProcessorCountInformation = 0x87,
    SystemDeviceDataInformation = 0x88,
    SystemDeviceDataEnumerationInformation = 0x89,
    SystemMemoryTopologyInformation = 0x8a,
    SystemMemoryChannelInformation = 0x8b,
    SystemBootLogoInformation = 0x8c,
    SystemProcessorPerformanceInformationEx = 0x8d,
    SystemSpare0 = 0x8e,
    SystemSecureBootPolicyInformation = 0x8f,
    SystemPageFileInformationEx = 0x90,
    SystemSecureBootInformation = 0x91,
    SystemEntropyInterruptTimingRawInformation = 0x92,
    SystemPortableWorkspaceEfiLauncherInformation = 0x93,
    SystemFullProcessInformation = 0x94,
    SystemKernelDebuggerInformationEx = 0x95,
    SystemBootMetadataInformation = 0x96,
    SystemSoftRebootInformation = 0x97,
    SystemElamCertificateInformation = 0x98,
    SystemOfflineDumpConfigInformation = 0x99,
    SystemProcessorFeaturesInformation = 0x9a,
    SystemRegistryReconciliationInformation = 0x9b,
    MaxSystemInfoClass = 0x9c,
} SYSTEM_INFORMATION_CLASS,*PSYSTEM_INFORMATION_CLASS;



typedef struct _SYSTEM_PROCESS_INFORMATION {
    ULONG NextEntryOffset;
    ULONG NumberOfThreads;
    LARGE_INTEGER Reserved[3];
    LARGE_INTEGER CreateTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER KernelTime;
    UNICODE_STRING ImageName;
    ULONG BasePriority;
    HANDLE UniqueProcessId;
    PVOID InheritedFromUniqueProcessId;
    ULONG HandleCount;
    ULONG SessionId;
    ULONG_PTR PageDirectoryBase;
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
    SIZE_T PrivatePageCount;
    LARGE_INTEGER ReadOperationCount;
    LARGE_INTEGER WriteOperationCount;
    LARGE_INTEGER OtherOperationCount;
    LARGE_INTEGER ReadTransferCount;
    LARGE_INTEGER WriteTransferCount;
    LARGE_INTEGER OtherTransferCount;
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

#endif
