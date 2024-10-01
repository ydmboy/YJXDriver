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
	//}
	//// ������־����Ϊ���󼶱�
	spdlog::set_level(spdlog::level::err);
	
	// ʹ�� SPDLog �ĸ�ʽ�����ܣ���¼������Ϣ
	spdlog::error(format, std::forward<Args>(args)...);
	
}

