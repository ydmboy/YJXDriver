#include "AppDriverComm.h"
#include "CommonHeader.h"
#include <minwindef.h>


void DriverUnload(PDRIVER_OBJECT DriverObject)
{
    //UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\DosDevices\\ExampleDevice");
    //IoDeleteSymbolicLink(&SymbolicLinkName);
	UNICODE_STRING smLinkCode = RTL_CONSTANT_STRING(DRIVER_SYM_NAME);
	IoDeleteSymbolicLink(&smLinkCode);
	PDEVICE_OBJECT pdObj = DriverObject->DeviceObject;
	UnistallAllProcessType();
	if (pdObj)
	{
		IoDeleteDevice(DriverObject->DeviceObject);
		pdObj->NextDevice;
	}
}


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


NTSTATUS CreateDevice(_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING pUnicode_Device_String,
	_In_ PUNICODE_STRING pUnicode_SymbolicLinkName,
	_Outptr_ PDEVICE_OBJECT*  DeviceObject)
{

	NTSTATUS status = STATUS_SUCCESS;

	CHECK_STATUS(status ,status =IoCreateDevice(DriverObject, 0, pUnicode_Device_String, FILE_DEVICE_UNKNOWN, 0, FALSE, DeviceObject));
	if (NT_SUCCESS(status))
	{
		DbgPrint("CreateDevice successfully\n");
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchRoutineBuffer;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchRoutineBuffer;
	(*DeviceObject)->Flags &= ~DO_DEVICE_INITIALIZING;
	(*DeviceObject)->Flags |= DO_BUFFERED_IO;

	CHECK_STATUS(status,status = IoCreateSymbolicLink(pUnicode_SymbolicLinkName, pUnicode_Device_String));
	if (NT_SUCCESS(status))
	{
		DbgPrint("CreateDevice symblic successfully\n");
	}


	//EXCEPTION_ACCESS_VIOLATION


	return status;

CLEANUP:
	
	
	if (DeviceObject && *DeviceObject)
	{
		IoDeleteDevice(*DeviceObject);
		*DeviceObject = NULL;
	}



	return status;
}


//void IoDeleteDevice(
//  [in] PDEVICE_OBJECT DeviceObject
//);
//
//NTSTATUS  DeleteDevice()

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
    ULONG bufferLength = 0x10000; // 初始缓冲区大小
    PVOID buffer = NULL;

    // 分配内存给缓冲区
    buffer = ExAllocatePoolWithTag(NonPagedPool, bufferLength, 'Proc');
    if (buffer == NULL)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    while (TRUE)
    {
        // 调用 ZwQuerySystemInformation 获取系统进程信息
        status = ZwQuerySystemInformation(SystemProcessInformation, buffer, bufferLength, &bufferLength);

        if (status == STATUS_INFO_LENGTH_MISMATCH)
        {
            // 缓冲区不够大，重新分配更大的缓冲区
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
        // 清理缓冲区
        ExFreePool(buffer);
    }

    return status;
}

 VOID FreeSystemProcessInformation(PVOID ProcessInfoBuffer)
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

	// 用于遍历整个缓冲区
	ULONG offset = 0;

	while (offset < BufferLength) {
		PSYSTEM_PROCESS_INFORMATION currentProcess = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)ProcessInfo + offset);

		// 打印所有字段
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

		// 移动到下一个条目
		if (currentProcess->NextEntryOffset == 0) {
			break; // 没有下一个条目
		}
		offset += currentProcess->NextEntryOffset;
	}
}


