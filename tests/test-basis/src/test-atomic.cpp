#include <tests.hpp>

//#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/mutex>

#include <atomic>

void test_atomic()
{
	std::atomic_uint_fast64_t a;

	int64_t b = a;
	LogReport(L"is_lock_free(): %d, %I64d\n", a.is_lock_free(), b);
}
