#include <system/console.hpp>
#include <system/crt.hpp>

#include <simstd/string>

#include "A.hpp"

void test_to_str()
{
	{
		long long nums[] = {LLONG_MIN, -1, 0, 1, LLONG_MAX};
		for (size_t i = 0; i < lengthof(nums); ++i) {
			ustring u = simstd::to_wstring(nums[i]);
			console::printf(L"%I64d -> '%s'\n", nums[i], u.c_str());
		}
	}
	{
		unsigned long long nums[] = {0, 1, ULLONG_MAX};
		for (size_t i = 0; i < lengthof(nums); ++i) {
			ustring u = simstd::to_wstring(nums[i]);
			console::printf(L"%I64u -> '%s'\n", nums[i], u.c_str());
		}
	}
	{
		double nums[] = {LONG_MIN, -1000000000, -1, 0, 1, 1000000000, LONG_MAX};
		for (size_t i = 0; i < lengthof(nums); ++i) {
			ustring u = simstd::to_wstring(nums[i]);
			console::printf(L"%lf -> '%s'\n", nums[i], u.c_str());
		}
	}
}

class Interface
{
public:
	virtual void do_stuff() = 0;

protected:
	~Interface() {}
};

class Implementation : public Interface
{
public:
	~Implementation()
	{
		console::printf(L"%S\n", __PRETTY_FUNCTION__);
	}

	Implementation()
	{
		console::printf(L"%S\n", __PRETTY_FUNCTION__);
	}

	void do_stuff() override
	{
		// ...
	}
};

#ifdef NDEBUG
int wWmain()
#else
int main()
#endif
{

//	simstd::shared_ptr<Interface> ptr1 = simstd::make_shared<Implementation>();
//	simstd::shared_ptr<Interface> ptr2 = nullptr;
//	std::shared_ptr<Interface> ptr3 = nullptr;
//	ptr2.reset(new Implementation);

//	test_to_str();

	return 0;
}

/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
extern "C" {
	int atexit(crt::Function pf)
	{
		return crt::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		crt::cxa_pure_virtual();
	}

	int mainCRTStartup()
	{
		//	int	WinMainCRTStartup() {
		crt::init_atexit();
		//		Base::console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		int Result = 0;

		//		STARTUPINFO StartupInfo = {sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		//		::GetStartupInfo(&StartupInfo);
		//
		//		Result = wWinMain(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(),
		//						  StartupInfo.dwFlags & STARTF_USESHOWWINDOW ? StartupInfo.wShowWindow : SW_SHOWDEFAULT);
		Result = wWmain();

		crt::invoke_atexit();
		::ExitProcess(Result);
		return Result;
	}
}
#endif
