#include <excis/service.hpp>
#include <excis/exception.hpp>

//#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>

using cstr::compare_cs;

namespace service {

	namespace {
		bool if_differ(const wchar_t* n, const wchar_t* o)
		{
			return !cstr::is_empty(n) && compare_cs(n, o) != 0;
		}
	}

	ConfigRequest::ConfigRequest() :
		binaryPathName(nullptr),
		loadOrderGroup(nullptr),
		dependencies(nullptr),
		displayName(nullptr),
		login(nullptr),
		passw(nullptr),
		tagId(nullptr),
		serviceType(SERVICE_NO_CHANGE),
		startType(SERVICE_NO_CHANGE),
		errorControl(SERVICE_NO_CHANGE)
	{
	}

	void ConfigRequest::set_type(Type n, Type o)
	{
		if (n != o)
			serviceType = static_cast<DWORD>(n);
	}

	void ConfigRequest::set_start(Start n, Start o)
	{
		if (n != o)
			startType = static_cast<DWORD>(n);
	}

	void ConfigRequest::set_error_control(Error n, Error o)
	{
		if (n != o)
			errorControl = static_cast<DWORD>(n);
	}

	void ConfigRequest::set_path(const wchar_t* n, const wchar_t* o)
	{
		if (if_differ(n, o))
			binaryPathName = n;
	}

	void ConfigRequest::set_group(const wchar_t* n, const wchar_t* o)
	{
		if (if_differ(n, o))
			loadOrderGroup = n;
	}

	void ConfigRequest::set_tag(DWORD & n, DWORD o)
	{
		if (n != o)
			tagId = &n;
	}

	void ConfigRequest::set_dependencies(const wchar_t* n, const wchar_t* o)
	{
		if (if_differ(n, o))
			dependencies = n;
	}

	void ConfigRequest::set_display_name(const wchar_t* n, const wchar_t* o)
	{
		if (if_differ(n, o))
			displayName = n;
	}

	void ConfigRequest::set_login(const wchar_t* user, const wchar_t* pass)
	{
		login = user;
		passw = pass;
	}

	void ConfigRequest::log() const
	{
		LogInfo(L"serviceType: %X\n", (int)serviceType);
		LogInfo(L"startType: %X\n", (int)startType);
		LogInfo(L"errorControl: %X\n", (int)errorControl);
		LogInfo(L"binaryPathName: '%s'\n", binaryPathName);
		LogInfo(L"loadOrderGroup: '%s'\n", loadOrderGroup);
		LogInfo(L"tagId: '%s'\n", tagId);
		LogInfo(L"dependencies: '%s'\n", dependencies);
		LogInfo(L"displayName: '%s'\n", displayName);
	}

	Item& ConfigRequest::execute(Item& svc) const
	{
		LogNoise(L"(%p)\n", static_cast<SC_HANDLE>(svc));
		CheckApi(
		    ::ChangeServiceConfigW(static_cast<SC_HANDLE>(svc),
		                           serviceType,
		                           (startType != SERVICE_NO_CHANGE) ? startType & 0xFFFF : startType,
		                           errorControl,
		                           binaryPathName,
		                           loadOrderGroup,
		                           tagId,
		                           dependencies,
		                           login,
		                           passw,
		                           displayName)
		);

		if (startType != SERVICE_NO_CHANGE) {
			svc.set_delayed(startType == static_cast<DWORD>(Start::AUTO_DELAYED));
		}

		return svc;
	}

}
