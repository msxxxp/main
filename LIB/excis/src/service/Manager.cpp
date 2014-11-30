#include <excis/service.hpp>
#include <excis/exception.hpp>

#include <basis/sys/logger.hpp>

namespace service {

	Manager::~Manager()
	{
		LogTraceObjBegin();
		close(m_hndl);
		LogTraceObjEnd();
	}

	Manager::Manager(connection::Remote* conn, ACCESS_MASK acc) :
		m_hndl(open(conn, acc))
	{
		LogTraceObjBegin();
		LogTraceObjEnd();
	}

	Manager::Manager(Manager&& other):
		m_hndl(other.m_hndl)
	{
		LogTraceObj();
		other.m_hndl = nullptr;
	}

	Manager& Manager::operator =(Manager&& other)
	{
		LogTraceObj();
		swap(other);
		return *this;
	}

	void Manager::swap(Manager& other)
	{
		using simstd::swap;
		swap(m_hndl, other.m_hndl);
	}

	Manager::operator SC_HANDLE() const {
		return m_hndl;
	}

	void Manager::reconnect(connection::Remote* conn, ACCESS_MASK acc)
	{
		LogNoise(L"(%p, %X)\n", conn, acc);
		SC_HANDLE l_hndl = open(conn, acc);

		using simstd::swap;
		swap(m_hndl, l_hndl);

		close(l_hndl);
	}

	Item Manager::create_service(const CreateRequest& request) const
	{
		LogNoise(L"(%p)\n", &request);
		SC_HANDLE hndl = CheckHandleErr(
			::CreateServiceW(m_hndl, request.name.c_str(),
			                 request.displayName,
			                 SERVICE_ALL_ACCESS,
			                 request.serviceType,
			                 request.startType,
			                 request.errorControl,
			                 request.binaryPathName.c_str(),
			                 request.loadOrderGroup,
			                 request.tagId,
			                 request.dependencies,
			                 nullptr,
			                 nullptr)
		);
		Item tmp(hndl);
		if (request.delayedStart)
			tmp.set_delayed(request.delayedStart);
		return simstd::move(tmp);
	}

	bool Manager::is_exist(const wchar_t* name) const
	{
		LogNoise(L"(%s)\n", name);
		SC_HANDLE hndl = ::OpenServiceW(m_hndl, name, SERVICE_QUERY_STATUS);
		if (hndl)
			::CloseServiceHandle(hndl);
		return hndl;
	}

	SC_HANDLE Manager::open(connection::Remote* conn, ACCESS_MASK acc)
	{
		LogNoise(L"(%p, %X)\n", conn, acc);
		return CheckHandleErr(::OpenSCManagerW(conn->get_host().c_str(), nullptr, acc));
	}

	void Manager::close(SC_HANDLE scm)
	{
		LogNoise(L"(%p)\n", scm);
		if (scm)
			::CloseServiceHandle(scm);
	}

}
