#pragma once

template <typename... Args>
void Error(const char* format, Args&&... args)
{
	// ��ȡ�򴴽�һ����Ϊ "logger_a" �Ļ����ļ���־��

	//auto it = loggers.find(LOG_FILE_DIR+"logger_a");
	//auto logger_a;
	//if (it != loggers.end()) {
	//	logger_a = it;
	//}
	//else
	//{
	//	logger_a = spdlog::basic_logger_mt("logger_a", LOG_FILE_DIR);
	//

	static auto file_logger = spdlog::basic_logger_mt("file_logger", "logs/my_log.log");
    spdlog::set_default_logger(file_logger);

	// ʹ�� SPDLog �ĸ�ʽ�����ܣ���¼������Ϣ
	file_logger->error(format, std::forward<Args>(args)...);
	
}

