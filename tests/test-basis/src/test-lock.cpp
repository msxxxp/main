#include <tests.hpp>

#include <basis/sys/sync.hpp>
//#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/thread.hpp>
#include <basis/simstd/mutex>

sync::SyncUnit_i * m1;
sync::SyncUnit_i * m2;

struct LockMutexThead1: public thread::Routine {
	size_t run(void * data) override
	{
		UNUSED(data);

		while (true) {
			simstd::lock(*m2, *m1);
//			m2->lock();
			LogTrace();
//			m1->lock();
			Sleep(33);
			m1->unlock();
			m2->unlock();
		}

		return 0;
	}
};

struct LockMutexThead2: public thread::Routine {
	size_t run(void * data) override
	{
		UNUSED(data);

		while (true) {
			simstd::lock(*m1, *m2);
//			m1->lock();
			LogTrace();
//			m2->lock();
			Sleep(33);
			m1->unlock();
			m2->unlock();
		}

		return 0;
	}
};

void test_lock()
{
	m1 = sync::get_CritSection();
	m2 = sync::get_CritSection();

	LockMutexThead1 routine1;
	LockMutexThead2 routine2;

	thread::Pool threads;
	threads.create_thread(&routine1);
	threads.create_thread(&routine2);

	threads.wait_all();
}
