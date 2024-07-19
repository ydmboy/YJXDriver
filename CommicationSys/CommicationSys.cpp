#include <windows.h>
#include <iostream>

#define IOCTL_EXAMPLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

void SendIoctlExample()
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

	DWORD bytesReturned;
	char x[] = "ydmboy";
	char y[20] = { 0 };
	BOOL success = DeviceIoControl(
		hDevice,
		IOCTL_EXAMPLE,
		x,strlen(x),   // No input buffer
		y, sizeof(y),   // No output buffer
		&bytesReturned,
		nullptr
	);

	if (success)
	{

		std::cout << "IOCTL_EXAMPLE request sent successfully." <<"Input:"<<x<< std::endl;
	}
	else
	{
		std::cerr << "Failed to send IOCTL_EXAMPLE request: " << GetLastError() << std::endl;
	}
	std::cout << "OUTPUT:"<<y<<std::endl;
	std::cout << "RtNum:" << bytesReturned << std::endl;
	CloseHandle(hDevice);
}

int main()
{
	SendIoctlExample();
	return 0;
}

