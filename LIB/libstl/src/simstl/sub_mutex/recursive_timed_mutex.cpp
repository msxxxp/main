#include <system/configure.hpp>
#include <simstl/impl/mutex/recursive_timed_mutex.hpp>

namespace simstd {

	bool recursive_timed_mutex::_try_lock_for(size_t timeout_millisec)
	{
		return WAIT_OBJECT_0 == ::WaitForSingleObject(reinterpret_cast<HANDLE>(m_handle), timeout_millisec);
	}

}    // namespace simstd
