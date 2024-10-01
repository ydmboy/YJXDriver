#pragma once

template <typename... Args>
void Error(const char* format, Args&&... args)
{
	// 获取或创建一个名为 "logger_a" 的基本文件日志器

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

	// 使用 SPDLog 的格式化功能，记录错误消息
	file_logger->error(format, std::forward<Args>(args)...);
	
}

