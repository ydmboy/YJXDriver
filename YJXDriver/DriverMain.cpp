#include <wdm.h>
#define IOCTL_EXAMPLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)


extern "C"
NTSTATUS DriverIoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS status = STATUS_SUCCESS;

    UNREFERENCED_PARAMETER(DeviceObject);

    switch (stack->Parameters.DeviceIoControl.IoControlCode)
    {
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
					_In_ PUNICODE_STRING pUnicode_Device_String ,
				_In_ PUNICODE_STRING pUnicode_SymbolicLinkName,	
			_Outptr_ PDEVICE_OBJECT * DeviceObject)
{

	NTSTATUS status = STATUS_SUCCESS;

	status = IoCreateDevice(DriverObject, 0, pUnicode_Device_String, FILE_DEVICE_UNKNOWN, 0, FALSE, DeviceObject);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("Failed to create devcie:%08x\n",status);
		return status;
	}
	status = IoCreateSymbolicLink();
	return status;
}


extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject,_In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);
	DriverObject->DriverUnload = DriverUnload;

	return STATUS_SUCCESS;
}
