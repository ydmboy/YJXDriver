#include <wdm.h>
#include "MacroUtils.h"
#define IOCTL_EXAMPLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)



extern "C"
NTSTATUS DispatchRoutine(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS status = STATUS_SUCCESS;

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
            // 处理 IOCTL_EXAMPLE
            status = STATUS_SUCCESS;
			DbgPrint("IRP_MJ_DEVICE_CONTROL_IOCTL_EXAMPLE_STATUS_SUCCESS\n");
            break;

        default:
            status = STATUS_INVALID_DEVICE_REQUEST;
            break;
        }
        Irp->IoStatus.Status = status;
        Irp->IoStatus.Information = 0;
        break;

    default:
        status = STATUS_INVALID_DEVICE_REQUEST;
        Irp->IoStatus.Status = status;
        Irp->IoStatus.Information = 0;
        break;
    }

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

extern "C"
NTSTATUS DriverIoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS status = STATUS_SUCCESS;

    UNREFERENCED_PARAMETER(DeviceObject);

    switch (stack->Parameters.DeviceIoControl.IoControlCode)
    {
		case IRP_MJ_CREATE:
			DbgPrint("IRP_MJ_CREATE \n");
			break;
		case IRP_MJ_CLOSE:
			DbgPrint("IRP_MJ_CLOSE\n");
			break;
        case IOCTL_EXAMPLE:
            KdPrint(("IOCTL_EXAMPLE received\n"));
            break;
        default:
            status = STATUS_INVALID_DEVICE_REQUEST;
            break;
    }

    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

extern "C"
void DriverUnload(PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\DosDevices\\ExampleDevice");
    IoDeleteSymbolicLink(&SymbolicLinkName);
	UNICODE_STRING smLinkCode = RTL_CONSTANT_STRING(L"asd\\asd");
	IoDeleteSymbolicLink(&smLinkCode);
	PDEVICE_OBJECT pdObj = DriverObject->DeviceObject;
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
	_Outptr_ PDEVICE_OBJECT * DeviceObject)
{

	NTSTATUS status = STATUS_SUCCESS;

	//__try
	//{

		CHECK_STATUS(status = IoCreateDevice(DriverObject,0 /*sizeof(DriverObject->DriverExtension)*/, pUnicode_Device_String, FILE_DEVICE_UNKNOWN, 0, FALSE, DeviceObject));
	//}
	//__except(EXCEPTION_EXECUTE_HANDLER )
	//{
	//	DbgPrint("EXCEPTION_EXECUTE_HANDLER");
	//	goto CLEANUP;
	//}
	CHECK_STATUS(status = IoCreateSymbolicLink(pUnicode_SymbolicLinkName, pUnicode_Device_String));
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchRoutine;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchRoutine;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchRoutine;
	(*DeviceObject)->Flags &= ~DO_DEVICE_INITIALIZING;
	return status;

CLEANUP:
	if (DeviceObject && *DeviceObject)
	{
		IoDeleteDevice(*DeviceObject);
		*DeviceObject = NULL;
	}
	return status;
}


extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject,_In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = DriverUnload;
	UNICODE_STRING deviceStr = { 0 };
	UNICODE_STRING symblicStr = { 0 };
	PDEVICE_OBJECT deviceObj = NULL;
	RtlInitUnicodeString(&deviceStr, L"\\Device\\ExampleDevice");
    RtlInitUnicodeString(&symblicStr, L"\\DosDevices\\ExampleDevice");
	CreateDevice(DriverObject,&deviceStr, &symblicStr,&deviceObj);

	return STATUS_SUCCESS;
}
