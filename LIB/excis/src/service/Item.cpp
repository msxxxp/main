#include "service_pvt.hpp"

#include <excis/exception.hpp>

//#include <libbase/bit.hpp>
//#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>


namespace service {

	namespace {
//		const size_t SERVICE_CONFIG_DELAYED_AUTO_START_INFO = 3;
	}

	Item::~Item()
	{
		LogTraceObjBegin();
//		LogNoise2(L"%p\n", m_hndl);
		if (m_hndl) {
			::CloseServiceHandle(m_hndl);
			m_hndl = nullptr;
		}
		LogTraceObjEnd();
	}

	Item::Item(SC_HANDLE scm, const wchar_t* name, ACCESS_MASK access) :
		m_hndl(CheckHandleErr(::OpenServiceW(scm, name, access)))
	{
		LogTraceObjBegin();
		LogNoise2(L"(%p, %s, 0x%X) -> %p\n", scm, name, access, m_hndl);
		LogTraceObjEnd();
	}

	Item::Item(this_type&& other) :
		m_hndl(other.m_hndl)
	{
		LogTraceObj();
		other.m_hndl = nullptr;
	}

	Item& Item::operator =(this_type&& other)
	{
		LogTraceObj();
		m_hndl = other.m_hndl;
		other.m_hndl = nullptr;
		return *this;
	}

	Item::operator SC_HANDLE() const
	{
		return m_hndl;
	}

	void Item::del()
	{
		CheckApi(::DeleteService(m_hndl));
	}

//	template<typename Functor>
//	void WaitForState(DWORD state, DWORD dwTimeout, Functor &func, PVOID param = nullptr) const {
//		DWORD	dwStartTime = ::GetTickCount();
//		DWORD	dwBytesNeeded;
//		SERVICE_STATUS_PROCESS ssp = {0};
//		while (true) {
//			CheckApi(::QueryServiceStatusEx(m_hndl, SC_STATUS_PROCESS_INFO, (PBYTE)&ssp, sizeof(ssp), &dwBytesNeeded));
//			if (ssp.dwCurrentState == state)
//				break;
//			if (::GetTickCount() - dwStartTime > dwTimeout)
//				throw	ApiError(WAIT_TIMEOUT);
//			func(state, ::GetTickCount() - dwStartTime, param);
//		}
//	}

	Item::this_type& Item::start()
	{
		CheckApi(::StartServiceW(m_hndl, 0, nullptr) || ::GetLastError() == ERROR_SERVICE_ALREADY_RUNNING);
		return *this;
	}

	Item::this_type& Item::stop()
	{
		SERVICE_STATUS ss;
		CheckApi(::ControlService(m_hndl, SERVICE_CONTROL_STOP, &ss) || ::GetLastError() == ERROR_SERVICE_NOT_ACTIVE);
		return *this;
	}

	Item::this_type& Item::contin()
	{
		SERVICE_STATUS ss;
		CheckApi(::ControlService(m_hndl, SERVICE_CONTROL_CONTINUE, &ss));
		return *this;
	}

	Item::this_type& Item::pause()
	{
		SERVICE_STATUS ss;
		CheckApi(::ControlService(m_hndl, SERVICE_CONTROL_PAUSE, &ss));
		return *this;
	}

	Item::this_type& Item::set_description(const wchar_t* info)
	{
		SERVICE_DESCRIPTIONW descr = {(PWSTR)info};
		CheckApi(::ChangeServiceConfig2W(m_hndl, SERVICE_CONFIG_DESCRIPTION, &descr));
		return *this;
	}

	Item::this_type& Item::set_delayed(bool state)
	{
		SERVICE_DELAYED_AUTO_START_INFO info = {state};
		CheckApi(::ChangeServiceConfig2W(m_hndl, SERVICE_CONFIG_DELAYED_AUTO_START_INFO, &info));
		return *this;
	}

	Item::this_type& Item::wait_state(State state, DWORD dwTimeout)
	{
		DWORD dwStartTime = ::GetTickCount();
		while (true) {
			Status ssp(get_status());
			if (ssp.dwCurrentState == (DWORD)state)
				break;
			if (::GetTickCount() - dwStartTime > dwTimeout)
				CheckApiError(WAIT_TIMEOUT);
			::Sleep(500);
		};
		return *this;
	}

