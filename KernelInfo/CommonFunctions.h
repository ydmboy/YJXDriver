#pragma once

template <typename... Args>
void Error(const char* format, Args&&... args)
{
	// ��ȡ�򴴽�һ����Ϊ "logger_a" �Ļ����ļ���־��
	auto logger_a = spdlog::basic_logger_mt("logger_a", LOG_FILE_DIR);

	// ������־����Ϊ���󼶱�
	logger_a->set_level(spdlog::level::err);

	// ʹ�� SPDLog �ĸ�ʽ�����ܣ���¼������Ϣ
	logger_a->error(format, std::forward<Args>(args)...);
}

