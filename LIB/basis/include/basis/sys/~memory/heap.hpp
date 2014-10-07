#ifndef BASIS_SYS_MEMORY_HEAP_HPP_
#define BASIS_SYS_MEMORY_HEAP_HPP_

#include <basis/sys/memory.hpp>
#include <basis/sys/console.hpp>

namespace memory {

	struct HeapStat
	{
		uint64_t allocations;
		uint64_t frees;
		uint64_t allocSize;
		uint64_t freeSize;
	};

	struct HeapDefault
	{
		static void init(size_t size = 0);

		static void destroy();

		static void* alloc(size_t size);

		static void free(const void* ptr);

		static size_t size();

		static size_t size(const void* ptr);

		static HeapStat get_stat();

	private:
		static HANDLE m_heap;
	};

	struct HeapDefaultLogged
	{
		static void init(size_t size = 0);

		static void destroy();

		static void* alloc(size_t size);

		static void free(const void* ptr);

		static size_t size();

		static size_t size(const void* ptr);

		static const HeapStat& get_stat();

	private:
		static HANDLE   m_heap;
		static HeapStat m_stat;
	};

	struct HeapDefaultStat
	{
		static void init(size_t size = 0);

		static void destroy();

		static void* alloc(size_t size);

		static void free(const void* ptr);

		static size_t size();

		static size_t size(const void* ptr);

		static const HeapStat& get_stat();

	private:
		static HANDLE m_heap;
		static HeapStat m_stat;
	};

	template<typename Type>
	struct HeapSpecial
	{
		static void init(size_t /*size*/ = 0)
		{
			m_heap = HeapCreate(0, 0, 0);
		}

		static void destroy()
		{
			HeapDestroy(m_heap);
		}

		static void* alloc(size_t size)
		{
			void* ret = HeapAlloc(m_heap, 0, size);
			if (ret) {
				++m_stat.allocations;
				m_stat.allocSize += size;
			}
			return ret;
		}

		static void free(const void* ptr)
		{
			if (ptr) {
				++m_stat.frees;
				m_stat.freeSize += HeapSize(m_heap, 0, ptr);
			}
			HeapFree(m_heap, 0, const_cast<void*>(ptr));
		}

		static size_t size()
		{
			return static_cast<size_t>(-1);
		}

		static size_t size(const void* ptr)
		{
			return HeapSize(m_heap, 0, ptr);
		}

		static const HeapStat& get_stat()
		{
			return m_stat;
		}

	private:
		static HANDLE   m_heap;
		static HeapStat m_stat;
	};

	template<typename Type>
	HANDLE HeapSpecial<Type>::m_heap;

	template<typename Type>
	HeapStat HeapSpecial<Type>::m_stat;

	template<typename Type>
	struct HeapSpecialLogged
	{
		static void init(size_t /*size*/ = 0)
		{
			m_heap = HeapCreate(0, 0, 0);
		}

		static void destroy()
		{
			HeapDestroy(m_heap);
		}

		static void* alloc(size_t size)
		{
			void* ret = HeapAlloc(m_heap, 0, size);
			console::printf(L"<%5u> %S() [%Iu] -> %p\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, size, ret);
			if (ret) {
				++m_stat.allocations;
				m_stat.allocSize += size;
			}
			return ret;
		}

		static void free(const void* ptr)
		{
			console::printf(L"<%5u> %S() [%p, %Iu]\n", GetCurrentThreadId(), __PRETTY_FUNCTION__, ptr, HeapSize(m_heap, 0, ptr));
			if (ptr) {
				++m_stat.frees;
				m_stat.freeSize += HeapSize(m_heap, 0, ptr);
			}
			HeapFree(m_heap, 0, const_cast<void*>(ptr));
		}

		static size_t size()
		{
			return static_cast<size_t>(-1);
		}

		static size_t size(const void* ptr)
		{
			return HeapSize(m_heap, 0, ptr);
		}

		static const HeapStat& get_stat()
		{
			return m_stat;
		}

	private:
		static HANDLE   m_heap;
		static HeapStat m_stat;
	};

	template<typename Type>
	HANDLE HeapSpecialLogged<Type>::m_heap;

	template<typename Type>
	HeapStat HeapSpecialLogged<Type>::m_stat;

	struct Heap {
		~Heap();

		Heap(size_t size);

		void* alloc(size_t size);

		void free(const void* ptr);

		size_t size() const;

		size_t size(const void* ptr) const;

		HeapStat get_stat();

	private:
		HANDLE m_heap;
	};

}

#endif
