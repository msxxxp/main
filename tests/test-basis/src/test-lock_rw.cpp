#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>

//class WriterRoutine: public thread::Routine
//{
//public:
//	~WriterRoutine()
//	{
//		LogTraceObj();
//	}
//
//	WriterRoutine(sync::Queue* queue, ssize_t num):
//		m_queue(queue),
//		m_num(num)
//	{
//		LogTraceObj();
//	}
//
//	size_t run(void*) override
//	{
//		LogDebug(L"Start routine\n");
//		::Sleep(3000);
//		sync::Message message;
//		m_queue->get_message(message, 5000);
//		LogDebug(L"Exit routine\n");
//		return m_num;
//	}
//
//private:
//	sync::Queue* m_queue;
//	ssize_t m_num;
//};

void test_lock_rw()
{


}
