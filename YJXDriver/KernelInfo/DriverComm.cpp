#include "pch.h"
#include "DriverComm.h"
#include <winioctl.h>


#define IOCTL_EXAMPLE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

DriverComm::DriverComm()
{
	 m_file_logger = spdlog::basic_logger_mt("basic_logger", "logs/logfile.txt");
}

void DriverComm::SendIoctlExample()
{

	auto logger_a = spdlog::basic_logger_mt("logger_a", "logs/module_a.log");
	logger_a->set_level(spdlog::level::info);
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

		spdlog::set_level(spdlog::level::err);
		spdlog::error("INVALID_HANDLE_VALUE\n");
		m_file_logger->info("asd");
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

		auto logger_a = spdlog::basic_logger_mt("logger_a", "logs/module_a.log");
		logger_a->set_level(spdlog::level::info);

		spdlog::set_level(spdlog::level::info);
		spdlog::error("IOCTL_EXAMPLE request sent successfully.input:%d",x);
		spdlog::error("SIZEOF%x", sizeof(y));
		m_file_logger->info("asd");
	}
	else
	{
		spdlog::error("Failed to send IOCTL_EXAMPLE request: \n" );
	}
	CloseHandle(hDevice);
}
