#include <system/configure.hpp>
#include <system/sub_sync/CriticalSection.hpp>

namespace sync {

	CriticalSection::~CriticalSection()
	{
		::DeleteCriticalSection(reinterpret_cast<PCRITICAL_SECTION>(m_handle));
		delete reinterpret_cast<PCRITICAL_SECTION>(m_handle);
	}

	CriticalSection::CriticalSection():
		m_handle(reinterpret_cast<native_handle_type>(new CRITICAL_SECTION))
	{
		::InitializeCriticalSection(reinterpret_cast<PCRITICAL_SECTION>(m_handle));
	}

	void CriticalSection::lock()
	{
		::EnterCriticalSection(reinterpret_cast<PCRITICAL_SECTION>(m_handle));
	}

	bool CriticalSection::try_lock()
	{
		return ::TryEnterCriticalSection(reinterpret_cast<PCRITICAL_SECTION>(m_handle));
	}

	void CriticalSection::unlock()
	{
		::LeaveCriticalSection(reinterpret_cast<PCRITICAL_SECTION>(m_handle));
	}

	CriticalSection::native_handle_type CriticalSection::native_handle()
	{
		return m_handle;
	}

}
