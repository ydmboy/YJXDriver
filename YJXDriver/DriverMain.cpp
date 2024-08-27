#include "AppDriverComm.h"
#include "CommonHeader.h"
#include "ListInfo.h"

extern "C"
void DriverUnload(PDRIVER_OBJECT DriverObject)
{
 //   UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\DosDevices\\ExampleDevice");
 //   IoDeleteSymbolicLink(&SymbolicLinkName);
	//UNICODE_STRING smLinkCode = RTL_CONSTANT_STRING(L"\\DosDevices\\ExampleDevice");
	//IoDeleteSymbolicLink(&smLinkCode);
	//PDEVICE_OBJECT pdObj = DriverObject->DeviceObject;
	//UnistallAllProcessType();
	//if (pdObj)
	//{
	//	IoDeleteDevice(DriverObject->DeviceObject);
	//	pdObj->NextDevice;
	//}
}

extern "C"
NTSTATUS DriverCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}


BOOLEAN IsValidUnicodeString(PUNICODE_STRING pstr)
{
    // 检查指针本身是否为 NULL
    if (pstr == NULL)
    {
        return FALSE;
    }

    // 检查 UNICODE_STRING 结构的成员是否有效
    if (pstr->Buffer == NULL || pstr->Length == 0 || pstr->MaximumLength == 0)
    {
        return FALSE;
    }

    // 检查 Length 和 MaximumLength 是否符合逻辑
    if (pstr->Length > pstr->MaximumLength)
    {
        return FALSE;
    }

    return TRUE;
}

extern "C"
NTSTATUS CreateDevice(_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING pUnicode_Device_String,
	_In_ PUNICODE_STRING pUnicode_SymbolicLinkName,
	_Outptr_ PDEVICE_OBJECT*  DeviceObject)
{

	NTSTATUS status = STATUS_SUCCESS;

	CHECK_STATUS(status ,status =IoCreateDevice(DriverObject,  sizeof(DriverObject->DriverExtension), pUnicode_Device_String, FILE_DEVICE_UNKNOWN, 0, FALSE, DeviceObject));

	CHECK_STATUS(status,status = IoCreateSymbolicLink(pUnicode_SymbolicLinkName, pUnicode_Device_String));


	//EXCEPTION_ACCESS_VIOLATION

	DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchRoutineBuffer;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchRoutineDirect;
	//DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchRoutineBuffer;
	(*DeviceObject)->Flags &= ~DO_DEVICE_INITIALIZING;
	//DeviceObject->Flags |= DO_BUFFERED_IO;

	return status;

CLEANUP:
	if (DeviceObject && *DeviceObject)
	{
		IoDeleteDevice(*DeviceObject);
		DeviceObject = NULL;
	}
	return status;
}


//void IoDeleteDevice(
//  [in] PDEVICE_OBJECT DeviceObject
//);
//extern "C"
//NTSTATUS  DeleteDevice()

extern "C"
NTSTATUS DisengageDrv(_In_ PDRIVER_OBJECT pdo,UNICODE_STRING& httpName)
{
	NTSTATUS status = STATUS_SUCCESS;
	PKLDR_DATA_TABLE_ENTRY selfNode = (PKLDR_DATA_TABLE_ENTRY)pdo->DriverSection;
	PKLDR_DATA_TABLE_ENTRY preNode = selfNode;

	do {
		DbgPrint("%wZ\r\n",&preNode->BaseDllName);
		//if (preNode->BaseDllName.Length != 0 && RtlCompareUnicodeString(&preNode->BaseDllName, &httpName, TRUE))
		//{
		//	DbgPrint("%wZ\r\n",&preNode->BaseDllName);
		//	RemoveEntryList((PLIST_ENTRY)preNode);
		//	break;
		//}
		preNode = (PKLDR_DATA_TABLE_ENTRY)preNode->InLoadOrderLinks.Flink;
	} while (preNode != selfNode);
	return status;

	
}

extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject,_In_ PUNICODE_STRING RegistryPath)
{

	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = DriverUnload;
	UNICODE_STRING unicode = { 0 };
	DisengageDrv(DriverObject,unicode);


	//DbgPrint("DriverEntry\n");
	//UNICODE_STRING deviceStr = { 0 };
	//UNICODE_STRING symblicStr = { 0 };
	//PDEVICE_OBJECT deviceObj = NULL;
	//RtlInitUnicodeString(&deviceStr, L"\\Device\\ExampleDevice");
 //   RtlInitUnicodeString(&symblicStr, L"\\DosDevices\\ExampleDevice");


	//CreateDevice(DriverObject,&deviceStr, &symblicStr,&deviceObj);
	//setMemoryProtect();
	//ListProcessTypeCallbacks();

	//ListInfo li;
	////li.PrintObjectTypeInfo();
	//li.PrintObTypeIndexList(li.GetObTypeIndexTable());

	return STATUS_SUCCESS;
}
