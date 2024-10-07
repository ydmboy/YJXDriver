#include <ntddk.h>
#include <wdm.h>

void DriverUnload(PDRIVER_OBJECT pdo)
{

}


RTL_AVL_TABLE avlTable;
RTL_GENERIC_COMPARE_RESULTS NTAPI Avlcompare(PRTL_AVL_TABLE tree,PVOID fs,PVOID ss)
{

	int infs = *(int*)fs;
	int inss = *(int*)ss;
	
	return (infs < inss) ? GenericLessThan : (infs > inss) ? GenericGreaterThan : GenericEqual;

}
PVOID NTAPI AvlAllocate(PRTL_AVL_TABLE Table,CLONG ByteSize)
{
	UNREFERENCED_PARAMETER(Table);
	return ExAllocatePoolWithTag(NonPagedPool, ByteSize, 'Av1T');
}
PVOID NTAPI AvlFree(PRTL_AVL_TABLE Table, PVOID buffer)
{
	UNREFERENCED_PARAMETER(Table);
	ExFreePoolWithTag(buffer, 'AvlT');
}
BOOLEAN InsertElement(int value)
{
    BOOLEAN newElement = FALSE;
    RtlInsertElementGenericTableAvl(&avlTable, &value, sizeof(int), &newElement);
    return newElement;
}
void TraverseAvlTree()
{
	PVOID restartKey = NULL;
	PVOID element = RtlEnumerateGenericTableWithoutSplayingAvl(&avlTable, &restartKey);
	while (element != NULL)
	{
		int value = *(int*)element;
		DbgPrint("Avl Tree Element:%d\n",value);
		element = RtlEnumerateGenericTableWithoutSplayingAvl(&avlTable, &restartKey);
	}
}

typedef struct _NON_PAGED_DEBUG_INFO
{
    USHORT      Signature;
    USHORT      Flags;
    ULONG       Size;
    USHORT      Machine;
    USHORT      Characteristics;
    ULONG       TimeDateStamp;
    ULONG       CheckSum;
    ULONG       SizeOfImage;
    ULONGLONG   ImageBase;
} NON_PAGED_DEBUG_INFO, *PNON_PAGED_DEBUG_INFO;

typedef struct _KLDR_DATA_TABLE_ENTRY
{
    LIST_ENTRY InLoadOrderLinks;
    PVOID ExceptionTable;
    ULONG ExceptionTableSize;
    // ULONG padding on IA64
    PVOID GpValue;
    PNON_PAGED_DEBUG_INFO NonPagedDebugInfo;
    PVOID DllBase;
    PVOID EntryPoint;
    ULONG SizeOfImage;
    UNICODE_STRING FullDllName;
    UNICODE_STRING BaseDllName;
    ULONG Flags;
    USHORT LoadCount;
    USHORT __Unused5;
    PVOID SectionPointer;
    ULONG CheckSum;
    // ULONG padding on IA64
    PVOID LoadedImports;
    PVOID PatchInformation;
} KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;

VOID PrintKLDRDataTableEntry(PKLDR_DATA_TABLE_ENTRY pdr)
{
    if (pdr == NULL) {
        DbgPrint("Invalid PKLDR_DATA_TABLE_ENTRY pointer.\n");
        return;
    }

    // Print out some of the key fields
    DbgPrint("KLDR_DATA_TABLE_ENTRY at: 0x%p\n", pdr);
    DbgPrint("  DllBase: 0x%p\n", pdr->DllBase);
    DbgPrint("  EntryPoint: 0x%p\n", pdr->EntryPoint);
    DbgPrint("  SizeOfImage: 0x%X\n", pdr->SizeOfImage);
    
    DbgPrint("  FullDllName: %wZ\n", &(pdr->FullDllName));
    DbgPrint("  BaseDllName: %wZ\n", &(pdr->BaseDllName));

    DbgPrint("  Flags: 0x%X\n", pdr->Flags);
    DbgPrint("  LoadCount: 0x%X\n", pdr->LoadCount);

    DbgPrint("  CheckSum: 0x%X\n", pdr->CheckSum);
    DbgPrint("  LoadedImports: 0x%p\n", pdr->LoadedImports);

    DbgPrint("  PatchInformation: 0x%p\n", pdr->PatchInformation);
}

