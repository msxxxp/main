#include <libext/service.hpp>
#include <liblog/logger.hpp>
#include <libbase/cstr.hpp>

using Cstr::compare_cs;

namespace Ext {

	Service::Config_t::Config_t() :
		serviceType(SERVICE_NO_CHANGE),
		startType(SERVICE_NO_CHANGE),
		errorControl(SERVICE_NO_CHANGE),
		binaryPathName(nullptr),
		loadOrderGroup(nullptr),
		tagId(nullptr),
		dependencies(nullptr),
		displayName(nullptr),
		delayedStart(SERVICE_NO_CHANGE)
	{
	}

	void Service::Config_t::set_type(Type_t n, Type_t o)
	{
		if (n != o)
			serviceType = (DWORD)n;
	}

	void Service::Config_t::set_start(Start_t n, Start_t o)
	{
		if (n != o) {
			if (n == Start_t::AUTO_DELAYED) {
				startType = (DWORD)Start_t::AUTO;
				delayedStart = 1;
			} else {
				startType = (DWORD)n;
				delayedStart = 0;
			}
		}
	}

	void Service::Config_t::set_error_control(Error_t n, Error_t o)
	{
		if (n != o)
			errorControl = (DWORD)n;
	}

	void Service::Config_t::set_path(PCWSTR n, PCWSTR o)
	{
		if (compare_cs(n, o) != 0)
			binaryPathName = n;
	}

	void Service::Config_t::set_group(PCWSTR n, PCWSTR o)
	{
		if (compare_cs(n, o) != 0)
			loadOrderGroup = n;
	}

	void Service::Config_t::set_tag(DWORD & n, DWORD o)
	{
		if (n != o)
			tagId = &n;
	}

	void Service::Config_t::set_dependencies(PCWSTR n, PCWSTR o)
	{
		if (compare_cs(n, o) != 0)
			dependencies = n;
	}

	void Service::Config_t::set_display_name(PCWSTR n, PCWSTR o)
	{
		if (compare_cs(n, o) != 0)
			displayName = n;
	}

	void Service::Config_t::set_delayed_start(DWORD n, DWORD o)
	{
		if (n != o)
			delayedStart = n;
	}

	void Service::Config_t::log() const
	{
		LogInfo(L"serviceType: %d\n", (int)serviceType);
		LogInfo(L"startType: %d\n", (int)startType);
		LogInfo(L"errorControl: %d\n", (int)errorControl);
		LogInfo(L"binaryPathName: '%s'\n", binaryPathName);
		LogInfo(L"loadOrderGroup: '%s'\n", loadOrderGroup);
		LogInfo(L"tagId: '%s'\n", tagId);
		LogInfo(L"dependencies: '%s'\n", dependencies);
		LogInfo(L"displayName: '%s'\n", displayName);
		LogInfo(L"delayedStart: %d\n", (int)delayedStart);
	}

}
