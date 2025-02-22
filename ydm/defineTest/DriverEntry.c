#include <ntddk.h>

#define DEFINE_A L"abc"
#define DEFINE_B L"print" ## DEFINE_A

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
	DbgPrint("%ws",DEFINE_B);

	// 设置卸载函数
	DriverObject->DriverUnload = DriverUnload;

	return STATUS_SUCCESS;
}