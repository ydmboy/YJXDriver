#include <windows.h>
#include <iostream>

#define IOCTL_EXAMPLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#pragma comment(lib,"./lib/spdlogd.lib")

#include "include/spdlog/spdlog.h"
#include "include/spdlog/sinks/basic_file_sink.h"


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

int main()
{
    auto logger = spdlog::basic_logger_mt("basic_logger", "logs/logfile.txt");

    // 设置日志级别
    logger->set_level(spdlog::level::info);

    // 写日志
    logger->info("Welcome to spdlog!");
    logger->error("This is an error message");

    // 清理所有日志器
    spdlog::drop_all();
	return 0;
}

