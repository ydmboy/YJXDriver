#include "pch.h"
#include "DriverComm.h"
#include <winioctl.h>
//#include "../YJXDriver/MacroUtils.h"


DriverComm::DriverComm()
{
}


#define IOCTL_EXAMPLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_REFRESH_INFO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

void DriverComm::SendFlushList()
{
	//afxMessageBox("SendflushList");
	HANDLE hDevice = CreateFile(
		L"\\\\.\\ExampleDevice",   // 与驱动程序中的符号链接名称匹配
		GENERIC_READ | GENERIC_WRITE,
		0,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
		);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		Error("ERROR:INVALID_HANDLE_VALUE,\tFILE:{},\tLine:{}\n", __FILE__, __LINE__);
		return;
	}
	DWORD bytesReturned;
	char x[] = "ydmboy11";
	char y[20] = { 0 };
	BOOL success = DeviceIoControl(
		hDevice,
		IOCTL_REFRESH_INFO,
		x,strlen(x),   // No input buffer
		y, sizeof(y),   // No output buffer
		&bytesReturned,
		(LPOVERLAPPED) NULL
	);

	if (success)
	{
		Error("IOCTL_EXAMPLE request sent successfully.input:%d", x);
	}
	else
	{
		Error("Failed to send IOCTL_EXAMPLE request: \n" );
	}
	CloseHandle(hDevice);
	AfxMessageBox(_T("afxMessageBox"));
}

void DriverComm::SendIoctlExample()
{
	HANDLE hDevice = CreateFile(
		L"\\\\.\\ExampleDevice",   // 与驱动程序中的符号链接名称匹配
		GENERIC_READ | GENERIC_WRITE,
		0,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
	);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		Error("ERROR:INVALID_HANDLE_VALUE,\tFILE:{},\tLine:{}\n", __FILE__, __LINE__);
		return;
	}
	DWORD bytesReturned;
	char x[] = "ydmboy11";
	char y[20] = { 0 };
	BOOL success = DeviceIoControl(
		hDevice,
		IOCTL_EXAMPLE,
		x,strlen(x),   // No input buffer
		y, sizeof(y),   // No output buffer
		&bytesReturned,
		(LPOVERLAPPED) NULL
	);

	if (success)
	{
		Error("IOCTL_EXAMPLE request sent successfully.input:%d", x);
	}
	else
	{
		Error("Failed to send IOCTL_EXAMPLE request: \n" );
	}
	CloseHandle(hDevice);
}
