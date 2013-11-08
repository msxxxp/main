#include <libbase/sstr.hpp>
#include <libbase/err.hpp>
#include <liblog/logger.hpp>

ssize_t Base::sstr::unit_test()
{
	LogTrace();
	Base::sstr str1;
	Base::sstr str2 = L"str2";
	Base::sstr str3 = Base::sstr::format(L"%s, %s, %I64u", L"hello", L"world", 36ull);
	Base::sstr str4 = Base::ErrAsStr(5);
	Base::sstr str5 = Base::sstr::format(L"%d%d%d%d", 4,4,4,4);
	Base::sstr str6 = Base::sstr::format(L"%d%d%d%d%d", 5,5,5,5,5);

	LogDebug(L"'%s'\n", str1.c_str());
	LogDebug(L"'%s'\n", str2.c_str());
	LogDebug(L"'%s'\n", str3.c_str());
	LogDebug(L"'%s'\n", str4.c_str());
	LogDebug(L"'%s'\n", str5.c_str());
	LogDebug(L"'%s'\n", str6.c_str());
	return 0;
}
