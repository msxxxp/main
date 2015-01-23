#include <basis/configure.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/~memory/heap/Stat.hpp>

void* memory::heap::StatLog::commit_alloc(void* ptr, uint64_t size, const char* function, int line)
{
	if (ptr) {
#if !defined(MEMORY_LOGGING) || MEMORY_LOGGING != 0
		console::printf("%s(%p, %Iu,     %s:%d) <%u>\n", __FUNCTION__, ptr, size, function, line, GetCurrentThreadId());
#endif
		StatCount::commit_alloc(ptr, size, function, line);
	}

	return ptr;
}

void memory::heap::StatLog::commit_free(const void* ptr, uint64_t size, const char* function, int line)
{
	if (ptr) {
#if !defined(MEMORY_LOGGING) || MEMORY_LOGGING != 0
		console::printf("%s( %p, %Iu,     %s:%d) <%u>\n", __FUNCTION__, ptr, size, function, line, GetCurrentThreadId());
#endif
		StatCount::commit_free(ptr, size, function, line);
	}
}
