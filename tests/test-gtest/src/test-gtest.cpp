#include <libbase/string.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <gtest/gtest.h>

static const int N = 5000;

std::vector<std::string> std_strings;
std::vector<std::string*> std_strings_p;
std::vector<t_astring> strings;
std::vector<t_astring*> strings_p;

// Объект для сравнения двух std::string.
class StlStringCmp {
public:
	bool operator()(const std::string& a, const std::string& b)
	{
		return a < b;
	}
};

TEST(SortingStlString, StlString)
{
	std::sort(std_strings.begin(), std_strings.end(), StlStringCmp());
}

// Объект для сравнения двух std::string*.
class StlStringPointerCmp {
public:
	bool operator()(const std::string* a, const std::string* b)
	{
		return *a < *b;
	}
};

TEST(SortingStlString, StlStringPointer)
{
	std::sort(std_strings_p.begin(), std_strings_p.end(), StlStringPointerCmp());
}

// Объект для сравнения двух String.
class StringCmp {
public:
	bool operator()(const t_astring& a, const t_astring& b)
	{
		return a < b;
	}
};

TEST(SortingStlString, String) {
  std::sort(strings.begin(), strings.end(), StringCmp());
}

// Объект для сравнения двух String*.
class StringPointerCmp {
public:
	bool operator()(const t_astring* a, const t_astring* b)
	{
		return *a < *b;
	}
};

TEST(SortingStlString, StringPointer) {
  std::sort(strings_p.begin(), strings_p.end(), StringPointerCmp());
}

int main(int argc, char* argv[])
{
	// Это наполнитель, чтобы строки были длинные, и копирование было
	// ощутимо дорого.
	std::string big(1024 * 128, '?');
	for (int i = 0; i < N; ++i) {
		// Все строки будут одинаковой длины. Функции сравнения рассчитывают
		// на это.
		std::stringstream fmt;
		fmt << N * 2 - i << big;
		// std::string строка-объект.
		std_strings.push_back(fmt.str());
		// std::string строка-указатель.
		std_strings_p.push_back(new std::string(fmt.str()));
		// Моя строка-объект.
		strings.push_back(t_astring(fmt.str().c_str()));
		// Моя строка-указатель.
		strings_p.push_back(new t_astring(fmt.str().c_str()));
	}

	testing::InitGoogleTest(&argc, argv);
	// Принудительно печатаем время работы тестов.
	testing::GTEST_FLAG(print_time) = true;
	return RUN_ALL_TESTS();
}

