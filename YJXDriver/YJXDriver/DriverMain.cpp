#include "AppDriverComm.h"
#include "CommonHeader.h"
#include "ListInfo.h"
#include <minwindef.h>

extern "C"
void DriverUnload(PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\DosDevices\\ExampleDevice");
    IoDeleteSymbolicLink(&SymbolicLinkName);
	UNICODE_STRING smLinkCode = RTL_CONSTANT_STRING(L"\\DosDevices\\ExampleDevice");
	IoDeleteSymbolicLink(&smLinkCode);
	PDEVICE_OBJECT pdObj = DriverObject->DeviceObject;
	UnistallAllProcessType();
	if (pdObj)
	{
		IoDeleteDevice(DriverObject->DeviceObject);
		pdObj->NextDevice;
	}
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
    // ���ָ�뱾���Ƿ�Ϊ NULL
    if (pstr == NULL)
    {
        return FALSE;
    }

    // ��� UNICODE_STRING �ṹ�ĳ�Ա�Ƿ���Ч
    if (pstr->Buffer == NULL || pstr->Length == 0 || pstr->MaximumLength == 0)
    {
        return FALSE;
    }

    // ��� Length �� MaximumLength �Ƿ�����߼�
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
NTSYSAPI
NTSTATUS
ZwQuerySystemInformation(
    IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
    OUT PVOID SystemInformation,
    IN ULONG SystemInformationLength,
    OUT PULONG ReturnLength OPTIONAL 
    );

NTSTATUS QuerySystemProcessInformation(PVOID* ProcessInfoBuffer, ULONG* ProcessInfoLength)
{
    NTSTATUS status;
    ULONG bufferLength = 0x10000; // ��ʼ��������С
    PVOID buffer = NULL;

    // �����ڴ��������
    buffer = ExAllocatePoolWithTag(NonPagedPool, bufferLength, 'Proc');
    if (buffer == NULL)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    while (TRUE)
    {
        // ���� ZwQuerySystemInformation ��ȡϵͳ������Ϣ
        status = ZwQuerySystemInformation(SystemProcessInformation, buffer, bufferLength, &bufferLength);

        if (status == STATUS_INFO_LENGTH_MISMATCH)
        {
            // ���������������·������Ļ�����
            ExFreePool(buffer);
            buffer = ExAllocatePoolWithTag(NonPagedPool, bufferLength, 'Proc');
            if (buffer == NULL)
            {
                return STATUS_INSUFFICIENT_RESOURCES;
            }
        }
        else
        {
            break;
        }
    }

    if (NT_SUCCESS(status))
    {
        *ProcessInfoBuffer = buffer;
        *ProcessInfoLength = bufferLength;
    }
    else
    {
        // ��������
        ExFreePool(buffer);
    }

    return status;
}

extern "C" VOID FreeSystemProcessInformation(PVOID ProcessInfoBuffer)
{
    if (ProcessInfoBuffer)
    {
        ExFreePool(ProcessInfoBuffer);
    }
}

VOID PrintSystemProcessInformation(PSYSTEM_PROCESS_INFORMATION ProcessInfo, ULONG BufferLength) {
	if (ProcessInfo == NULL) {
		DbgPrint("Invalid ProcessInfo pointer.\n");
		return;
	}

	// ���ڱ�������������
	ULONG offset = 0;

	while (offset < BufferLength) {
		PSYSTEM_PROCESS_INFORMATION currentProcess = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)ProcessInfo + offset);

		// ��ӡ�����ֶ�
		DbgPrint("---------------------------------------------------\n");
		DbgPrint("Process ID: %p\n", currentProcess->UniqueProcessId);
		DbgPrint("Image Name: %wZ\n", &currentProcess->ImageName);
		DbgPrint("Base Priority: %lu\n", currentProcess->BasePriority);
		DbgPrint("Handle Count: %lu\n", currentProcess->HandleCount);
		DbgPrint("Session ID: %lu\n", currentProcess->SessionId);
		DbgPrint("Virtual Size: %llu\n", currentProcess->VirtualSize);
		DbgPrint("Working Set Size: %llu\n", currentProcess->WorkingSetSize);
		DbgPrint("Pagefile Usage: %llu\n", currentProcess->PagefileUsage);
		DbgPrint("Private Page Count: %llu\n", currentProcess->PrivatePageCount);
		DbgPrint("Read Operation Count: %llu\n", currentProcess->ReadOperationCount.QuadPart);
		DbgPrint("Write Operation Count: %llu\n", currentProcess->WriteOperationCount.QuadPart);
		DbgPrint("Other Operation Count: %llu\n", currentProcess->OtherOperationCount.QuadPart);
		DbgPrint("InheritedFromUniqueProcessId:%p;", currentProcess->InheritedFromUniqueProcessId);
		DbgPrint("---------------------------------------------------\n");

		// �ƶ�����һ����Ŀ
		if (currentProcess->NextEntryOffset == 0) {
			break; // û����һ����Ŀ
		}
		offset += currentProcess->NextEntryOffset;
	}
}