#define IOCTL_EXAMPLE CTL_CODE(FILE_DEVICE_UNKNOWN,0x800,METHOD_BUFFERED,FILE_ANY_ACCESS)
NTSTATUS DispatchRoutineBuffer(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	DbgPrint(" DispatchRoutineBuffer:%d\n",stack->MajorFunction);
	NTSTATUS status = STATUS_SUCCESS;
	ULONG inputBufferLength, outputBufferLength;
	PVOID inputBuffer, outputBuffer;
	inputBufferLength = stack->Parameters.DeviceIoControl.InputBufferLength;
	//stack->Parameters.Read.
	outputBufferLength = stack->Parameters.DeviceIoControl.OutputBufferLength;
	inputBuffer = Irp->AssociatedIrp.SystemBuffer;
	outputBuffer = Irp->AssociatedIrp.SystemBuffer;

	switch (stack->MajorFunction)
	{
	case IRP_MJ_CREATE:
	case IRP_MJ_CLOSE:
		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = 0;
		break;

	case IRP_MJ_DEVICE_CONTROL:
		// 处理 I/O 控制码
		switch (stack->Parameters.DeviceIoControl.IoControlCode)
		{
		case IOCTL_EXAMPLE:
		{

			const char* outputData = "ooppqq";
			int outputDataLen = strlen(outputData) + 1;

			DbgPrint("%s", (char*)inputBuffer);
			RtlZeroMemory(outputBuffer, outputBufferLength);
			KdPrint(("IOCTL_EXAMPLE received\n"));
			DbgPrint("InputBufferLength:%d\n", inputBufferLength);
			DbgPrint("OutputBufferLength:%d\n", outputBufferLength);
			DbgPrint("outputDataStrLen:%d\n", outputDataLen);
			RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, outputData, outputDataLen);
			DbgPrint("SystemBuffer:%s\n", Irp->AssociatedIrp.SystemBuffer);
			//outputBufferLength = stack->Parameters.DeviceIoControl.OutputBufferLength; = strlen("ooppqq")
			Irp->IoStatus.Information = 20;
			Irp->IoStatus.Status = STATUS_SUCCESS;// status

			break;
		}
		default:
			status = STATUS_INVALID_DEVICE_REQUEST;
			break;
		}
	}

	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}


extern PLIST_ENTRY PsLoadedModuleList;
NTSTATUS DriverEntry(PDRIVER_OBJECT pobj, PUNICODE_STRING rgs)
{

	//PROCESS_CONTEXT asd;

	NTSTATUS nt = STATUS_SUCCESS;
	UNREFERENCED_PARAMETER(rgs);
	pobj->DriverUnload = DriverUnload;
	pobj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchRoutineBuffer;

	

	//PKLDR_DATA_TABLE_ENTRY pdr = (PKLDR_DATA_TABLE_ENTRY)pobj->DriverSection;
	////PrintKLDRDataTableEntry(kPdr);

	PKLDR_DATA_TABLE_ENTRY kPdrA = CONTAINING_RECORD(PsLoadedModuleList,KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
	PKLDR_DATA_TABLE_ENTRY kPdr = kPdrA->InLoadOrderLinks.Flink;
	while (kPdr != kPdrA)
	{
		PrintKLDRDataTableEntry(kPdr);
		kPdr = kPdr->InLoadOrderLinks.Flink;
	}



	//while (pdr != kPdr)
	//while(i != 0)
	//{
	//	PrintKLDRDataTableEntry(pdr);
	//	pdr = pdr->InLoadOrderLinks.Flink;
	//	i--;
	//}


	//while (pd)
	//{

	//}




	//RtlInitializeGenericTableAvl(&avlTable, Avlcompare, AvlAllocate, AvlFree, NULL);

	  // 插入元素
	//if (InsertElement(10))
	//	DbgPrint("Insert successfully\n");
	//else
	//	DbgPrint("Insert failed\n");
	//if(InsertElement(10))
	//	DbgPrint("Insert successfully");
	//else
	//	DbgPrint("Insert failed\n");
 //   if(InsertElement(20))
	//	DbgPrint("Insert successfully");
	//else
	//	DbgPrint("Insert failed\n");
 //   if(InsertElement(15))
	//	DbgPrint("Insert successfully");
	//else
	//	DbgPrint("Insert failed\n");


 //   // 遍历并打印 AVL 树
 //   TraverseAvlTree();

    // 返回成功状态
    return STATUS_SUCCESS;

}