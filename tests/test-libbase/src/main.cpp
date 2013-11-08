#include <libbase/std.hpp>
#include <libbase/string.hpp>
#include <libbase/atexit.hpp>
#include <libbase/sstr.hpp>

#include <liblog/logger.hpp>

namespace {
	void setup_logger()
	{
		using namespace Logger;
		set_default_level(Level::Trace);
		set_default_prefix(Prefix::Medium | Prefix::Place);
		set_default_target(get_TargetToConsole());
	}
}

//#include <stdio.h>
//#include <cassert>
//
//void test_ustring()
//{
//		ustring str1(5, L'h');
//		printf("str1: '%S' size: %d\n", str1.c_str(), (int)str1.size());
//		assert(Base::Str::compare(str1.c_str(), L"hhhhh") == 0);
//		assert(str1.size() == 5);
//		assert(*(str1.cend()-1) == L'h');
//		assert(*str1.cend() == 0);
//		assert(*str1.cbegin() == L'h');
//
//		PCWSTR pcwstr = L"PCWSTR";
//		ustring str2(pcwstr);
//		printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//		assert(Base::Str::compare(str2.c_str(), pcwstr) == 0);
//		assert(str2.size() == 6);
//		assert(*(str2.cend()-1) == L'R');
//		assert(*str2.cend() == 0);
//		assert(*str2.cbegin() == L'P');
//
//		PWSTR pwstr = (PWSTR)L"1pwstr2";
//		ustring str3 = pwstr;
//		printf("str3: '%S' size: %d\n\n", str3.c_str(), (int)str3.size());
//		assert(Base::Str::compare(str3.c_str(), pwstr) == 0);
//		assert(str3.size() == 7);
//		assert(*(str3.cend()-1) == L'2');
//		assert(*str3.cend() == 0);
//		assert(*str3.cbegin() == L'1');
//
//		str1 = L"que1";
//		printf("str1: '%S' size: %d\n", str1.c_str(), (int)str1.size());
//		assert(Base::Str::compare(str1.c_str(), L"que1") == 0);
//		assert(str1.size() == 4);
//		assert(*(str1.cend()-1) == L'1');
//		assert(*str1.cend() == 0);
//		assert(*str1.cbegin() == L'q');
//
//		str2 = str1;
//		printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//		assert(Base::Str::compare(str2.c_str(), L"que1") == 0);
//		assert(str2.size() == 4);
//		assert(*(str2.cend()-1) == L'1');
//		assert(*str2.cend() == 0);
//		assert(*str2.cbegin() == L'q');
//		assert(str1.c_str() == str2.c_str());
//
//		str3.clear();
//		printf("str3: '%S' size: %d\n\n", str3.c_str(), (int)str3.size());
//		assert(Base::Str::compare(str3.c_str(), L"") == 0);
//		assert(str3.size() == 0);
////		assert(*(str3.cend()-1) == L'2');
//		assert(*str3.cend() == 0);
//		assert(*str3.cbegin() == 0);
//		assert(str3.empty());
//		assert(str3.cbegin() == str3.cend());
//
//		str2.append(str1);
//		printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//		assert(Base::Str::compare(str2.c_str(), L"que1que1") == 0);
//		assert(str2.size() == 8);
//		assert(*(str2.cend()-1) == L'1');
//		assert(*str2.cend() == 0);
//		assert(*str2.cbegin() == L'q');
//		assert(str1.c_str() != str2.c_str());
//
//		str2.replace(4, 0, L"test2");
//		printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//		assert(Base::Str::compare(str2.c_str(), L"que1test2que1") == 0);
//		assert(str2.size() == 13);
//		assert(*(str2.cend()-1) == L'1');
//		assert(*str2.cend() == 0);
//		assert(*str2.cbegin() == L'q');
//		assert(str2.find(L"tes") == 4);
//		assert(str2.find(L"teqs") == ustring::npos);
//		assert(str2.find(L"ue") == 1);
//		assert(str2.rfind(L'u') == 10);
//		assert(str2.find_first_of(L"12") == 3);
//		assert(str2.find_first_not_of(L"12") == 0);
//		assert(str2.find_first_of(L"21") == 3);
//		assert(str2.find_first_not_of(L"21") == 0);
//		assert(str2.find_first_not_of(L"que1") == 4);
//		assert(str2.find_last_of(L"12") == 12);
//		assert(str2.find_last_of(L"2d") == 8);
//		assert(str2.find_last_not_of(L"12") == 11);
//		assert(str2.find_last_not_of(L"2d") == 12);
//
//		str2.replace(4, 5, L"TEST3");
//		printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//		assert(Base::Str::compare(str2.c_str(), L"que1TEST3que1") == 0);
//		assert(str2.size() == 13);
//
//		str2.replace(0, 512, L"T4");
//		printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//		assert(Base::Str::compare(str2.c_str(), L"T4") == 0);
//		assert(str2.size() == 2);
//
//		str2.replace(2, 128, L"testing5");
//		printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//		assert(Base::Str::compare(str2.c_str(), L"T4testing5") == 0);
//		assert(str2.size() == 10);
//
//		str2.replace(0, 1024, L"");
//		printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//		assert(Base::Str::compare(str2.c_str(), L"") == 0);
//		assert(str2.size() == 0);
//
//		str2.replace(0, 1, L"again6");
//		printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//		assert(Base::Str::compare(str2.c_str(), L"again6") == 0);
//		assert(str2.size() == 6);
//}

int test_mstring()
{
	const wchar_t * w = L"hello\0";
	wchar_t q[] = {L'1', L'\0', L'2', L'2', L'\0', L'3', L'3', L'3', L'\0', L'\0'};

	Base::mstring mstr1;
	Base::mstring mstr2(w);
	Base::mstring mstr3(q);
	Base::mstring mstr4(mstr3);
	Base::mstring mstr5(std::move(mstr3));

	mstr3.push_back(L"hello");
	mstr3.push_back(L", ");
	mstr3.push_back(L"world!");

	LogNoise(L"size: %Iu, capa: %Iu\n", mstr3.size(), mstr3.capacity());
	for (size_t i = 0; i < mstr3.size(); ++i)
		LogNoise(L"'%s'\n", mstr3[i]);

	LogNoise(L"size: %Iu, capa: %Iu\n", mstr4.size(), mstr4.capacity());
	for (size_t i = 0; i < mstr4.size(); ++i)
		LogNoise(L"'%s'\n", mstr4[i]);

	LogNoise(L"size: %Iu, capa: %Iu\n", mstr5.size(), mstr5.capacity());
	for (size_t i = 0; i < mstr5.size(); ++i)
		LogNoise(L"'%s'\n", mstr5[i]);

	return 0;
}

#ifdef NDEBUG
int wWmain()
#else
int main()
#endif
{
	setup_logger();
	LogTrace();
//	test_ustring();
	test_mstring();
	return 0;
}

/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
extern "C" {
	int atexit(Base::CrtFunction pf)
	{
		return Base::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		Base::cxa_pure_virtual();
	}

	int mainCRTStartup()
	{
		//	int	WinMainCRTStartup() {
		Base::init_atexit();
		//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		int Result = 0;

		//		STARTUPINFO StartupInfo = {sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		//		::GetStartupInfo(&StartupInfo);
		//
		//		Result = wWinMain(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(),
		//						  StartupInfo.dwFlags & STARTF_USESHOWWINDOW ? StartupInfo.wShowWindow : SW_SHOWDEFAULT);
		Result = wWmain();

		Base::invoke_atexit();
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