	ustring Item::get_description() const
	{
		auto conf(QueryConfig2(m_hndl, SERVICE_CONFIG_DESCRIPTION));
		LPSERVICE_DESCRIPTIONW lpsd = (LPSERVICE_DESCRIPTIONW)conf.data();
		return ustring((lpsd->lpDescription) ? lpsd->lpDescription : EMPTY_STR);
	}

	bool Item::get_delayed() const
	{
		bool ret = false;
		try {
			auto conf(QueryConfig2(m_hndl, SERVICE_CONFIG_DELAYED_AUTO_START_INFO));
			LPSERVICE_DELAYED_AUTO_START_INFO lpsd = (LPSERVICE_DELAYED_AUTO_START_INFO)conf.data();
			ret = lpsd->fDelayedAutostart;
		} catch (exception::Abstract& e) {
		}
		return ret;
	}

	Status Item::get_status() const
	{
		Status ret;
		DWORD bytesNeeded;
		CheckApi(::QueryServiceStatusEx(m_hndl, SC_STATUS_PROCESS_INFO, (PBYTE )&ret, sizeof(ret), &bytesNeeded));
		return ret;
	}

	State Item::get_state() const
	{
		Status tmp(get_status());
		return static_cast<State>(tmp.dwCurrentState);
	}

	Start Item::get_start_type() const
	{
		return static_cast<Start>(QueryConfig(m_hndl)->dwStartType);
	}

	DWORD Item::get_type() const
	{
		return QueryConfig(m_hndl)->dwServiceType;
	}

	ustring Item::get_user() const
	{
		auto config = QueryConfig(m_hndl);
		PCWSTR ret = config->lpServiceStartName ? config->lpServiceStartName : EMPTY_STR;
		return ustring(ret);
	}

	void Item::del(SC_HANDLE scm, PCWSTR name)
	{
		Item(scm, name, SERVICE_STOP | DELETE).stop().del();
	}

	Item::this_type Item::start(SC_HANDLE scm, PCWSTR name)
	{
		return simstd::move(this_type(scm, name, SERVICE_START | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).start());
	}

	Item::this_type Item::stop(SC_HANDLE scm, const wchar_t* name)
	{
		return simstd::move(this_type(scm, name, SERVICE_STOP | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).stop());
	}

	Item::this_type Item::restart(SC_HANDLE scm, const wchar_t* name)
	{
		return simstd::move(this_type(scm, name, SERVICE_STOP | SERVICE_START | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).stop().wait_state(State::STOPPED, 30000).start());
	}

	Item::this_type Item::contin(SC_HANDLE scm, const wchar_t* name)
	{
		return simstd::move(this_type(scm, name, SERVICE_START | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).contin());
	}

	Item::this_type Item::pause(SC_HANDLE scm, const wchar_t* name)
	{
		return simstd::move(this_type(scm, name, SERVICE_STOP | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).pause());
	}

	Item::this_type Item::set_config(SC_HANDLE scm, const wchar_t* name, const ConfigRequest& request)
	{
		this_type svc(scm, name, SERVICE_CHANGE_CONFIG | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS);
		return simstd::move(request.execute(svc));
	}

	Status Item::get_status(SC_HANDLE scm, const wchar_t* name)
	{
		return this_type(scm, name, SERVICE_QUERY_STATUS).get_status();
	}

	State Item::get_state(SC_HANDLE scm, const wchar_t* name)
	{
		return (State)get_status(scm, name).dwCurrentState;
	}

	Start Item::get_start_type(SC_HANDLE scm, const wchar_t* name)
	{
		return this_type(scm, name, SERVICE_QUERY_CONFIG).get_start_type();
	}

	ustring Item::get_description(SC_HANDLE scm, const wchar_t* name)
	{
		return this_type(scm, name, SERVICE_QUERY_CONFIG).get_description();
	}

	Item::Item(SC_HANDLE svc):
		m_hndl(svc)
	{
	}

}
