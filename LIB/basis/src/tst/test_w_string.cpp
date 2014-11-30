#include <basis/sys/cstr.hpp>
#include <basis/simstd/string>

//#include <cassert>
//#include <string>

ssize_t simstd::Test::w_string(simstd::Test::wPrintFunc /*printFunc*/)
{
/*
	wstring str1(5, L'h');
	printFunc(L"str1: '%s' size: %d\n", str1.c_str(), (int)str1.size());
	assert(cstr::compare(str1.c_str(), L"hhhhh") == 0);
	assert(str1.size() == 5);
	assert(*(str1.cend()-1) == L'h');
	assert(*str1.cend() == 0);
	assert(*str1.cbegin() == L'h');

	const wchar_t* pcwstr = L"PCWSTR";
	wstring str2(pcwstr);
	printFunc(L"str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(cstr::compare(str2.c_str(), pcwstr) == 0);
	assert(str2.size() == 6);
	assert(*(str2.cend()-1) == L'R');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == L'P');

	wchar_t* pwstr = (wchar_t *)L"1pwstr2";
	wstring str3 = pwstr;
	printFunc(L"str3: '%s' size: %d\n\n", str3.c_str(), (int)str3.size());
	assert(cstr::compare(str3.c_str(), pwstr) == 0);
	assert(str3.size() == 7);
	assert(*(str3.cend()-1) == L'2');
	assert(*str3.cend() == 0);
	assert(*str3.cbegin() == L'1');

	str1 = L"que1";
	printFunc(L"str1: '%s' size: %d\n", str1.c_str(), (int)str1.size());
	assert(cstr::compare(str1.c_str(), L"que1") == 0);
	assert(str1.size() == 4);
	assert(*(str1.cend()-1) == L'1');
	assert(*str1.cend() == 0);
	assert(*str1.cbegin() == L'q');

	str2 = str1;
	printFunc(L"str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(cstr::compare(str2.c_str(), L"que1") == 0);
	assert(str2.size() == 4);
	assert(*(str2.cend()-1) == L'1');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == L'q');
	assert(str1.c_str() == str2.c_str());

	str3.clear();
	printFunc(L"str3: '%s' size: %d\n\n", str3.c_str(), (int)str3.size());
	assert(cstr::compare(str3.c_str(), L"") == 0);
	assert(str3.size() == 0);
	//		assert(*(str3.cend()-1) == L'2');
	assert(*str3.cend() == 0);
	assert(*str3.cbegin() == 0);
	assert(str3.empty());
	assert(str3.cbegin() == str3.cend());

	str2.append(str1);
	printFunc(L"str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(cstr::compare(str2.c_str(), L"que1que1") == 0);
	assert(str2.size() == 8);
	assert(*(str2.cend()-1) == L'1');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == L'q');
	assert(str1.c_str() != str2.c_str());

	str2.replace(4, 0, L"test2");
	printFunc(L"str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(cstr::compare(str2.c_str(), L"que1test2que1") == 0);
	assert(str2.size() == 13);
	assert(*(str2.cend()-1) == L'1');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == L'q');
	assert(str2.find(L"tes") == 4);
	assert(str2.find(L"teqs") == wstring::npos);
	assert(str2.find(L"ue") == 1);
	assert(str2.rfind(L'u') == 10);
	assert(str2.find_first_of(L"12") == 3);
	assert(str2.find_first_not_of(L"12") == 0);
	assert(str2.find_first_of(L"21") == 3);
	assert(str2.find_first_not_of(L"21") == 0);
	assert(str2.find_first_not_of(L"que1") == 4);
	assert(str2.find_last_of(L"12") == 12);
	assert(str2.find_last_of(L"2d") == 8);
	assert(str2.find_last_not_of(L"12") == 11);
	assert(str2.find_last_not_of(L"2d") == 12);

	str2.replace(4, 5, L"TEST3");
	printFunc(L"str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(cstr::compare(str2.c_str(), L"que1TEST3que1") == 0);
	assert(str2.size() == 13);

	str2.replace(0, 512, L"T4");
	printFunc(L"str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(cstr::compare(str2.c_str(), L"T4") == 0);
	assert(str2.size() == 2);

	str2.replace(2, 128, L"testing5");
	printFunc(L"str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(cstr::compare(str2.c_str(), L"T4testing5") == 0);
	assert(str2.size() == 10);

	str2.replace(0, 1024, L"");
	printFunc(L"str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(cstr::compare(str2.c_str(), L"") == 0);
	assert(str2.size() == 0);

	str2.replace(0, 1, L"again6");
	printFunc(L"str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(cstr::compare(str2.c_str(), L"again6") == 0);
	assert(str2.size() == 6);

*/
	return 0;
}
