#pragma once
#include "./include/spdlog/spdlog.h"
#include "./include/spdlog/sinks/basic_file_sink.h"

class DriverComm
{
private:
	std::shared_ptr<spdlog::logger>  m_file_logger = NULL;
public:
	DriverComm();
	void SendIoctlExample();
};

