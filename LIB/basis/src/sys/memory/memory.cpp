#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>

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
