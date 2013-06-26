#include <stdio.h>
#include <vector>
#include <algorithm>

class Base {
public:
	Base():
		m_val()
	{
	}

	Base(int val):
		m_val(val)
	{
	}

	int value() const
	{
		return m_val;
	}

	bool operator < (const Base & right) {return m_val < right.m_val;}

private:
	int m_val;
};

class Derived : public Base {
public:
	Derived() {}
	Derived(int val): Base(val) {}
};

int wWinMain(const wchar_t * /*pCmdLine*/) {
	using namespace sarastd;

	typedef vector<Base>           vect;
	typedef vector<Base>::iterator iter;
	int m1[] = {0, 1, 2, 3, 4, 5};
	printf("construct v1\n");
	vect v1;
//	v1.resize(5);
//	v1.resize(10, 9);
	for (int i = 10; i < 30; ++i) {
		printf("v1.capa(): %d, v1.size(): %d\n", v1.capacity(), v1.size());
		v1.push_back(i);
	}
	printf("v1.capa(): %d, v1.size(): %d\n", v1.capacity(), v1.size());
	printf("construct v2\n");
	vect v2(2, Base(77));
	v2.insert(end(v2), rbegin(v1), rend(v1));
	printf("construct v3\n");
	vect v3(vector<int>::size_type(3), Derived(63));
	v2.insert(begin(v2) + 10, v3.begin(), v3.end());
	printf("construct v4\n");
	vect v4(m1, m1 + 6);
	v4.insert(begin(v4), rbegin(m1), rend(m1));
	printf("v4.capa(): %d, v4.size(): %d (", v4.capacity(), v4.size());
	for (iter it = begin(v4); it != end(v4); ++it)
	{
		printf(" %d", it->value());
	}
	printf(" )\n");
	printf("construct v5\n");
	vect v5(v4);
	printf("v1.capa(): %d, v1.size(): %d (", v1.capacity(), v1.size());
	for (iter it = begin(v1); it != end(v1); ++it)
	{
		printf(" %d", it->value());
	}
	printf(" )\n");
	return 0;
	printf("before v2.capa(): %d, v2.size(): %d (", v2.capacity(), v2.size());
	for (iter it = begin(v2); it != end(v2); ++it)
	{
		printf(" %d", it->value());
	}
	printf(" ) mid: %d\n", (begin(v2) + v2.size() / 2)->value());
//	iter mid = sarastd::_quick_sort_split(begin(v2), end(v2));
	sarastd::sort(begin(v2), end(v2));
	printf("after  v2.capa(): %d, v2.size(): %d (", v2.capacity(), v2.size());
	for (iter it = begin(v2); it != end(v2); ++it)
	{
		printf(" %d", it->value());
	}
//	printf(" ) mid: %d\n", mid->value());
	printf(" )\n");
	printf("v3.capa(): %d, v3.size(): %d (", v3.capacity(), v3.size());
	for (iter it = begin(v3); it != end(v3); ++it)
	{
		printf(" %d", it->value());
	}
	printf(" )\n");
//	v1.insert(v1.end(), m1, m1 + 6);

//	v1.insert(v1.cend(), 5, 0);
//	sarastd::vector<int> v2(5);
//	for (const int * it = cbegin(m1); it != cend(m1); ++it)
//	{
//		printf("%d\n", *it);
//	}
	for (vect::reverse_iterator it = rbegin(v5); it != rend(v5); ++it)
	{
		printf(" %d", it->value());
	}
	printf("\n");
//	printf("v5.clear\n");
//	v5.clear();
	printf("v5.erase\n");
	v5.erase(v5.begin() + 2);
	printf("v5.capa(): %d, v5.size(): %d (", v5.capacity(), v5.size());
	for (iter it = begin(v5); it != end(v5); ++it)
	{
		printf(" %d", it->value());
	}
	printf(" )\n");
	printf("v5.insert\n");
//	iter it = v5.insert(v5.begin() + 2, (vect::size_type)1, 2);
//	v5.insert(v5.begin() + 2, 2);
//	v5.insert(v5.begin() + 3, 22);
//	iter it = v5.insert(v5.begin() + 4, 222);
	int m2[] = {2, 22, 222};
	iter it = v5.insert(v5.begin() + 2, m2, m2 + 3);
	printf("v5.capa(): %d, v5.size(): %d (", v5.capacity(), v5.size());
	for (iter it = begin(v5); it != end(v5); ++it)
	{
		printf(" %d", it->value());
	}
	printf(" ) %d\n", it->value());



	printf("exit()\n");
	return 0;
}

#include <windows.h>
extern "C" int	WinMainCRTStartup() {
	int		Result = 0;
	STARTUPINFO StartupInfo = {sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	::GetStartupInfo(&StartupInfo);

	Result = wWinMain(::GetCommandLineW());
	::ExitProcess(Result);
	return	Result;
}

