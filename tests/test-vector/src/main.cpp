#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cassert>

#define NO_STD_NEW

#include <simstl/algorithm>
#include <simstl/vector>
#include <simstl/memory>
#include <simstl/iterator>
#include <simstl/string>


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
typedef sarastd::movable_vector<Value> vec_t;

namespace hh {

	inline ssize_t random0()
	{
//		printf("%s\n", __PRETTY_FUNCTION__);
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

	bool Less(const Value & a, const Value & b);
}

void test_vector()
{
	using namespace sarastd;
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
	sarastd::nth_element(begin(v), begin(v) + 4, end(v), hh::Less);
	printf("v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
	for (vec_t::iterator it = begin(v); it != end(v); ++it)
	{
		printf(" %Id", *it->get());
	}
	printf(" )\n");
	printf("sorting back:\n");
	sarastd::sort(rbegin(v), rend(v), hh::Less);
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
	sarastd::sort(begin(v), end(v), hh::Less);
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

	return;

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
	return;

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
}

inline int Str_compare(const char * in1, const char * in2) {
	return ::strcmp(in1, in2);
}

void test_string()
{
	using namespace sarastd;

	const char * cc = "const char *";

	string a;
	string b("qwerty12345");
	string c(b);

	printf("cc: '%s'\n", cc);

	printf("a (%Iu, %Iu): '%s'\n", a.size(), a.capacity(), a.c_str());
	printf("b (%Iu, %Iu): '%s'\n", b.size(), b.capacity(), b.c_str());

	a = b;
	printf("c (%Iu, %Iu): '%s'\n", c.size(), c.capacity(), c.c_str());
	printf("a (%Iu, %Iu): '%s'\n", a.size(), a.capacity(), a.c_str());

	string str1(5, 'h');
	printf("str1: '%s' size: %d\n", str1.c_str(), (int)str1.size());
	assert(Str_compare(str1.c_str(), "hhhhh") == 0);
	assert(str1.size() == 5);
	assert(*(str1.cend()-1) == 'h');
	assert(*str1.cend() == 0);
	assert(*str1.cbegin() == 'h');

	const char * pcwstr = "PCWSTR";
	string str2(pcwstr);
	printf("str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(Str_compare(str2.c_str(), pcwstr) == 0);
	assert(str2.size() == 6);
	assert(*(str2.cend()-1) == 'R');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == 'P');

	char * pwstr = (char *)"1pwstr2";
	string str3 = pwstr;
	printf("str3: '%s' size: %d\n\n", str3.c_str(), (int)str3.size());
	assert(Str_compare(str3.c_str(), pwstr) == 0);
	assert(str3.size() == 7);
	assert(*(str3.cend()-1) == '2');
	assert(*str3.cend() == 0);
	assert(*str3.cbegin() == '1');

	str1 = "que1";
	printf("str1: '%s' size: %d\n", str1.c_str(), (int)str1.size());
	assert(Str_compare(str1.c_str(), "que1") == 0);
	assert(str1.size() == 4);
	assert(*(str1.cend()-1) == '1');
	assert(*str1.cend() == 0);
	assert(*str1.cbegin() == 'q');

	str2 = str1;
	printf("str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(Str_compare(str2.c_str(), "que1") == 0);
	assert(str2.size() == 4);
	assert(*(str2.cend()-1) == '1');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == 'q');
	assert(str1.c_str() == str2.c_str());

	str3.clear();
	printf("str3: '%s' size: %d\n\n", str3.c_str(), (int)str3.size());
	assert(Str_compare(str3.c_str(), "") == 0);
	assert(str3.size() == 0);
//		assert(*(str3.cend()-1) == '2');
	assert(*str3.cend() == 0);
	assert(*str3.cbegin() == 0);
	assert(str3.empty());
	assert(str3.cbegin() == str3.cend());

	str2.append(str1);
	printf("str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(Str_compare(str2.c_str(), "que1que1") == 0);
	assert(str2.size() == 8);
	assert(*(str2.cend()-1) == '1');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == 'q');
	assert(str1.c_str() != str2.c_str());

	str2.replace(4, 0, "test2");
	printf("str2: '%s' size: %d\n", str2.c_str(), (int)str2.size());
	assert(Str_compare(str2.c_str(), "que1test2que1") == 0);
	assert(str2.size() == 13);
	assert(*(str2.cend()-1) == '1');
	assert(*str2.cend() == 0);
	assert(*str2.cbegin() == 'q');
	assert(str2.find("tes") == 4);
	assert(str2.find("teqs") == string::npos);
	assert(str2.find("ue") == 1);
	assert(str2.rfind('u') == 10);
	assert(str2.find_first_of("12") == 3);
	assert(str2.find_first_not_of("12") == 0);
	assert(str2.find_first_of("21") == 3);
	assert(str2.find_first_not_of("21") == 0);
	assert(str2.find_first_not_of("que1") == 4);
	assert(str2.find_last_of("12") == 12);
	assert(str2.find_last_of("2d") == 8);
	assert(str2.find_last_not_of("12") == 11);
	assert(str2.find_last_not_of("2d") == 12);

	str2.replace(4, 5, "TEST3");
	printf("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Str_compare(str2.c_str(), "que1TEST3que1") == 0);
	assert(str2.size() == 13);

	str2.replace(0, 512, "T4");
	printf("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Str_compare(str2.c_str(), "T4") == 0);
	assert(str2.size() == 2);

	str2.replace(2, 128, "testing5");
	printf("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Str_compare(str2.c_str(), "T4testing5") == 0);
	assert(str2.size() == 10);

	str2.replace(0, 1024, "");
	printf("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Str_compare(str2.c_str(), "") == 0);
	assert(str2.size() == 0);

	str2.replace(0, 1, "again6");
	printf("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Str_compare(str2.c_str(), "again6") == 0);
	assert(str2.size() == 6);

	str2.erase(4, 20);
	printf("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Str_compare(str2.c_str(), "agai") == 0);
	assert(str2.size() == 4);

	str2.erase(str2.cbegin() + 2);
	printf("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	assert(Str_compare(str2.c_str(), "agi") == 0);
	assert(str2.size() == 3);

	str2.append(str2);
	printf("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
	str2.append(str2);
	printf("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());

	const char * qqq = str2.c_str() + 3;
	str2.append(qqq, 5);
	printf("str2: '%s' size: %d capacity: %d\n", str2.c_str(), (int)str2.size(), (int)str2.capacity());
}

int wWinMain(const wchar_t * /*pCmdLine*/) {
	printf("%s begin\n", __PRETTY_FUNCTION__);

	srand(time(0));

	test_string();

	//printf("exit()\n");
	return 0;
}

extern "C" {
	void* _system_malloc(sarastd::size_t size)
	{
		return malloc(size);
	}

	void _system_free(void * ptr)
	{
		free(ptr);
	}

	void* _system_movable_malloc(sarastd::size_t size)
	{
		return malloc(size);
	}

	void _system_movable_free(void * handle)
	{
		free(handle);
	}

	void* _system_movable_lock(void * handle)
	{
		return handle;
	}

	void _system_movable_unlock(void * /*handle*/)
	{
	}
}

bool hh::Less(const Value & a, const Value & b)
{
//	printf("%s\n", __PRETTY_FUNCTION__);
	return *a < *b;
}
