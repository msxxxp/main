#include <excis/service.hpp>

//#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>

using cstr::compare_cs;

namespace service {

	ConfigRequest::ConfigRequest() :
		serviceType(SERVICE_NO_CHANGE),
		startType(SERVICE_NO_CHANGE),
		errorControl(SERVICE_NO_CHANGE),
		delayedStart(SERVICE_NO_CHANGE),
		binaryPathName(nullptr),
		loadOrderGroup(nullptr),
		dependencies(nullptr),
		displayName(nullptr),
		tagId(nullptr)
	{
	}

	void ConfigRequest::set_type(Type n, Type o)
	{
		if (n != o)
			serviceType = (DWORD)n;
	}

	void ConfigRequest::set_start(Start n, Start o)
	{
		if (n != o) {
			if (n == Start::AUTO_DELAYED) {
				startType = (DWORD)Start::AUTO;
				delayedStart = 1;
			} else {
				startType = (DWORD)n;
				delayedStart = 0;
			}
		}
	}

	void ConfigRequest::set_error_control(Error n, Error o)
	{
		if (n != o)
			errorControl = (DWORD)n;
	}

	void ConfigRequest::set_path(PCWSTR n, PCWSTR o)
	{
		if (compare_cs(n, o) != 0)
			binaryPathName = n;
	}

	void ConfigRequest::set_group(PCWSTR n, PCWSTR o)
	{
		if (compare_cs(n, o) != 0)
			loadOrderGroup = n;
	}

	void ConfigRequest::set_tag(DWORD & n, DWORD o)
	{
		if (n != o)
			tagId = &n;
	}

	void ConfigRequest::set_dependencies(const wchar_t* n, const wchar_t* o)
	{
		if (compare_cs(n, o) != 0)
			dependencies = n;
	}

	void ConfigRequest::set_display_name(const wchar_t* n, const wchar_t* o)
	{
		if (compare_cs(n, o) != 0)
			displayName = n;
	}

	void ConfigRequest::set_delayed_start(DWORD n, DWORD o)
	{
		if (n != o)
			delayedStart = n;
	}

	void ConfigRequest::log() const
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