//#include <libbase/std.hpp>
//#include <libbase/string.hpp>
//#include <libbase/str.hpp>
//
////#include <libbase/logger.hpp>
//
//namespace {
//	void setup_logger()
//	{
////		using namespace Logger;
////		set_default_level(Level::Trace);
////		set_default_prefix(Prefix::Medium | Prefix::Place);
////		set_default_target(get_TargetToConsole());
//	}
//}
//
//#include <stdio.h>
//#include <cassert>
//
//void test_ustring()
//{
////	printf("start\n");
//	t_ustring str1(5, L'h');
////	printf("str1: '%S' size: %d\n", str1.c_str(), (int)str1.size());
//	assert(Base::Str::compare(str1.c_str(), L"hhhhh") == 0);
//	assert(str1.size() == 5);
//	assert(*(str1.cend()-1) == L'h');
//	assert(*str1.cend() == 0);
//	assert(*str1.cbegin() == L'h');
//
//	PCWSTR pcwstr = L"PCWSTR";
//	t_ustring str2(pcwstr);
////	printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//	assert(Base::Str::compare(str2.c_str(), pcwstr) == 0);
//	assert(str2.size() == 6);
//	assert(*(str2.cend()-1) == L'R');
//	assert(*str2.cend() == 0);
//	assert(*str2.cbegin() == L'P');
//
//	PWSTR pwstr = (PWSTR)L"1pwstr2";
//	t_ustring str3 = pwstr;
////	printf("str3: '%S' size: %d\n\n", str3.c_str(), (int)str3.size());
//	assert(Base::Str::compare(str3.c_str(), pwstr) == 0);
//	assert(str3.size() == 7);
//	assert(*(str3.cend()-1) == L'2');
//	assert(*str3.cend() == 0);
//	assert(*str3.cbegin() == L'1');
//
//	str1 = L"que1";
////	printf("str1: '%S' size: %d\n", str1.c_str(), (int)str1.size());
//	assert(Base::Str::compare(str1.c_str(), L"que1") == 0);
//	assert(str1.size() == 4);
//	assert(*(str1.cend()-1) == L'1');
//	assert(*str1.cend() == 0);
//	assert(*str1.cbegin() == L'q');
//
//	str2 = str1;
////	printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//	assert(Base::Str::compare(str2.c_str(), L"que1") == 0);
//	assert(str2.size() == 4);
//	assert(*(str2.cend()-1) == L'1');
//	assert(*str2.cend() == 0);
//	assert(*str2.cbegin() == L'q');
//	assert(str1.c_str() == str2.c_str());
//
//	str3.clear();
////	printf("str3: '%S' size: %d\n\n", str3.c_str(), (int)str3.size());
//	assert(Base::Str::compare(str3.c_str(), L"") == 0);
//	assert(str3.size() == 0);
//	assert(*str3.cend() == 0);
//	assert(*str3.cbegin() == 0);
//	assert(str3.empty());
//	assert(str3.cbegin() == str3.cend());
//
//	str2.append(str1);
////	printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//	assert(Base::Str::compare(str2.c_str(), L"que1que1") == 0);
//	assert(str2.size() == 8);
//	assert(*(str2.cend()-1) == L'1');
//	assert(*str2.cend() == 0);
//	assert(*str2.cbegin() == L'q');
//	assert(str1.c_str() != str2.c_str());
//
//	str2.replace(4, 1, L"test2");
////	printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//	assert(Base::Str::compare(str2.c_str(), L"que1test2ue1") == 0);
//	assert(str2.size() == 12);
//	assert(*(str2.cend()-1) == L'1');
//	assert(*str2.cend() == 0);
//	assert(*str2.cbegin() == L'q');
//	assert(str2.find(L"tes") == 4);
//	assert(str2.find(L"teqs") == t_ustring::npos);
//	assert(str2.find(L"ue") == 1);
//	assert(str2.rfind(L'u') == 9);
//	assert(str2.find_first_of(L"12") == 3);
//	assert(str2.find_first_not_of(L"12") == 0);
//	assert(str2.find_first_of(L"21") == 3);
//	assert(str2.find_first_not_of(L"21") == 0);
//	assert(str2.find_first_not_of(L"que1") == 4);
//	assert(str2.find_last_of(L"12") == 11);
//	assert(str2.find_last_of(L"2d") == 8);
//	assert(str2.find_last_not_of(L"2d") == 11);
//
//	str2.erase(3, 6);
////	printf("str2: '%S' size: %d\n", str2.c_str(), (int)str2.size());
//	assert(Base::Str::compare(str2.c_str(), L"queue1") == 0);
//}
//
//int wWmain() {
//	setup_logger();
//	test_ustring();
//
////	std::string a;
////	a += "a";
//
////	typedef std::pair<char, int> datapair;
////	typedef std::vector<datapair> vec;
////
////	std::unique_ptr<int[]> ptr1(new int[5]);
////	std::auto_ptr<int> ptr2(new int);
////	vec v;
////	v.reserve(128);
////	vec::iterator it = v.begin();
////
////	v.push_back(vec::value_type(1, 2));
////	v.emplace_back(2, 3);
////	v.emplace_back(3, 4);
////	v.size();
////	WCHAR qqq[256];
////	_snwprintf(qqq, 256, L"size: %Iu, capacity: %Iu\n", v.size(), v.capacity());
////	::MessageBoxW(nullptr, qqq, L"test", MB_OK);
////	::MessageBoxW(nullptr, L"body", L"test", MB_OK);
//	return 0;
//}
//
/////=================================================================================================
//namespace {
//
//	typedef void (*FAtExit)(void);
//
//	const int64_t MAX_ATEXITLIST_ENTRIES = 8;
//
//	int64_t atexit_index = MAX_ATEXITLIST_ENTRIES - 1;
//	FAtExit pf_atexitlist[MAX_ATEXITLIST_ENTRIES];
//
//	void init_atexit()
//	{
//	}
//
//	void invoke_atexit()
//	{
//
//		if (atexit_index < 0)
//			atexit_index = 0;
//		else
//			++atexit_index;
//
//		for (int64_t i = atexit_index; i < MAX_ATEXITLIST_ENTRIES; ++i)
//		{
//			(*pf_atexitlist[i])();
//		}
//	}
//
//}
//
///// ========================================================================== Startup (entry point)
//extern "C" {
//
//	int	mainCRTStartup() {
////	int	WinMainCRTStartup() {
//		init_atexit();
//		int Result = 0;
////		STARTUPINFO StartupInfo = {sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
////		::GetStartupInfo(&StartupInfo);
////
////		Result = wWinMain(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(),
////						  StartupInfo.dwFlags & STARTF_USESHOWWINDOW ? StartupInfo.wShowWindow : SW_SHOWDEFAULT);
//		Result = wWmain();
//		invoke_atexit();
//		::ExitProcess(Result);
//		return	Result;
//	}
//
////	BOOL WINAPI	DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID) {
////		switch (dwReason) {
////			case DLL_PROCESS_ATTACH:
////				init_atexit();
////				break;
////
////			case DLL_PROCESS_DETACH:
////				invoke_atexit();
////				break;
////		}
////		return true;
////	}
//
//	int atexit(FAtExit pf)
//	{
//		int64_t ind = ::InterlockedExchangeAdd64(&atexit_index, -1);
//		if (ind >= 0)
//		{
//			pf_atexitlist[ind] = pf;
//			return 0;
//		}
//		return -1;
//	}
//
//	void __cxa_pure_virtual(void)
//	{
////		::abort_message("pure virtual method called");
//	}
//
//}
