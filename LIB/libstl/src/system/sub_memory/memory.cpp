#include <system/configure.hpp>
#include <system/memory.hpp>

#ifdef MEMORY_DEBUG
namespace memory {
	namespace Watchdog
	{
		size_t allocations = 0;
		size_t deletions = 0;
		uint64_t allocations_size = 0;
		uint64_t deletions_size = 0;

		pfunc on_delete = nullptr;
	};
}
#endif

namespace memory {

	auto_close<HANDLE>::auto_close(const this_type & rhs) :
		m_hndl(nullptr)
	{
		::DuplicateHandle(::GetCurrentProcess(), rhs.m_hndl, ::GetCurrentProcess(), &m_hndl, 0, FALSE, DUPLICATE_SAME_ACCESS);
	}

	auto_close<HANDLE> & auto_close<HANDLE>::operator =(const this_type & rhs)
	{
		if (this != &rhs)
			this_type(rhs).swap(*this);
		return *this;
	}

	auto_close<HANDLE>::auto_close(this_type && rhs) :
		m_hndl(nullptr)
	{
		swap(rhs);
	}

	auto_close<HANDLE> & auto_close<HANDLE>::operator =(this_type && rhs)
	{
		if (this->m_hndl != rhs.m_hndl)
			this_type(simstd::move(rhs)).swap(*this);
		return *this;
	}

	auto_close<HANDLE>::value_type * auto_close<HANDLE>::operator &()
	{
		close();
		return &m_hndl;
	}

	void auto_close<HANDLE>::close()
	{
		if (is_valid()) {
			::CloseHandle(m_hndl);
			m_hndl = nullptr;
		}
	}

}
