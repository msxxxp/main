//#include <libbase/wstr.hpp>
//#include <libbase/err.hpp>
//#include <liblog/logger.hpp>
//
//ssize_t Base::wstr::unit_test()
//{
//	LogTrace();
//	Base::wstr str1;
//	Base::wstr str2 = L"str2";
//	Base::wstr str3 = Base::wstr::format(L"%s, %s, %I64u", L"hello", L"world", 36ull);
//	Base::wstr str4 = Base::ErrAsStr(5);
//	Base::wstr str5 = Base::wstr::format(L"%d%d%d%d", 4,4,4,4);
//	Base::wstr str6 = Base::wstr::format(L"%d%d%d%d%d", 5,5,5,5,5);
//
//	LogDebug(L"'%s'\n", str1.c_str());
//	LogDebug(L"'%s'\n", str2.c_str());
//	LogDebug(L"'%s'\n", str3.c_str());
//	LogDebug(L"'%s'\n", str4.c_str());
//	LogDebug(L"'%s'\n", str5.c_str());
//	LogDebug(L"'%s'\n", str6.c_str());
//	return 0;
//}
