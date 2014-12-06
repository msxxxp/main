#include <basis/sys/logger.hpp>

#include "test-zodiac.hpp"
Reader::Reader(MessageQueue<MessageType>& que, int num) :
	m_que(que),
	m_thread(nullptr),
	m_num(num)
{
}

void Reader::run()
{
	m_thread = ::CreateThread(NULL, 0, thread_func, this, 0, NULL);
}

Reader::operator HANDLE() const
{
	return m_thread;
}

void Reader::handle_message(const MessageType& msg)
{
	LogConsoleReport(FOREGROUND_GREEN, L"\tReader::handle_message(%d, %s)\n", m_num, msg.c_str());
}

DWORD WINAPI Reader::thread_func(PVOID data)
{
	Reader* th = static_cast<Reader*>(data);
	LogConsoleReport(-1, L"Reader::run(%d, %p)\n", th->m_num, th->m_thread);
	while (true) {
		MessageType msg;
		RetCodes ret = th->m_que.get(msg);
		switch (ret) {
			case RetCodes::OK:
				th->handle_message(msg);
				break;
			case RetCodes::STOPPED:
				LogConsoleReport(FOREGROUND_INTENSITY | FOREGROUND_RED, L"\tReader(%d, %p) tried to get on STOPPED, exiting!\n", th->m_num, th->m_thread);
				return 0;
			default:
				break;
		}
		Sleep(300);
	}
	return 0;
}
