#include <system/configure.hpp>
#include <simstl/impl/mutex/recursive_mutex_base.hpp>

namespace simstd {
	namespace pvt {

		recursive_mutex_base::~recursive_mutex_base()
		{
			::CloseHandle(reinterpret_cast<HANDLE>(m_handle));
		}

		recursive_mutex_base::recursive_mutex_base() :
			m_handle(nullptr)
		{
			m_handle = reinterpret_cast<native_handle_type>(::CreateMutexW(nullptr, 0, nullptr));
		}

		void recursive_mutex_base::lock()
		{
			::WaitForSingleObject(reinterpret_cast<HANDLE>(m_handle), INFINITE);
		}

		bool recursive_mutex_base::try_lock()
		{
			return WAIT_OBJECT_0 == ::WaitForSingleObject(reinterpret_cast<HANDLE>(m_handle), 0);
		}

		void recursive_mutex_base::unlock()
		{
			::ReleaseMutex(reinterpret_cast<HANDLE>(m_handle));
		}

		recursive_mutex_base::native_handle_type recursive_mutex_base::native_handle()
		{
			return m_handle;
		}

	}    // namespace pvt
}    // namespace simstd
