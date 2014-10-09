#include <basis/configure.hpp>
#include <basis/sys/console.hpp>
#include <basis/sys/memory.hpp>

namespace memory {
	namespace heap {

		void DefaultLogged::init(size_t /*size*/)
		{
		}

		void DefaultLogged::destroy()
		{
		}

		void* DefaultLogged::alloc(size_t size, size_t flags)
		{
			void* ret = HeapAlloc(GetProcessHeap(), flags, size);
			console::printf(L"<%5u> %S() [%Iu] -> %p\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, size, ret);
			if (ret) {
				++m_stat.allocations;
				m_stat.allocSize += size;
			}
			return ret;
		}

		void* DefaultLogged::realloc(void* ptr, size_t size, size_t flags)
		{
			size_t freeSize = HeapSize(GetProcessHeap(), 0, ptr);
			void* ret = HeapReAlloc(GetProcessHeap(), flags, ptr, size);
			if (ret) {
				console::printf(L"<%5u> %S() [%Iu] -> %p\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, size, ret);
				++m_stat.frees;
				++m_stat.allocations;
				m_stat.freeSize += freeSize;
				m_stat.allocSize += size;
			}
			return ret;
		}

		void DefaultLogged::free(const void* ptr)
		{
			console::printf(L"<%5u> %S() [%p]\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, ptr);
			if (ptr) {
				++m_stat.frees;
				m_stat.freeSize += HeapSize(GetProcessHeap(), 0, ptr);
			}
			HeapFree(GetProcessHeap(), 0, const_cast<void*>(ptr));
		}

		size_t DefaultLogged::size()
		{
			size_t ret = static_cast<size_t>(-1);
			console::printf(L"<%5u> %S() -> %Iu\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, ret);
			return ret;
		}

		size_t DefaultLogged::size(const void* ptr)
		{
			size_t ret = HeapSize(GetProcessHeap(), 0, ptr);
			console::printf(L"<%5u> %S() [%p] -> %Iu\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, ptr, ret);
			return ret;
		}

		const Stat& DefaultLogged::get_stat()
		{
			return m_stat;
		}

		Stat DefaultLogged::m_stat;

	}
}
