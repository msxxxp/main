#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>

namespace memory {
	namespace heap {

		void DefaultStat::init(size_t size)
		{
			UNUSED(size);
		}

		void DefaultStat::destroy()
		{
		}

		void* DefaultStat::alloc(size_t size, size_t flags)
		{
			void* ret = HeapAlloc(GetProcessHeap(), flags, size);
			if (ret) {
				++m_stat.allocations;
				m_stat.allocSize += size;
			}
			return ret;
		}

		void* DefaultStat::realloc(void* ptr, size_t size, size_t flags)
		{
			size_t freeSize = HeapSize(GetProcessHeap(), 0, ptr);
			void* ret = HeapReAlloc(GetProcessHeap(), flags, ptr, size);
			if (ret) {
				++m_stat.frees;
				++m_stat.allocations;
				m_stat.freeSize += freeSize;
				m_stat.allocSize += size;
			}
			return ret;
		}

		void DefaultStat::free(const void* ptr)
		{
			if (ptr) {
				++m_stat.frees;
				m_stat.freeSize += HeapSize(GetProcessHeap(), 0, ptr);
			}
			HeapFree(GetProcessHeap(), 0, const_cast<void*>(ptr));
		}

		size_t DefaultStat::size()
		{
			return static_cast<size_t>(-1);
		}

		size_t DefaultStat::size(const void* ptr)
		{
			return HeapSize(GetProcessHeap(), 0, ptr);
		}

		const Stat& DefaultStat::get_stat()
		{
			return m_stat;
		}

		Stat DefaultStat::m_stat;

	}
}
