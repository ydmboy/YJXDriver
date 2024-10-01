#include <ntddk.h>
#include <wdm.h>

extern PLIST_ENTRY PsLoadedModuleList; // 外部引用，已加载模块列表

NTSTATUS GetFileVersionInfo(PUNICODE_STRING FileName, PVOID* VersionInfoBuffer)
{
	NTSTATUS status;
	HANDLE fileHandle = NULL;
	OBJECT_ATTRIBUTES objAttrs;
	IO_STATUS_BLOCK ioStatus;
	FILE_STANDARD_INFORMATION fileStandardInfo;

	// Initialize object attributes for the file
	InitializeObjectAttributes(&objAttrs, FileName, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	// Open the file to get a handle
	status = ZwCreateFile(&fileHandle,
		GENERIC_READ,
		&objAttrs,
		&ioStatus,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ,
		FILE_OPEN,
		FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
		NULL,
		0);

	if (!NT_SUCCESS(status)) {
		DbgPrint("Failed to open file: %wZ, Status: 0x%x\n", FileName, status);
		return status;
	}

	// Query the file size
	status = ZwQueryInformationFile(fileHandle, &ioStatus, &fileStandardInfo, sizeof(fileStandardInfo), FileStandardInformation);
	if (!NT_SUCCESS(status)) {
		DbgPrint("Failed to query file information: %wZ, Status: 0x%x\n", FileName, status);
		ZwClose(fileHandle);
		return status;
	}

	// Allocate memory for version info (file size + some extra for safety)
	*VersionInfoBuffer = ExAllocatePoolWithTag(NonPagedPool, fileStandardInfo.EndOfFile.LowPart + 100, 'verT');
	if (*VersionInfoBuffer == NULL) {
		DbgPrint("Failed to allocate memory for version info.\n");
		ZwClose(fileHandle);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	// Read the file contents into the buffer
	LARGE_INTEGER byteOffset;
	byteOffset.LowPart = byteOffset.HighPart = 0;

	status = ZwReadFile(fileHandle, NULL, NULL, NULL, &ioStatus, *VersionInfoBuffer, fileStandardInfo.EndOfFile.LowPart, &byteOffset, NULL);
	if (!NT_SUCCESS(status)) {
		DbgPrint("Failed to read file: %wZ, Status: 0x%x\n", FileName, status);
		ExFreePoolWithTag(*VersionInfoBuffer, 'verT');
		*VersionInfoBuffer = NULL;
		ZwClose(fileHandle);
		return status;
	}

	ZwClose(fileHandle);
	return STATUS_SUCCESS;
}

NTSTATUS GetFileManufacturer(PUNICODE_STRING FileName)
{
	PVOID versionInfoBuffer = NULL;
	NTSTATUS status;

	// Get file version information
	status = GetFileVersionInfo(FileName, &versionInfoBuffer);
	if (!NT_SUCCESS(status)) {
		DbgPrint("Failed to get file version info for: %wZ, Status: 0x%x\n", FileName, status);
		return status;
	}

	// Assuming you have parsed the version info, you can now extract manufacturer information
	// (The parsing part would involve looking for the company name string in the version info block)

	// Example debug output
	DbgPrint("File Manufacturer for %wZ: <Company Name Here>\n,%wZ", FileName,versionInfoBuffer);

	// Free allocated memory
	if (versionInfoBuffer) {
		ExFreePoolWithTag(versionInfoBuffer, 'verT');
	}

	return STATUS_SUCCESS;
}

void DriverUnload(PDRIVER_OBJECT DriverObject)
{
	DbgPrint("Driver Unload\n");
}


typedef struct _LDR_DATA_TABLE_ENTRY {
    LIST_ENTRY InLoadOrderLinks;        // 链接到加载顺序列表的下一个和上一个模块
    LIST_ENTRY InMemoryOrderLinks;      // 链接到内存顺序列表的下一个和上一个模块
    LIST_ENTRY InInitializationOrderLinks;  // 链接到初始化顺序列表的下一个和上一个模块
    PVOID DllBase;                      // 模块的基地址（加载到内存中的基址）
    PVOID EntryPoint;                   // 模块的入口点（如果适用）
    ULONG SizeOfImage;                  // 模块的大小（以字节为单位）
    UNICODE_STRING FullDllName;         // 模块的完整路径名
    UNICODE_STRING BaseDllName;         // 模块的基本名称（不包含路径）
    ULONG Flags;                        // 模块标志（加载标志、状态等）
    USHORT LoadCount;                   // 模块的加载次数
    USHORT TlsIndex;                    // 线程局部存储索引
    LIST_ENTRY HashLinks;               // 链接到哈希表的下一个和上一个模块（用于快速查找）
    ULONG TimeDateStamp;                // 模块的时间戳（编译时间）
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;


void TraverseDriversAndPrintInfo()
{
    PLIST_ENTRY moduleList = PsLoadedModuleList; // 已加载模块列表
    PLDR_DATA_TABLE_ENTRY ldrEntry;
    UNICODE_STRING fileName;
	int i = 0;

	if (!moduleList)
		return;
    // 遍历PsLoadedModuleList链表
    for (PLIST_ENTRY pEntry = moduleList->Flink; pEntry != moduleList; pEntry = pEntry->Flink)
    {
		DbgPrint("LoaderModuleList:%d\n",i++);

	   if (pEntry == NULL)
        {
            DbgPrint("Invalid list entry, stopping traversal.\n");
            break;
        }
	   if (i >= 1000)
	   {
		   DbgPrint("Invalid list entry, stopping traversal.\n");
		   break;
	   }
        ldrEntry = CONTAINING_RECORD(pEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

        // 获取驱动程序的完整路径名
        fileName = ldrEntry->FullDllName;

        // 打印驱动程序的完整路径
        DbgPrint("Driver Full Path: %wZ\n", &fileName);

        // 获取并打印驱动程序的厂商信息
        GetFileManufacturer(&fileName);
    }
}

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
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_THREAD_INFORMATION
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	PVOID StartAddress;
	CLIENT_ID ClientId;
	KPRIORITY Priority;
	LONG BasePriority;
	ULONG ContextSwitches;
	ULONG ThreadState;
	KWAIT_REASON WaitReason;
}SYSTEM_THREAD_INFORMATION, *PSYSTEM_THREAD_INFORMATION;

//typedef struct _SYSTEM_PROCESS_INFO
//{
//    ULONG NextEntryOffset;
//    ULONG NumberOfThreads;
//    LARGE_INTEGER WorkingSetPrivateSize;
//    ULONG HardFaultCount;
//    ULONG NumberOfThreadsHighWatermark;
//    ULONGLONG CycleTime;
//    LARGE_INTEGER CreateTime;
//    LARGE_INTEGER UserTime;
//    LARGE_INTEGER KernelTime;
//    UNICODE_STRING ImageName;
//    KPRIORITY BasePriority;
//    HANDLE UniqueProcessId;
//    HANDLE InheritedFromUniqueProcessId;
//    ULONG HandleCount;
//    ULONG SessionId;
//    ULONG_PTR UniqueProcessKey;
//    SIZE_T PeakVirtualSize;
//    SIZE_T VirtualSize;
//    ULONG PageFaultCount;
//    SIZE_T PeakWorkingSetSize;
//    SIZE_T WorkingSetSize;
//    SIZE_T QuotaPeakPagedPoolUsage;
//    SIZE_T QuotaPagedPoolUsage;
//    SIZE_T QuotaPeakNonPagedPoolUsage;
//    SIZE_T QuotaNonPagedPoolUsage;
//    SIZE_T PagefileUsage;
//    SIZE_T PeakPagefileUsage;
//    SIZE_T PrivatePageCount;
//    LARGE_INTEGER ReadOperationCount;
//    LARGE_INTEGER WriteOperationCount;
//    LARGE_INTEGER OtherOperationCount;
//    LARGE_INTEGER ReadTransferCount;
//    LARGE_INTEGER WriteTransferCount;
//    LARGE_INTEGER OtherTransferCount;
//    SYSTEM_THREAD_INFORMATION Threads[1];
//}SYSTEM_PROCESS_INFO, *PSYSTEM_PROCESS_INFO;


typedef struct _SYSTEM_THREADS
{
	LARGE_INTEGER           KernelTime;
	LARGE_INTEGER           UserTime;
	LARGE_INTEGER           CreateTime;
	ULONG                   WaitTime;
	PVOID                   StartAddress;
	CLIENT_ID               ClientIs;
	KPRIORITY               Priority;
	KPRIORITY               BasePriority;
	ULONG                   ContextSwitchCount;
	ULONG                   ThreadState;
	KWAIT_REASON            WaitReason;
}SYSTEM_THREADS, *PSYSTEM_THREADS;




//进程信息结构体  
//typedef struct _SYSTEM_PROCESSES
//{
//    ULONG                           NextEntryDelta;    //链表下一个结构和上一个结构的偏移
//    ULONG                           ThreadCount;
//    ULONG                           Reserved[6];
//    LARGE_INTEGER                   CreateTime;
//    LARGE_INTEGER                   UserTime;
//    LARGE_INTEGER                   KernelTime;
//    UNICODE_STRING                  ProcessName;     //进程名字
//    KPRIORITY                       BasePriority;
//    ULONG                           ProcessId;      //进程的pid号
//    ULONG                           InheritedFromProcessId;
//    ULONG                           HandleCount;
//    ULONG                           Reserved2[2];
//    VM_COUNTERS                     VmCounters;
//    IO_COUNTERS                     IoCounters; //windows 2000 only  
//    struct _SYSTEM_THREADS          Threads[1];
//}SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;

typedef struct _SYSTEM_PROCESSES
{
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER WorkingSetPrivateSize;
	ULONG HardFaultCount;
	ULONG NumberOfThreadsHighWatermark;
	ULONGLONG CycleTime;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING processName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	HANDLE InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG SessionId;
	ULONG_PTR UniqueProcessKey;
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
	SYSTEM_THREAD_INFORMATION Threads[1];
}SYSTEM_PROCESSES, *PSYSTEM_PROCESSES;

NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL 
    );

// 枚举系统中所有进程的函数
NTSTATUS EnumerateProcesses()
{
	DbgPrint("work in the EnumerateProcess\n");
    NTSTATUS status;
    ULONG bufferSize = 128; // 初始缓冲区大小
    PVOID buffer = NULL;
    ULONG returnLength = 0;
	//SYSTEM_INFORMATION_CLASS SystemProcessInformation = 5;
	int x = 0;

    // 分配缓冲区并调用 ZwQuerySystemInformation
    do
    {
        buffer = ExAllocatePoolWithTag(NonPagedPool, bufferSize, 'prcs');
        if (!buffer)
        {
            DbgPrint("内存分配失败。\n");
            return STATUS_INSUFFICIENT_RESOURCES;
        }

        status = ZwQuerySystemInformation(SystemProcessInformation, buffer,bufferSize, &returnLength);
        if (status == STATUS_INFO_LENGTH_MISMATCH)
        {
            ExFreePoolWithTag(buffer, 'prcs');
			buffer = NULL;
            //bufferSize = returnLength;
			bufferSize = returnLength;
			DbgPrint("bufferSize %d\n",bufferSize);
			DbgPrint("need buffer%d\n",returnLength);
        }
        else if (!NT_SUCCESS(status))
        {
            DbgPrint("ZwQuerySystemInformation 失败，状态码: 0x%X\n", status);
            ExFreePoolWithTag(buffer, 'prcs');
            return status;
        }
		else
		{
			DbgPrint("else size\n");
		}
    } while (status == STATUS_INFO_LENGTH_MISMATCH);

    // 解析进程信息
    if (NT_SUCCESS(status))
    {
		PSYSTEM_PROCESSES processInfo = (PSYSTEM_PROCESSES)buffer;

        do
        {
            if (processInfo->processName.Buffer)
            {
                DbgPrint("进程名称:%-20ws  (PID: %d)\n", processInfo->processName.Buffer, (ULONG)(ULONG_PTR)processInfo->UniqueProcessId);
            }
            else
            {
				DbgPrint("进程名称: (空) (PID: %u)\n", (ULONG)(ULONG_PTR)processInfo->UniqueProcessId);

            }

            if (processInfo->NextEntryOffset == 0)
                break;
			//processInfo->NextEntryOffset;
            processInfo = (PSYSTEM_PROCESSES)((PUCHAR)processInfo +processInfo->NextEntryOffset);
        } while (TRUE);
    }

    ExFreePoolWithTag(buffer, 'prcs');
    return status;
}


//extern PLIST_ENTRY PsActiveProcessHead;
//
//PEPROCESS 


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNICODE_STRING fileName;
	PLDR_DATA_TABLE_ENTRY driverEntry;

	DbgPrint("Driver Loaded\n");

	DriverObject->DriverUnload = DriverUnload;
	HANDLE handle = PsGetCurrentProcessId();
	DbgPrint("CurrentProcessId: %p\n", handle);
	DbgPrint("CurrentProcessId:0x%x\n", handle);

	DbgPrint("Handle:%d",PsGetCurrentProcessId());

	//PEPROCESS process = { 0 };


	//EnumerateProcesses();


	//// 获取当前驱动程序的 LDR 数据表项
	//driverEntry = (PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;

	//// 获取驱动程序文件名
	//fileName = driverEntry->FullDllName;

	//// 打印当前驱动的文件路径
	//DbgPrint("Current Driver File Path: %wZ\n", &fileName);

	//// 获取当前驱动的厂商信息
	//GetFileManufacturer(&fileName);


	//TraverseDriversAndPrintInfo();



	return STATUS_SUCCESS;
}

