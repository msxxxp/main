#include <basis/sys/logger.hpp>

#include <excis/service.hpp>
#include <excis/exception.hpp>
#include <excis/connection.hpp>

namespace Ext {

	///===================================================================================== Manager
	Service::Manager::~Manager()
	{
		LogTraceObjBegin();
		close(m_hndl);
		LogTraceObjEnd();
	}

	Service::Manager::Manager(connection::Remote * conn, ACCESS_MASK acc) :
		m_hndl(open(conn, acc))
	{
		LogTraceObjBegin();
		LogTraceObjEnd();
	}

	Service::Manager::Manager(Manager && right):
		m_hndl(right.m_hndl)
	{
		right.m_hndl = nullptr;
	}

	Service::Manager & Service::Manager::operator =(Manager && right)
	{
		m_hndl = right.m_hndl;
		right.m_hndl = nullptr;
		return *this;
	}

	void Service::Manager::reconnect(connection::Remote * conn, ACCESS_MASK acc)
	{
		LogTrace();
		SC_HANDLE l_hndl = open(conn, acc);
		simstd::swap(m_hndl, l_hndl);
		close(l_hndl);
	}

	Service Service::Manager::create_service(const Service::Create_t & info) const
	{
		LogTrace();
		SC_HANDLE hndl = CheckHandleErr(
			::CreateServiceW(m_hndl, info.name.c_str(),
				info.displayName,
				SERVICE_ALL_ACCESS,
				info.serviceType,
				info.startType,
				info.errorControl,
				info.binaryPathName.c_str(),
				info.loadOrderGroup,
				info.tagId,
				info.dependencies,
				nullptr,
				nullptr)
		);
		Service tmp(hndl);
		if (info.delayedStart)
			tmp.set_delayed(info.delayedStart);
		return simstd::move(tmp);
	}

	bool Service::Manager::is_exist(PCWSTR name) const
	{
		LogTrace();
		SC_HANDLE hndl = ::OpenServiceW(m_hndl, name, SERVICE_QUERY_STATUS);
		if (hndl)
			::CloseServiceHandle(hndl);
		return hndl;
	}

	SC_HANDLE Service::Manager::open(connection::Remote * conn, ACCESS_MASK acc)
	{
		LogTrace();
		return CheckHandleErr(::OpenSCManagerW(conn->get_host().c_str(), nullptr, acc));
	}

	void Service::Manager::close(SC_HANDLE scm)
	{
		LogTrace();
		if (scm)
			::CloseServiceHandle(scm);
	}

}
