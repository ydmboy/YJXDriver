//#include <ntddk.h>
#include <wdm.h>


#define DRIVER_NAME L"YDMDriver"

#define DRIVER_DEVICE_NAME L"\\device\\"##DRIVER_NAME
#define DRIVER_SYM_NAME L"\\??\\" ## DRIVER_DEVICE_NAME
#define DRIVER_SONG L"ASD"##L"bcd"

// 驱动程序的卸载函数
VOID DriverUnload(_In_ PDRIVER_OBJECT DriverObject)
{
	UNREFERENCED_PARAMETER(DriverObject);
	DbgPrint("[HelloDriver] Driver Unloaded\n");
}

// 驱动程序的入口点
NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);

	DbgPrint("[HelloDriver] Driver Loaded\n");
	DbgPrint("%ws",DRIVER_SONG);


	// 设置卸载函数
	DriverObject->DriverUnload = DriverUnload;

	return STATUS_SUCCESS;
}