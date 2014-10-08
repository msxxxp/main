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

		void* DefaultStat::alloc(size_t size)
		{
			void* ret = HeapAlloc(GetProcessHeap(), 0, size);
			if (ret) {
				++m_stat.allocations;
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
