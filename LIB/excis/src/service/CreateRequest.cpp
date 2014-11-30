#include <excis/service.hpp>

#include <basis/sys/cstr.hpp>

namespace service {

	CreateRequest::CreateRequest(const ustring& name, const ustring& binaryPathName) :
		name(name),
		binaryPathName(binaryPathName),
		serviceType(SERVICE_WIN32_OWN_PROCESS),
		startType(SERVICE_DEMAND_START),
		errorControl(SERVICE_ERROR_NORMAL),
		loadOrderGroup(nullptr),
		tagId(nullptr),
		dependencies(nullptr),
		displayName(name.c_str()),
		delayedStart(0)
	{
	}

	void CreateRequest::set_type(Type n)
	{
		serviceType = (DWORD)n;
	}

	void CreateRequest::set_start(Start n)
	{
		if (n == Start::AUTO_DELAYED) {
			startType = (DWORD)Start::AUTO;
			set_delayed_start(1);
		} else {
			startType = (DWORD)n;
		}
	}

	void CreateRequest::set_error_control(Error n)
	{
		errorControl = (DWORD)n;
	}

	void CreateRequest::set_group(const wchar_t* n)
	{
		if (!cstr::is_empty(n))
			loadOrderGroup = n;
	}

	void CreateRequest::set_tag(DWORD & n)
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

	void CreateRequest::set_delayed_start(DWORD n)
	{
		delayedStart = n;
	}

	const wchar_t* CreateRequest::get_name() const
	{
		return name.c_str();
	}

}
