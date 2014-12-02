#include <excis/service.hpp>
#include <excis/exception.hpp>

#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>

namespace service {

	CreateRequest::CreateRequest(const ustring& name, const ustring& binaryPathName) :
		name(name),
		displayName(name),
		binaryPathName(binaryPathName),
		dependencies(nullptr),
		loadOrderGroup(nullptr),
		login(nullptr),
		passw(nullptr),
		tagId(nullptr),
		serviceType(SERVICE_WIN32_OWN_PROCESS),
		startType(SERVICE_DEMAND_START),
		errorControl(SERVICE_ERROR_NORMAL)
	{
	}

	void CreateRequest::set_type(Type n)
	{
		serviceType = static_cast<DWORD>(n);
	}

	void CreateRequest::set_start(Start n)
	{
		startType = static_cast<DWORD>(n);
	}

	void CreateRequest::set_error_control(Error n)
	{
		errorControl = static_cast<DWORD>(n);
	}

	void CreateRequest::set_group(const wchar_t* n)
	{
		if (!cstr::is_empty(n))
			loadOrderGroup = n;
	}

	void CreateRequest::set_tag(DWORD& n)
	{
		tagId = &n;
	}

	void CreateRequest::set_dependencies(const wchar_t* n)
	{
		if (!cstr::is_empty(n))
			dependencies = n;
	}

	void CreateRequest::set_display_name(const wchar_t* n)
	{
		if (!cstr::is_empty(n))
			displayName = n;
	}

	const wchar_t* CreateRequest::get_name() const
	{
		return name.c_str();
	}

	Item CreateRequest::execute(const Manager& manager) const
	{
		LogNoise(L"(%p)\n", manager.get_handle());
		SC_HANDLE hndl = CheckHandleErr(
				::CreateServiceW(manager.get_handle(),
			                 name.c_str(),
			                 displayName.c_str(),
			                 SERVICE_ALL_ACCESS,
			                 serviceType,
			                 startType & 0xFFFF,
			                 errorControl,
			                 binaryPathName.c_str(),
			                 loadOrderGroup,
			                 tagId,
			                 dependencies,
			                 login,
			                 passw)
		);

		Item svc(hndl);
		if (startType == static_cast<DWORD>(Start::AUTO_DELAYED))
			svc.set_delayed(true);

		return simstd::move(svc);
	}

}
