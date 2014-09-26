#include <basis/configure.hpp>
#include <basis/sys/sub_sync/Mutex.hpp>

namespace sync {

	Mutex::~Mutex()
	{
		::CloseHandle(reinterpret_cast<HANDLE>(m_handle));
	}

	Mutex::Mutex(const wchar_t * name) :
		m_handle(nullptr)
	{
		m_handle = reinterpret_cast<native_handle_type>(::CreateMutexW(nullptr, 0, name));
	}

	void Mutex::lock()
	{
		::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(m_handle), INFINITE, true);
	}

	bool Mutex::try_lock(size_t timeout_millisec)
	{
		return WAIT_OBJECT_0 == ::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(m_handle), timeout_millisec, true);
	}

	void Mutex::unlock()
	{
		::ReleaseMutex(reinterpret_cast<HANDLE>(m_handle));
	}

	Mutex::native_handle_type Mutex::native_handle()
	{
		return m_handle;
	}

}
