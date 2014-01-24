#include <system/configure.hpp>
#include <system/sub_sync/Semaphore.hpp>

namespace sync {

	Semaphore::~Semaphore()
	{
		::CloseHandle(reinterpret_cast<HANDLE>(m_handle));
	}

	Semaphore::Semaphore(const wchar_t * name) :
		m_handle(nullptr)
	{
		m_handle = reinterpret_cast<native_handle_type>(::CreateSemaphoreW(nullptr, 0, LONG_MAX, name));
	}

	void Semaphore::lock()
	{
		::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(m_handle), INFINITE, true);
	}

	bool Semaphore::try_lock(size_t timeout_millisec)
	{
		return WAIT_OBJECT_0 == ::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(m_handle), timeout_millisec, true);
	}

	WaitResult_t Semaphore::try_lock_ex(size_t timeout_millisec)
	{
		return static_cast<WaitResult_t>(::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(m_handle), timeout_millisec, true));
	}

	void Semaphore::unlock(size_t cnt)
	{
		::ReleaseSemaphore(reinterpret_cast<HANDLE>(m_handle), cnt, nullptr);
	}

	Semaphore::native_handle_type Semaphore::native_handle()
	{
		return m_handle;
	}

}
