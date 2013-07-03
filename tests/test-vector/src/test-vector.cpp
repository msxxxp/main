#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <simstl/algorithm>
#include <simstl/vector>
#include <simstl/memory>

class Base {
public:
	~Base()
	{
		printf("%s\n", __PRETTY_FUNCTION__);
	}

	Base():
		m_val()
	{
		printf("%s\n", __PRETTY_FUNCTION__);
	}

	Base(int val):
		m_val(val)
	{
		printf("%s\n", __PRETTY_FUNCTION__);
	}

	Base(const Base & other):
		m_val(other.m_val)
	{
		//printf("%s\n", __PRETTY_FUNCTION__);
	}

	Base & operator =(const Base & other)
	{
		m_val = other.m_val;
		//printf("%s\n", __PRETTY_FUNCTION__);
		return *this;
	}

	sarastd::ssize_t value() const
	{
		return m_val;
	}

	bool operator < (const Base & right) const {return m_val < right.m_val;}

private:
	sarastd::ssize_t m_val;
};

//bool operator < (const Base & left, const Base & right) {return left.operator <(right);}

class Derived : public Base {
public:
	Derived() {}
	Derived(int val): Base(val) {}
};

typedef sarastd::shared_ptr<ssize_t> Value;
typedef sarastd::vector<Value> vec_t;

bool Less(const Value & a, const Value & b)
{
	return *a < *b;
}

namespace hh {
	inline ssize_t random0()
	{
		printf("%s\n", __PRETTY_FUNCTION__);
		return rand() % 100;
	}

	inline sarastd::shared_ptr<ssize_t> random0p()
	{
//		printf("%s\n", __PRETTY_FUNCTION__);
		return sarastd::shared_ptr<ssize_t>(new ssize_t(rand() % 100));
	}

	inline ssize_t random1(ssize_t l)
	{
//		printf("%s\n", __PRETTY_FUNCTION__);
		return rand() % l;
	}

}


