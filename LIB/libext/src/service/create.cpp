#include <libext/service.hpp>
#include <system/cstr.hpp>
#include <simstd/string>

namespace Ext {

	Service::Create_t::Create_t(const ustring & _name, const ustring & _binaryPathName) :
		name(_name),
		serviceType(SERVICE_WIN32_OWN_PROCESS),
		startType(SERVICE_DEMAND_START),
		errorControl(SERVICE_ERROR_NORMAL),
		binaryPathName(_binaryPathName),
		loadOrderGroup(nullptr),
		tagId(nullptr),
		dependencies(nullptr),
		displayName(_name.c_str()),
		delayedStart(0)
	{
	}

	void Service::Create_t::set_type(Type_t n)
	{
		serviceType = (DWORD)n;
	}

	void Service::Create_t::set_start(Start_t n)
	{
		if (n == Start_t::AUTO_DELAYED) {
			startType = (DWORD)Start_t::AUTO;
			set_delayed_start(1);
		} else {
			startType = (DWORD)n;
		}
	}

	void Service::Create_t::set_error_control(Error_t n)
	{
		errorControl = (DWORD)n;
	}

	void Service::Create_t::set_group(PCWSTR n)
	{
		if (!cstr::is_empty(n))
			loadOrderGroup = n;
	}

	void Service::Create_t::set_tag(DWORD & n)
	{
		tagId = &n;
	}

	void Service::Create_t::set_dependencies(PCWSTR n)
	{
		if (!cstr::is_empty(n))
			dependencies = n;
	}

	void Service::Create_t::set_display_name(PCWSTR n)
	{
		if (!cstr::is_empty(n))
			displayName = n;
	}

	void Service::Create_t::set_delayed_start(DWORD n)
	{
		delayedStart = n;
	}

	PCWSTR Service::Create_t::get_name() const
	{
		return name.c_str();
	}

}
