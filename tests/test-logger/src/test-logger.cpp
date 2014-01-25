#include <liblog/logger.hpp>

#include <system/console.hpp>
#include <system/crt.hpp>

#include <simstd/algorithm>
#include <simstd/vector>

#include <algorithm>
#include <vector>

namespace {
	void setup_logger()
	{
		using namespace logger;
		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		Default::set_level(Level::Trace);
		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		Default::set_prefix(Prefix::Full/* | Prefix::Place*/);
		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
}

class A: private pattern::Uncopyable
{
public:
	A():
		_i(),
		_str(L"")
	{
		console::printf(L"%S[%d] %Iu '%s'\n", __PRETTY_FUNCTION__, __LINE__, _i, _str);
	}

	A(size_t i, const wchar_t * str):
		_i(i),
		_str(str)
	{
		console::printf(L"%S[%d] %Iu '%s'\n", __PRETTY_FUNCTION__, __LINE__, _i, _str);
	}

	A(A&& a):
		_i(),
		_str(L"")
	{
		swap(a);
		console::printf(L"%S[%d] %Iu '%s'\n", __PRETTY_FUNCTION__, __LINE__, _i, _str);
	}

	A& operator =(A&& a)
	{
		A(simstd::move(a)).swap(*this);
		console::printf(L"%S[%d] %Iu '%s'\n", __PRETTY_FUNCTION__, __LINE__, _i, _str);
		return *this;
	}

	void swap(A& a)
	{
		using simstd::swap;
		swap(_i, a._i);
		console::printf(L"%S[%d] %Iu '%s'\n", __PRETTY_FUNCTION__, __LINE__, _i, _str);
	}

	void print() const
	{
		console::printf(L"%S[%d] %Iu '%s'\n", __PRETTY_FUNCTION__, __LINE__, _i, _str);
	}

private:
	size_t _i;
	const wchar_t * _str;
};

A get_a()
{
	return A();
}

#ifdef NDEBUG
int wWmain()
#else
int main()
#endif
{
	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	setup_logger();

	LogTrace();

	const A a1(1, L"1");
		  A a2(2, L"2");
	const A a3(3, L"3");
		  A a4(4, L"4");

	simstd::vector<A> va;
	va.push_back(A(0, L"0"));
//	va.push_back(a1);
//	va.push_back(a2);
//	va.emplace_back(a3);
//	va.emplace_back(a4);
	va.emplace_back(5, L"5");
	va.push_back(A(6, L"6"));

//	va.push_back(simstd::move(a1));
	va.push_back(simstd::move(a2));
//	va.emplace_back(simstd::move(a3));
	va.emplace_back(simstd::move(a4));


	LogTrace();

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

//	BOOL WINAPI	DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID) {
//		switch (dwReason) {
//			case DLL_PROCESS_ATTACH:
//				init_atexit();
//				break;
//
//			case DLL_PROCESS_DETACH:
//				invoke_atexit();
//				break;
//		}
//		return true;
//	}
}
#endif