int wWinMain(const wchar_t * /*pCmdLine*/) {
	using namespace sarastd;
	printf("%s begin\n", __PRETTY_FUNCTION__);

//	{
////		auto_ptr<Base> a(new Base(5));
//		Base * b = new Base;
//		printf("%s: b: %p\n", __PRETTY_FUNCTION__, b);
////		shared_ptr<Base> s1(b);
//		shared_ptr<Base> s1(b, sarastd::default_delete<Base[]>());
//		shared_ptr<Base> s2(s1);
//		printf("%s: get(): %p\n", __PRETTY_FUNCTION__, s1.get());
//		printf("%s: count(): %Iu\n", __PRETTY_FUNCTION__, s1.use_count());
//	}
//	printf("%s end\n", __PRETTY_FUNCTION__);
//	return 0;

	srand(time(0));

	printf("construct v\n");
	vec_t v(10);
	printf("generate v\n");
	sarastd::generate(begin(v), end(v), hh::random0p);
	printf("v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
	for (vec_t::iterator it = begin(v); it != end(v); ++it)
	{
		printf(" %Id", *it->get());
	}
	printf(" )\n");
	printf("nth_element:\n");
	sarastd::nth_element(begin(v), begin(v) + 4, end(v), Less);
	printf("v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
	for (vec_t::iterator it = begin(v); it != end(v); ++it)
	{
		printf(" %Id", *it->get());
	}
	printf(" )\n");
	printf("sorting back:\n");
	sarastd::sort(rbegin(v), rend(v), Less);
	printf("v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
	for (vec_t::iterator it = begin(v); it != end(v); ++it)
	{
		printf(" %Id", *it->get());
	}
	printf(" )\n");
	printf("shuffling:\n");
	sarastd::random_shuffle(begin(v), end(v), hh::random1);
	printf("v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
	for (vec_t::iterator it = begin(v); it != end(v); ++it)
	{
		printf(" %Id", *it->get());
	}
	printf(" )\n");
	printf("sorting:\n");
	sarastd::sort(begin(v), end(v), Less);
	printf("v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
	for (vec_t::iterator it = begin(v); it != end(v); ++it)
	{
		printf(" %Id", *it->get());
	}
	printf(" )\n");
	while (sarastd::next_permutation(begin(v), end(v))) {
//		printf("(");
//		for (vec_t::iterator it = begin(v); it != end(v); ++it)
//		{
//			printf(" %Id", *it->get());
//		}
//		printf(" )\n");
	}

	return 0;

	Base b1(1);
	Base b2(2);
	Base b3(3);

	movable_vector<Base> mv1;
	mv1.lock();
	mv1.push_back(b1);
	mv1.push_back(b2);
	mv1.push_back(b3);
	mv1.push_back(b3);
	mv1.push_back(b3);
	mv1.push_back(b3);
	mv1.unlock();
	//printf("mv1.capa(): %Id, mv1.size(): %Id\n", mv1.capacity(), mv1.size());


	typedef vector<Base>           vect;
	typedef vector<Base>::iterator iter;
	ssize_t m1[] = {0, 1, 2, 3, 4, 5};
	//printf("construct v1\n");
	vect v1;
	//	v1.resize(5);
	//	v1.resize(10, 9);
	for (int i = 10; i < 30; ++i) {
		//printf("v1.capa(): %Id, v1.size(): %Id\n", v1.capacity(), v1.size());
		v1.push_back(i);
	}
	//printf("v1.capa(): %Id, v1.size(): %Id\n", v1.capacity(), v1.size());
	//printf("construct v2\n");
	vect v2(2, Base(77));
	v2.insert(end(v2), rbegin(v1), rend(v1));
	//printf("construct v3\n");
	vect v3(vector<int>::size_type(3), Derived(63));
	v2.insert(begin(v2) + 10, v3.begin(), v3.end());
	//printf("construct v4\n");
	vect v4(m1, m1 + 6);
	v4.insert(begin(v4), rbegin(m1), rend(m1));
	//printf("v4.capa(): %Id, v4.size(): %Id (", v4.capacity(), v4.size());
	for (iter it = begin(v4); it != end(v4); ++it)
	{
		//printf(" %Id", it->value());
	}
	//printf(" )\n");
	//printf("construct v5\n");
	vect v5(v4);
	//printf("v1.capa(): %Id, v1.size(): %Id (", v1.capacity(), v1.size());
	for (iter it = begin(v1); it != end(v1); ++it)
	{
		//printf(" %Id", it->value());
	}
	//printf(" )\n");
	return 0;
	//printf("before v2.capa(): %Id, v2.size(): %Id (", v2.capacity(), v2.size());
	for (iter it = begin(v2); it != end(v2); ++it)
	{
		//printf(" %Id", it->value());
	}
	//printf(" ) mid: %Id\n", (begin(v2) + v2.size() / 2)->value());
	//	iter mid = sarastd::_quick_sort_split(begin(v2), end(v2));
	sarastd::sort(begin(v2), end(v2));
	//printf("after  v2.capa(): %Id, v2.size(): %Id (", v2.capacity(), v2.size());
	for (iter it = begin(v2); it != end(v2); ++it)
	{
		//printf(" %Id", it->value());
	}
	//	//printf(" ) mid: %Id\n", mid->value());
	//printf(" )\n");
	//printf("v3.capa(): %Id, v3.size(): %Id (", v3.capacity(), v3.size());
	for (iter it = begin(v3); it != end(v3); ++it)
	{
		//printf(" %Id", it->value());
	}
	//printf(" )\n");
	//	v1.insert(v1.end(), m1, m1 + 6);

	//	v1.insert(v1.cend(), 5, 0);
	//	sarastd::vector<int> v2(5);
	//	for (const int * it = cbegin(m1); it != cend(m1); ++it)
	//	{
	//		//printf("%Id\n", *it);
	//	}
	for (vect::reverse_iterator it = rbegin(v5); it != rend(v5); ++it)
	{
		//printf(" %Id", it->value());
	}
	//printf("\n");
	//	//printf("v5.clear\n");
	//	v5.clear();
	//printf("v5.erase\n");
	v5.erase(v5.begin() + 2);
	//printf("v5.capa(): %Id, v5.size(): %Id (", v5.capacity(), v5.size());
	for (iter it = begin(v5); it != end(v5); ++it)
	{
		//printf(" %Id", it->value());
	}
	//printf(" )\n");
	//printf("v5.insert\n");
	//	iter it = v5.insert(v5.begin() + 2, (vect::size_type)1, 2);
	//	v5.insert(v5.begin() + 2, 2);
	//	v5.insert(v5.begin() + 3, 22);
	//	iter it = v5.insert(v5.begin() + 4, 222);
	//	int m2[] = {2, 22, 222};
	//	iter it = v5.insert(v5.begin() + 2, m2, m2 + 3);
	//printf("v5.capa(): %Id, v5.size(): %Id (", v5.capacity(), v5.size());
	for (iter it = begin(v5); it != end(v5); ++it)
	{
		//printf(" %Id", it->value());
	}
	//printf(" ) %Id\n", it->value());



	//printf("exit()\n");
	return 0;
}


#include <windows.h>
#ifndef DEBUG
extern "C" void __cxa_pure_virtual(void)
{
	printf("%s\n", __PRETTY_FUNCTION__);
	//		::abort_message("pure virtual method called");
}

extern "C" int	WinMainCRTStartup() {
	int		Result = 0;
	STARTUPINFO StartupInfo = {sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	::GetStartupInfo(&StartupInfo);

	printf("%s\n", __PRETTY_FUNCTION__);
	Result = wWinMain(::GetCommandLineW());
	::ExitProcess(Result);
	return	Result;
}
#else
int main()
{
	return wWinMain(::GetCommandLineW());
}
#endif
