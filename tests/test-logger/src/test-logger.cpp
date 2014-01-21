#include <liblog/logger.hpp>

#include <system/console.hpp>
#include <system/crt.hpp>

#include <simstl/algorithm>

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

//class A {
//public:
//	~A()
//	{
//		Base::console::printf(L"%S\n", __PRETTY_FUNCTION__);
//	}
//
//	A() :
//		m_int(1)
//	{
//		Base::console::printf(L"%S\n", __PRETTY_FUNCTION__);
//	}
//
//	int get_int() const
//	{
//		return m_int;
//	}
//
//private:
//	int m_int;
//};
//
//class B: public A {
//public:
//	~B()
//	{
//		Base::console::printf(L"%S\n", __PRETTY_FUNCTION__);
//	}
//
//	B() :
//		m_double(10)
//	{
//		Base::console::printf(L"%S\n", __PRETTY_FUNCTION__);
//	}
//
//	double get_double() const
//	{
//		return m_double;
//	}
//
//private:
//	double m_double;
//};

#include <tuple>

std::tuple<double, char> get_student(int /*id*/)
{
	return std::make_tuple(3.8, 'A');
	//    if (id == 1) return std::make_tuple(2.9, 'C', "Milhouse Van Houten");
	//    if (id == 2) return std::make_tuple(1.7, 'D', "Ralph Wiggum");
	//    throw std::invalid_argument("id");
}

class A: private pattern::Uncopyable
{
public:
	A():
		_i()
	{
		console::printf(L"%S[%d]\n", __PRETTY_FUNCTION__, __LINE__);
	}

	A(A&& a):
		_i()
	{
		console::printf(L"%S[%d]\n", __PRETTY_FUNCTION__, __LINE__);
		swap(a);
	}

	A& operator =(A&& a)
	{
		console::printf(L"%S[%d]\n", __PRETTY_FUNCTION__, __LINE__);
		A(simstd::move(a)).swap(*this);
		return *this;
	}

	void swap(A& a)
	{
		console::printf(L"%S[%d]\n", __PRETTY_FUNCTION__, __LINE__);
		using simstd::swap;
		swap(_i, a._i);
	}

private:
	int _i;
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
	A a(get_a());
	A b(simstd::move(a));

	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	setup_logger();

	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	LogTrace();

	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

//	{
//		std::shared_ptr<A> sptr(std::make_shared<B>());
//	}

	{
		auto student0 = get_student(0);
		LogDebug(L"ID: %f, GPA: %c\n", std::get<0>(student0), std::get<1>(student0));
		LogDebug(L"ID: %d, GPA: %c\n", (int )std::get<0>(student0), std::get<1>(student0));

	}
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
