#pragma once

class DriverComm
{
private:
	std::shared_ptr<spdlog::logger>  m_file_logger = NULL;
public:
	DriverComm();
	void SendIoctlExample();
	void SendFlushList();
};

