#include <libbase/wstr.hpp>
#include <liblog/logger.hpp>

#include <algorithm>

ssize_t Base::mstring::unit_test()
{
	LogTrace();

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

	LogForce(L"size: %Iu, capa: %Iu\n", mstr3.size(), mstr3.capacity());
	for (size_t i = 0; i < mstr3.size(); ++i)
		LogForce(L"'%s'\n", mstr3[i]);

	LogForce(L"size: %Iu, capa: %Iu\n", mstr4.size(), mstr4.capacity());
	for (size_t i = 0; i < mstr4.size(); ++i)
		LogForce(L"'%s'\n", mstr4[i]);

	LogForce(L"size: %Iu, capa: %Iu\n", mstr5.size(), mstr5.capacity());
	for (size_t i = 0; i < mstr5.size(); ++i)
		LogForce(L"'%s'\n", mstr5[i]);

	return 0;
}
