#include <basis/sys/logger.hpp>

#include <basis/simstd/vector>

#include "test-zodiac.hpp"
Main::~Main()
{
	LogConsoleReport(-1, L"Main::%S()\n", __FUNCTION__);
	for (int i = 0; i < num_of_writers; ++i) {
		delete m_writers[i];
	}
	for (int i = 0; i < num_of_readers; ++i) {
		delete m_readers[i];
	}
	delete queue;
	LogConsoleReport(-1, L"Main::%S() end\n", __FUNCTION__);
}

Main::Main()
{
	LogConsoleReport(-1, L"Main::%S()\n", __FUNCTION__);
	queue = new MessageQueue<MessageType>(42);
	queue->set_events(this);
	for (int i = 0; i < num_of_readers; ++i) {
		m_readers[i] = new Reader(*queue, i);
	}
	for (int i = 0; i < num_of_writers; ++i) {
		m_writers[i] = new Writer(*queue, i);
	}
	LogConsoleReport(-1, L"Number of Readers: %d, Number of Writers: %d\n", num_of_readers, num_of_writers);
	LogConsoleReport(-1, L"Main::%S() end\n", __FUNCTION__);
}

int Main::main()
{
	queue->run();
	Sleep(12000);
	queue->stop();
	return 0;
}

void Main::on_start()
{
	LogConsoleReport(-1, L"Main::%S()\n", __FUNCTION__);
	for (int i = 0; i < num_of_readers; ++i) {
		m_readers[i]->run();
	}
	for (int i = 0; i < num_of_writers; ++i) {
		m_writers[i]->run();
	}
	LogConsoleReport(-1, L"Main::%S() end\n", __FUNCTION__);
}

void Main::on_stop()
{
	LogConsoleReport(-1, L"Main::%S()\n", __FUNCTION__);
	Writer::sending();

	simstd::vector<HANDLE> hndls;
	for (int i = 0; i < num_of_writers; ++i) {
		hndls.push_back(*m_writers[i]);
	}
	for (int i = 0; i < num_of_readers; ++i) {
		hndls.push_back(*m_readers[i]);
	}
	DWORD res = WaitForMultipleObjects(hndls.size(), &hndls[0], TRUE, 10000);
	if (res == WAIT_OBJECT_0) {
		LogConsoleReport(-1, L"\tAll writers and readers are stopped correctly\n");
	} else {
		LogConsoleReport(-1, L"\tWriters or readers are stopped Incorrectly\n");
	}

	LogConsoleReport(-1, L"Main::%S() end\n", __FUNCTION__);
}

void Main::on_hwm()
{
	LogConsoleReport(-1, L"Main::%S()\n", __FUNCTION__);
	Writer::sleep();
	LogConsoleReport(-1, L"Main::%S() end\n", __FUNCTION__);
}

void Main::on_lwm()
{
	LogConsoleReport(-1, L"Main::%S()\n", __FUNCTION__);
	Writer::sending();
	LogConsoleReport(-1, L"Main::%S() end\n", __FUNCTION__);
}