// funcName: GetFilePathFromProcessId
// unavailable  
 NTSTATUS GetFilePathFromProcessId(
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
	IO_STATUS_BLOCK IoStatusBlock = { 0 };

	// 初始化 CLIENT_ID 和 OBJECT_ATTRIBUTES
	InitializeObjectAttributes(&objectAttributes, NULL, 0, NULL, NULL);
	clientId.UniqueProcess = ProcessId;
	clientId.UniqueThread = NULL;

	// 打开进程句柄
	//status = NtOpenProcess(&processHandle, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, &objectAttributes, &clientId);
	//if (!NT_SUCCESS(status)) {
	//	DbgPrint("GetFilePathFromProcessId: NtOpenProcess failed with status 0x%X\n", status);
	//	return status;
	//}

	status = NtOpenFile(&processHandle, GENERIC_READ, &objectAttributes,&IoStatusBlock,FILE_SHARE_READ| FILE_SHARE_WRITE ,0);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("GetFilePathFromProcessId: NtOpenFile failed with status 0x%X\n", status);
		return status;
	}

	// 获取文件对象
	status = ObReferenceObjectByHandle(processHandle, FILE_READ_DATA, *IoFileObjectType, KernelMode, (PVOID*)&fileObject, NULL);
	if (!NT_SUCCESS(status)) {
		DbgPrint("GetFilePathFromProcessId: ObReferenceObjectByHandle failed with status 0x%X\n", status);
		NtClose(processHandle);
		return status;
	}

	// 获取设备对象
	deviceObject = IoGetRelatedDeviceObject(fileObject);
	DbgPrint("1.deviceObject:%llx", deviceObject);
	if (!deviceObject) {
		DbgPrint("GetFilePathFromProcessId: IoGetRelatedDeviceObject failed\n");
		ObDereferenceObject(fileObject);
		NtClose(processHandle);
		return STATUS_UNSUCCESSFUL;
	}

	// 分配内存来保存文件名信息
	length = sizeof(FILE_NAME_INFORMATION) + MAX_PATH * sizeof(WCHAR);
	buffer = ExAllocatePoolWithTag(PagedPool, length, 'tag');
	if (!buffer) {
		DbgPrint("GetFilePathFromProcessId: ExAllocatePoolWithTag failed\n");
		ObDereferenceObject(fileObject);
		NtClose(processHandle);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	// 查询文件名
	CHECK_STATUS(status, IoQueryFileDosDeviceName(fileObject, (POBJECT_NAME_INFORMATION*)buffer));
	//status = IoQueryFileDosDeviceName(fileObject, (POBJECT_NAME_INFORMATION*)buffer);
	if (NT_SUCCESS(status)) {
		RtlCopyUnicodeString(FilePath, (PUNICODE_STRING)buffer);
	}
	else {
		DbgPrint("GetFilePathFromProcessId: IoQueryFileDosDeviceName failed with status 0x%X\n", status);
	}

	// 释放资源
CLEANUP:
	ExFreePoolWithTag(buffer, 'tag');
	ObDereferenceObject(fileObject);
	NtClose(processHandle);

	return status;
}

 NTSTATUS GetVolumeDeviceObject(PCWSTR VolumeName, PDEVICE_OBJECT* VolumeDeviceObject)
{
	NTSTATUS status = STATUS_SUCCESS;
	PFILE_OBJECT fileObject = NULL;
	PDEVICE_OBJECT deviceObject = NULL;
	UNICODE_STRING volumeName = { 0 };
	RtlInitUnicodeString(&volumeName, VolumeName);
	status = IoGetDeviceObjectPointer(&volumeName, FILE_READ_ATTRIBUTES, &fileObject, &deviceObject);
	if (NT_SUCCESS(status))
	{
		*VolumeDeviceObject = IoGetAttachedDeviceReference(deviceObject);
		ObDereferenceObject(fileObject);
		ObDereferenceObject(deviceObject);
	}
	else
	{
		*VolumeDeviceObject = NULL;
	}
	return status;
	
}

 NTSTATUS GetProcessImagePath(
    IN HANDLE ProcessId,
    OUT PUNICODE_STRING FilePath
)
{
    NTSTATUS status;
    HANDLE processHandle;
    OBJECT_ATTRIBUTES objectAttributes;
    CLIENT_ID clientId;
    ULONG returnedLength;
    PVOID buffer;

    // 初始化 CLIENT_ID 和 OBJECT_ATTRIBUTES
    InitializeObjectAttributes(&objectAttributes, NULL, 0, NULL, NULL);
    clientId.UniqueProcess = ProcessId;
    clientId.UniqueThread = NULL;

    // 打开进程句柄
    status = NtOpenProcess(&processHandle, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, &objectAttributes, &clientId);
    if (!NT_SUCCESS(status)) {
        DbgPrint("GetProcessImagePath: NtOpenProcess failed with status 0x%X\n", status);
        return status;
    }

    // 分配内存以存储文件路径信息
    buffer = ExAllocatePoolWithTag(PagedPool, 512, 'tag'); // 假设路径不超过 512 字节
	if (!buffer) {
		DbgPrint("GetProcessImagePath: ExAllocatePoolWithTag failed\n");
		NtClose(processHandle);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	else
		RtlZeroMemory(buffer, 512);

    // 获取进程映像文件路径
    status = NtQueryInformationProcess(
        processHandle,
        ProcessImageFileName, // 获取映像文件名
        buffer,
        512,
        &returnedLength
    );

    if (NT_SUCCESS(status)) {

		FilePath->Length = (USHORT)returnedLength;
		FilePath->MaximumLength = (USHORT)returnedLength;
		FilePath->Buffer = (PWSTR)ExAllocatePoolWithTag(PagedPool,512,'tag');
		RtlCopyUnicodeString(FilePath, (PUNICODE_STRING)buffer);
		//RtlVolumeDeviceToDosName()



		//NTSYSAPI NTSTATUS RtlVolumeDeviceToDosName(
		//	  [in]  PVOID           VolumeDeviceObject,
		//	  [out] PUNICODE_STRING DosName);

    } else {
        DbgPrint("GetProcessImagePath: ZwQueryInformationProcess failed with status 0x%X\n", status);
		//ExFreePoolWithTag(buffer, 'tag');
    }

    // 释放资源
    ExFreePoolWithTag(buffer, 'tag');
    NtClose(processHandle);

    return status;
}



NTSTATUS ConvertKernelToUsrPath()
{
	NTSTATUS status = STATUS_SUCCESS;
	return status;
}


NTSTATUS GetSystemVolumeLetter(PUNICODE_STRING SystemVolumeLetter)
{
    NTSTATUS status;
    UNICODE_STRING kernelPath;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;
    HANDLE fileHandle;
    PFILE_OBJECT fileObject;
    PDEVICE_OBJECT deviceObject;
    UNICODE_STRING dosName;

    RtlInitUnicodeString(&kernelPath, L"\\SystemRoot\\System32\\ntoskrnl.exe");

    InitializeObjectAttributes(&objectAttributes, &kernelPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

    status = ZwCreateFile(
        &fileHandle,
        FILE_READ_ATTRIBUTES,
        &objectAttributes,
        &ioStatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ,
        FILE_OPEN,
        FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
        NULL,
        0
    );

    if (NT_SUCCESS(status)) {
        status = ObReferenceObjectByHandle(
            fileHandle,
            FILE_READ_ATTRIBUTES,
            *IoFileObjectType,
            KernelMode,
            (PVOID*)&fileObject,
            NULL
        );

        if (NT_SUCCESS(status)) {
            deviceObject = IoGetRelatedDeviceObject(fileObject);
			if(!deviceObject)
				DRIVER_ERROR("Error: %08x\n FILE:", status);
            RtlInitUnicodeString(&dosName, NULL);
            dosName.Buffer = (PWCHAR)ExAllocatePoolWithTag(NonPagedPool, 1024, 'tag1');
            dosName.Length = 0;
            dosName.MaximumLength = 1024;

            status = RtlVolumeDeviceToDosName(deviceObject, &dosName);

            if (NT_SUCCESS(status)) {
                *SystemVolumeLetter = dosName;  // 保存系统盘符
            } else {
				DRIVER_ERROR("Error: %08x\n FILE:", status);
                ExFreePool(dosName.Buffer);  // 如果失败，释放缓冲区
            }

            ObDereferenceObject(fileObject);
        }

        ZwClose(fileHandle);
    }

    return status;
}



NTSTATUS GetDosPathByProcessId(IN ULONG pid,OUT PANSI_STRING pAnsiNtPath)
{
	/*
	1.根据PID获取进程句柄
	2.使用ZwQueryInformationProcess 传入HANDLE 使用27号功能获取路径
	*/
	HANDLE hProcess = 0;
	CLIENT_ID cid;
	OBJECT_ATTRIBUTES obj;
	NTSTATUS ntStatus;
	ULONG RetLength = 0;
	PVOID pBuffer = NULL;
	HANDLE hFile;
	IO_STATUS_BLOCK iostu;
	PVOID FileObject = NULL;
	PFILE_OBJECT pMyFileObject = NULL;
	UNICODE_STRING DosName;
	UNICODE_STRING FunllPath;
	
	if (NtQueryInformationProcess == NULL)
		return STATUS_UNSUCCESSFUL;
 
	cid.UniqueProcess =(HANDLE)pid;
	cid.UniqueThread = 0;
	InitializeObjectAttributes(&obj, 0, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, 0, 0);
	ntStatus = ZwOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &obj, &cid);
	if (!NT_SUCCESS(ntStatus))
		return STATUS_UNSUCCESSFUL;
	//使用27 号功能遍历
	
	ntStatus = NtQueryInformationProcess(hProcess, ProcessImageFileName, NULL, 0, &RetLength);
	if (STATUS_INFO_LENGTH_MISMATCH != ntStatus)
		return STATUS_UNSUCCESSFUL;
 
	//申请内存继续获取.
	pBuffer = ExAllocatePoolWithTag(PagedPool, RetLength, 'niBI');
	if (NULL == pBuffer)
		return STATUS_UNSUCCESSFUL;
	//重新调用获取.
	
	ntStatus = NtQueryInformationProcess(hProcess, ProcessImageFileName, pBuffer, RetLength, &RetLength);
	if (!NT_SUCCESS(ntStatus))
	{
		if (NULL != pBuffer)
		{
			ExFreePoolWithTag(pBuffer, 'niBI');
		}
		return STATUS_UNSUCCESSFUL;
	}
	DbgPrint("NtQueryInformationProcess-dosPath:%wZ",(PUNICODE_STRING)pBuffer);
	
	//开始转化路径
	InitializeObjectAttributes(&obj, (PUNICODE_STRING)pBuffer, OBJ_KERNEL_HANDLE, 0, 0);
	ntStatus = ZwOpenFile(
		&hFile,
		GENERIC_READ,
		&obj,
		&iostu,
		FILE_SHARE_READ| FILE_SHARE_WRITE , 
		0);
	if (!NT_SUCCESS(ntStatus))
	{
		if (NULL != pBuffer)
		{
			ExFreePoolWithTag(pBuffer, 'niBI');
		}
		ZwClose(hFile);
		return STATUS_UNSUCCESSFUL;
	}
 
	//获得文件对象
	ntStatus = ObReferenceObjectByHandle(
		hFile, 
		GENERIC_ALL, 
		*IoFileObjectType, 
		KernelMode,
		&FileObject,
		NULL);
	
	if (!NT_SUCCESS(ntStatus))
	{
		if (NULL != pBuffer)
		{
			ExFreePoolWithTag(pBuffer, 'niBI');
		}
		ntStatus = ObDereferenceObject(FileObject);
		ZwClose(hFile);
		return STATUS_UNSUCCESSFUL;
	}
	pMyFileObject = (PFILE_OBJECT)FileObject;
	if (NULL == pMyFileObject)
	{
		if (NULL != pBuffer)
		{
			ExFreePoolWithTag(pBuffer, 'niBI');
		}
		ntStatus = ObDereferenceObject(FileObject);
		ZwClose(hFile);
		return STATUS_UNSUCCESSFUL;
		
	}
	
	DbgPrint("2.deviceObject:%llx", pMyFileObject->DeviceObject);

	//通过 RtlVolumeDeviceToDosName 获取Dos路径 也即是C: D: 等盘符
	RtlVolumeDeviceToDosName(pMyFileObject->DeviceObject,&DosName);
 
	//获得路径直接直接拼接即可.
 
	FunllPath.MaximumLength = pMyFileObject->FileName.MaximumLength + DosName.MaximumLength;
	FunllPath.Length = pMyFileObject->FileName.Length + DosName.Length;
	FunllPath.Buffer = (PWCH)ExAllocatePoolWithTag(NonPagedPool, FunllPath.MaximumLength, 0);
 
	//拼接路径
	RtlCopyUnicodeString(&FunllPath, &DosName);//得到C:
	RtlAppendUnicodeStringToString(&FunllPath, &pMyFileObject->FileName);//得到C:\\xxx路径,转为Asii
	RtlUnicodeStringToAnsiString(pAnsiNtPath, &FunllPath,TRUE); //RtlFreeAnsiString  要释放空间.
 
	
	ExFreePool(FunllPath.Buffer); //因为传出自动为其分配了内存所以这个进行谁放
	if (NULL != pBuffer)
	{
		ExFreePoolWithTag(pBuffer, 'niBI');
	}
 
	ntStatus = ObDereferenceObject(FileObject);
	ZwClose(hFile);
	return STATUS_SUCCESS;
}



NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject,_In_ PUNICODE_STRING RegistryPath)
{

	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);
	PVOID processInfoBuffer = NULL;
	ULONG processInfoLength = 0;
	NTSTATUS status = STATUS_SUCCESS;

	DriverObject->DriverUnload = DriverUnload;
	//return STATUS_INVALID_ADDRESS;

	PVOID pBuffer = ExAllocatePoolWithTag(PagedPool, 1024, 'niBI');

	DbgPrint("DriverEntry\n");
	UNICODE_STRING deviceStr = { 0 };
	UNICODE_STRING symblicStr = { 0 };
	PDEVICE_OBJECT deviceObj = NULL;

	//RtlInitUnicodeString(&deviceStr, L"\\Device\\ExampleDevice");
	//RtlInitUnicodeString(&symblicStr, L"\\DosDevices\\ExampleDevice");


	RtlInitUnicodeString(&deviceStr, DRIVER_DEVICE_NAME);
    RtlInitUnicodeString(&symblicStr, DRIVER_SYM_NAME);

	DbgPrint("DRIVER_DEVICE_NAME:%ws",DRIVER_DEVICE_NAME);
	DbgPrint("DRIVER_SYM_NAME:%ws",DRIVER_SYM_NAME);

	CreateDevice(DriverObject,&deviceStr, &symblicStr,&deviceObj);
	



	
	//setMemoryProtect();

	//ListProcessTypeCallbacks();

	//ListInfo li;
	////li.PrintObjectTypeInfo();
	//li.PrintObTypeIndexList(li.GetObTypeIndexTable());
	//ListProcessTypeCallbacks();


	/*DbgPrint("QuerySystem\n");
	if (!NT_SUCCESS( status = QuerySystemProcessInformation(&processInfoBuffer, &processInfoLength)))
	{
		return status;
	}

	PrintSystemProcessInformation((PSYSTEM_PROCESS_INFORMATION)processInfoBuffer, processInfoLength);

	UNICODE_STRING filePath = { 0 };
	SYSTEM_PROCESS_INFORMATION spi = { 0 };
	spi = *(PSYSTEM_PROCESS_INFORMATION)processInfoBuffer;
	ANSI_STRING AnsiNtPath;
	UNICODE_STRING uniDosPath = { 0 };
	ULONG offset = 0;

	while (offset < processInfoLength)
	{
		PSYSTEM_PROCESS_INFORMATION currentProcess = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)processInfoBuffer + offset);

		if (currentProcess->UniqueProcessId)
		{	
			if (NT_SUCCESS(GetProcessImagePath(((PSYSTEM_PROCESS_INFORMATION)currentProcess)->UniqueProcessId,&filePath)))
			{
				DbgPrint("ProcessPath: %wZ\n",filePath);
				if (NT_SUCCESS(status = GetDosPathByProcessId((ULONG)currentProcess->UniqueProcessId, &AnsiNtPath)))
					DbgPrint("ProcessDirectory:%s", AnsiNtPath.Buffer);
				else
					DbgPrint("GetDosPathByProcessId Error\n");

				if (NT_SUCCESS(status = GetFilePathFromProcessId(currentProcess->UniqueProcessId, &uniDosPath)))
					DbgPrint("ProcessDirectory:%wZ", uniDosPath);
				else
					DbgPrint("GetFilePathFromProcessId Error\n");

			}
			else
			{
				DbgPrint("Get Process error\n", filePath);
			}
		}

		if (filePath.Buffer)
		{
			ExFreePoolWithTag(filePath.Buffer, 'tag');
			filePath.Buffer = NULL;
		}
		if (currentProcess->NextEntryOffset == 0)
			break;
		offset += currentProcess->NextEntryOffset;
	}
*/



CLEANUP:
	if (processInfoBuffer)
		ExFreePool(processInfoBuffer);


	return STATUS_SUCCESS;
}
