#include <libbase/memory.hpp>

#ifdef DEBUG
namespace Memory {
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

namespace Base {

	auto_close<HANDLE>::auto_close(const this_type & rhs):
		m_ptr(nullptr) {
		::DuplicateHandle(::GetCurrentProcess(), rhs.m_ptr, ::GetCurrentProcess(), &m_ptr, 0, FALSE, DUPLICATE_SAME_ACCESS);
	}

	auto_close<HANDLE> & auto_close<HANDLE>::operator = (const this_type & rhs) {
		if (this != &rhs)
			this_type(rhs).swap(*this);
		return *this;
	}

	auto_close<HANDLE>::auto_close(this_type && rhs):
		m_ptr(nullptr) {
		swap(rhs);
	}

	auto_close<HANDLE> & auto_close<HANDLE>::operator = (this_type && rhs) {
		if (this != (this_type*)&(size_t&)rhs) // amazing operator & bypass
			this_type(std::move(rhs)).swap(*this);
		return *this;
	}

	auto_close<HANDLE>::value_type * auto_close<HANDLE>::operator & () {
		close();
		return &m_ptr;
	}


	void auto_close<HANDLE>::close() {
		if (is_valid()) {
			::CloseHandle(m_ptr);
			m_ptr = nullptr;
		}
	}

}
