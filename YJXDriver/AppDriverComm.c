#include "AppDriverComm.h"
#include "MacroUtils.h"

NTSTATUS DispatchRoutineBuffer(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	DbgPrint("disPatchh Routine Buffer\n");
	DbgPrint("Stack MajorFunction:%d\n",stack->MajorFunction);
	DbgPrint("IRP_MJ:%d\n",stack->Parameters.DeviceIoControl.IoControlCode);
	//DbgPrint(" DispatchRoutineBuffer:%d\n",stack->MajorFunction);
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
		// ´¦Àí I/O ¿ØÖÆÂë
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
		case IOCTL_REFRESH_INFO:
		{

			//PKLDR_DATA_TABLE_ENTRY kPdrA = CONTAINING_RECORD(PsLoadedModuleList,KLDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
			//PKLDR_DATA_TABLE_ENTRY kPdr = kPdrA->InLoadOrderLinks.Flink;
			//while (kPdr != kPdrA)
			//{
			//	PrintKLDRDataTableEntry(kPdr);
			//	kPdr = kPdr->InLoadOrderLinks.Flink;
			//}
			DbgPrint("IOCTL_REFRESH_INFO\n");
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

NTSTATUS DispatchRoutineDirect(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	//DbgPrint("IOCTL_EXAMPLE:%d",IOCTL_EXAMPLE);
	DbgPrint("DispatchRoutineDirect:%d\n",stack->MajorFunction);
	NTSTATUS status = STATUS_SUCCESS;
	VOID* inputBuffer = NULL;
	VOID* outputBuffer = NULL;
	if (Irp->MdlAddress)
	{
		inputBuffer = MmGetSystemAddressForMdl(Irp->MdlAddress);

	}
	else
	{
		status = STATUS_INVALID_DEVICE_REQUEST;
	}
	switch (stack->Parameters.DeviceIoControl.IoControlCode)
	{
		case IOCTL_EXAMPLE:
		{
			if (inputBuffer)
				DbgPrint("DispatchRoutineDirect:%s",inputBuffer);
			break;
		}
		default:
		{
			break;
		}
	}
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;

}
