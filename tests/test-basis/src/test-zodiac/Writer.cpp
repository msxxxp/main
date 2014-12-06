#include <basis/sys/logger.hpp>

#include "test-zodiac.hpp"
Writer::Writer(MessageQueue<MessageType>& que, int num) :
	m_que(que),
	m_thread(nullptr),
	m_num(num)
{
}

void Writer::run()
{
	if (!m_sending)
		m_sending = CreateEvent(NULL, TRUE, TRUE, NULL);
	m_thread = CreateThread(NULL, 0, thread_func, this, 0, NULL);
}

Writer::operator HANDLE() const
{
	return m_thread;
}

void Writer::sleep()
{
	ResetEvent(m_sending);
}

void Writer::sending()
{
	SetEvent(m_sending);
}

DWORD WINAPI Writer::thread_func(PVOID data)
{
	Writer * th = static_cast<Writer*>(data);
	LogConsoleReport(-1, L"Writer::run(%d, %p)\n", th->m_num, th->m_thread);
	for (int i = 0;; ++i) {
		DWORD res = WaitForSingleObject(m_sending, INFINITE);
		if (res == WAIT_OBJECT_0) {
			int pr = rand() % 100;
			wchar_t buf[MAX_PATH];
			snwprintf(buf, sizeof(buf), L"Writer(%d, %p) message: %d prio: %d", th->m_num, th->m_thread, i, pr);
			MessageType msg(buf);
			RetCodes ret = th->m_que.put(msg, pr);
			switch (ret) {
				case RetCodes::OK:
					LogConsoleReport(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"\tWriter: message '%s' has put in queue\n", msg.c_str());
					break;
				case RetCodes::HWM:
					LogConsoleReport(FOREGROUND_INTENSITY | FOREGROUND_RED, L"\tWriter(%d, %p) tried to put on HWM\n", th->m_num, th->m_thread);
					break;
				case RetCodes::NO_SPACE:
					LogConsoleReport(FOREGROUND_INTENSITY | FOREGROUND_RED, L"\tWriter(%d, %p) tried to put on NO_SPACE\n", th->m_num, th->m_thread);
					break;
				case RetCodes::STOPPED:
					LogConsoleReport(FOREGROUND_INTENSITY | FOREGROUND_RED, L"\tWriter(%d, %p) tried to put on STOPPED, exiting!\n", th->m_num, th->m_thread);
					return (DWORD)RetCodes::STOPPED;
			}
			Sleep(200);
		}
	}
	return 0;
}

HANDLE Writer::m_sending = NULL;
