#include <libbase/std.hpp>
#include <libbase/console.hpp>
#include <atomic>

#include <stdio.h>

int test_atomic()
{
	std::atomic_uint_fast64_t a;

	int64_t b = a;
	Base::Console::printf(L"%d\n", a.is_lock_free());

	return b;
}


int wWinMain(const wchar_t * pCmdLine)
{
	UNUSED(pCmdLine);
	return test_atomic();
}
