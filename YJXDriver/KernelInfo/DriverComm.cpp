#include "pch.h"
#include "DriverComm.h"
#include <winioctl.h>
#define IOCTL_EXAMPLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

DriverComm::DriverComm()
{

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
		std::cerr << "Failed to open device: " << GetLastError() << std::endl;
		return;
	}
	return;
	CloseHandle(hDevice);
	return;
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
		std::cout << "IOCTL_EXAMPLE request sent successfully." <<"Input:"<<x<< std::endl;
		std::cout << "SizeOfX" <<sizeof(y)<< std::endl;
	}
	else
	{
		std::cerr << "Failed to send IOCTL_EXAMPLE request: " << GetLastError() << std::endl;
	}
	printf("Return:%d",bytesReturned);
	std::cout << "OUTPUT:"<<y<<std::endl;
	std::cout << "RtNum:" << bytesReturned << std::endl;
	CloseHandle(hDevice);
}