extern "C" NTSTATUS GetFilePathFromProcessId(
	IN HANDLE ProcessId,
	OUT PUNICODE_STRING FilePath
)
{
	NTSTATUS status;
	PFILE_NAME_INFORMATION fileNameInfo;
	ULONG length = 0;
	PDEVICE_OBJECT deviceObject = NULL;
	PFILE_OBJECT fileObject = NULL;
	PVOID buffer = NULL;
	HANDLE processHandle;
	OBJECT_ATTRIBUTES objectAttributes;
	CLIENT_ID clientId;

	// ��ʼ�� CLIENT_ID �� OBJECT_ATTRIBUTES
	InitializeObjectAttributes(&objectAttributes, NULL, 0, NULL, NULL);
	clientId.UniqueProcess = ProcessId;
	clientId.UniqueThread = NULL;

	// �򿪽��̾��
	status = NtOpenProcess(&processHandle, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, &objectAttributes, &clientId);
	if (!NT_SUCCESS(status)) {
		DbgPrint("GetFilePathFromProcessId: NtOpenProcess failed with status 0x%X\n", status);
		return status;
	}

	// ��ȡ�ļ�����
	status = ObReferenceObjectByHandle(processHandle, FILE_READ_DATA, *IoFileObjectType, KernelMode, (PVOID*)&fileObject, NULL);
	if (!NT_SUCCESS(status)) {
		DbgPrint("GetFilePathFromProcessId: ObReferenceObjectByHandle failed with status 0x%X\n", status);
		NtClose(processHandle);
		return status;
	}

	// ��ȡ�豸����
	deviceObject = IoGetRelatedDeviceObject(fileObject);
	if (!deviceObject) {
		DbgPrint("GetFilePathFromProcessId: IoGetRelatedDeviceObject failed\n");
		ObDereferenceObject(fileObject);
		NtClose(processHandle);
		return STATUS_UNSUCCESSFUL;
	}

	// �����ڴ��������ļ�����Ϣ
	length = sizeof(FILE_NAME_INFORMATION) + MAX_PATH * sizeof(WCHAR);
	buffer = ExAllocatePoolWithTag(PagedPool, length, 'tag');
	if (!buffer) {
		DbgPrint("GetFilePathFromProcessId: ExAllocatePoolWithTag failed\n");
		ObDereferenceObject(fileObject);
		NtClose(processHandle);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	// ��ѯ�ļ���
	status = IoQueryFileDosDeviceName(fileObject, (POBJECT_NAME_INFORMATION*)buffer);
	if (NT_SUCCESS(status)) {
		RtlCopyUnicodeString(FilePath, (PUNICODE_STRING)buffer);
	}
	else {
		DbgPrint("GetFilePathFromProcessId: IoQueryFileDosDeviceName failed with status 0x%X\n", status);
	}

	// �ͷ���Դ
	ExFreePoolWithTag(buffer, 'tag');
	ObDereferenceObject(fileObject);
	NtClose(processHandle);

	return status;
}





extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject,_In_ PUNICODE_STRING RegistryPath)
{

	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);
	PVOID processInfoBuffer = NULL;
	ULONG processInfoLength = 0;
	NTSTATUS status = STATUS_SUCCESS;

	DriverObject->DriverUnload = DriverUnload;
	//return STATUS_INVALID_ADDRESS;

	DbgPrint("DriverEntry\n");
	UNICODE_STRING deviceStr = { 0 };
	UNICODE_STRING symblicStr = { 0 };
	PDEVICE_OBJECT deviceObj = NULL;
	RtlInitUnicodeString(&deviceStr, L"\\Device\\ExampleDevice");
    RtlInitUnicodeString(&symblicStr, L"\\DosDevices\\ExampleDevice");
	CreateDevice(DriverObject,&deviceStr, &symblicStr,&deviceObj);
	setMemoryProtect();
	ListProcessTypeCallbacks();

	ListInfo li;
	//li.PrintObjectTypeInfo();
	li.PrintObTypeIndexList(li.GetObTypeIndexTable());
	ListProcessTypeCallbacks();


	DbgPrint("QuerySystem\n");
	if (!NT_SUCCESS( status = QuerySystemProcessInformation(&processInfoBuffer, &processInfoLength)))
	{
		return status;
	}

	PrintSystemProcessInformation((PSYSTEM_PROCESS_INFORMATION)processInfoBuffer, processInfoLength);

	UNICODE_STRING filePath;
	SYSTEM_PROCESS_INFORMATION spi = { 0 };
	spi = *(PSYSTEM_PROCESS_INFORMATION)processInfoBuffer;
	
	ULONG offset = 0;
	while (offset < processInfoLength)
	{
		PSYSTEM_PROCESS_INFORMATION currentProcess = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)processInfoBuffer + offset);

		if (currentProcess->UniqueProcessId)
		{	
			if (NT_SUCCESS(GetFilePathFromProcessId(((PSYSTEM_PROCESS_INFORMATION)currentProcess)->UniqueProcessId,&filePath)))
			{
				DbgPrint("ProcessPath: %wZ\n",filePath);
			}
			else
			{
				DbgPrint("Get Process error\n", filePath);
			}
		}

		if (currentProcess->NextEntryOffset == 0)
			break;
		offset += currentProcess->NextEntryOffset;
	}



	return STATUS_SUCCESS;
}
