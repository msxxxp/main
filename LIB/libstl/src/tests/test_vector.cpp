#include <simstd/algorithm>
#include <simstd/vector>
#include <simstd/iterator>

#include <ctime>
#include <cassert>

//namespace {
//	class BaseCl {
//	public:
//		~BaseCl()
//		{
//			printFunc(L"%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl() :
//			m_val()
//		{
//			printFunc(L"%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl(int val) :
//			m_val(val)
//		{
//			printFunc(L"%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl(const BaseCl & other) :
//			m_val(other.m_val)
//		{
//			//printFunc(L"%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl & operator =(const BaseCl & other)
//		{
//			m_val = other.m_val;
//			//printFunc(L"%s\n", __PRETTY_FUNCTION__);
//			return *this;
//		}
//
//		simstd::ssize_t value() const
//		{
//			return m_val;
//		}
//
//		bool operator <(const BaseCl & right) const
//		{
//			return m_val < right.m_val;
//		}
//
//	private:
//		simstd::ssize_t m_val;
//	};
//
//	//bool operator < (const Base & left, const Base & right) {return left.operator <(right);}
//
//	class Derived: public BaseCl {
//	public:
//		Derived()
//		{
//		}
//		Derived(int val) :
//			BaseCl(val)
//		{
//		}
//	};
//}
//
typedef simstd::shared_ptr<ssize_t> Value;
//typedef simstd::movable_vector<Value> vec_t;
//
namespace hh {

	inline ssize_t random0()
	{
//		printFunc(L"%s\n", __PRETTY_FUNCTION__);
		return rand() % 100;
	}

	inline simstd::shared_ptr<ssize_t> random0p()
	{
//		printFunc(L"%s\n", __PRETTY_FUNCTION__);
		return simstd::shared_ptr<ssize_t>(new ssize_t(rand() % 100));
	}

	inline ssize_t random1(ssize_t l)
	{
//		printFunc(L"%s\n", __PRETTY_FUNCTION__);
		return rand() % l;
	}

	bool Less(const Value & a, const Value & b);
}

bool hh::Less(const Value & a, const Value & b)
{
//	printFunc(L"%s\n", __PRETTY_FUNCTION__);
	return *a < *b;
}



struct MyAllocator: public simstd::allocator<size_t>
{
};

class A
{
public:
	~A()
	{
		console::printf(L"%S[%d] %Id\n", __PRETTY_FUNCTION__, __LINE__, _i);
	}

	A():
		_i()
	{
		console::printf(L"%S[%d] %Id\n", __PRETTY_FUNCTION__, __LINE__, _i);
	}

	A(size_t i):
		_i(i)
	{
		console::printf(L"%S[%d] %Id\n", __PRETTY_FUNCTION__, __LINE__, _i);
	}

	A(const A& a):
		_i(a._i)
	{
		console::printf(L"%S[%d] %Id\n", __PRETTY_FUNCTION__, __LINE__, _i);
	}

	A& operator =(const A& a)
	{
		A(a).swap(*this);
		console::printf(L"%S[%d] %Id\n", __PRETTY_FUNCTION__, __LINE__, _i);
		return *this;
	}

	A(A&& a):
		_i()
	{
		swap(a);
		console::printf(L"%S[%d] %Id\n", __PRETTY_FUNCTION__, __LINE__, _i);
	}

	A& operator =(A&& a)
	{
		A(simstd::move(a)).swap(*this);
		console::printf(L"%S[%d] %Id\n", __PRETTY_FUNCTION__, __LINE__, _i);
		return *this;
	}

	void swap(A& a)
	{
		using simstd::swap;
		swap(_i, a._i);
	}

	ssize_t _i;
};


typedef simstd::vector<Value> vec_t;

void print_cont(const wchar_t * name, const simstd::vector<A>& c, simstd::Test::wPrintFunc printFunc)
{
	using namespace simstd;
	printFunc(L"%s: capa(): %Id, size(): %Id (", name, c.capacity(), c.size());
	for (auto it = begin(c); it != end(c); ++it) {
		printFunc(L" %Id", it->_i);
	}
	printFunc(L")\n");
}

ssize_t simstd::Test::_vector(simstd::Test::wPrintFunc printFunc)
{
	using namespace simstd;

	srand(static_cast<unsigned>(time(0)));

	//	{
	////		auto_ptr<Base> a(new Base(5));
	//		Base * b = new Base;
	//		printFunc(L"%s: b: %p\n", __PRETTY_FUNCTION__, b);
	////		shared_ptr<Base> s1(b);
	//		shared_ptr<Base> s1(b, simstd::default_delete<Base[]>());
	//		shared_ptr<Base> s2(s1);
	//		printFunc(L"%s: get(): %p\n", __PRETTY_FUNCTION__, s1.get());
	//		printFunc(L"%s: count(): %Iu\n", __PRETTY_FUNCTION__, s1.use_count());
	//	}
	//	printFunc(L"%s end\n", __PRETTY_FUNCTION__);
	//	return 0;
	A data[] {44, 45, 46, 47, 48};

	printFunc(L"\nconstruct v11\n");
	vector<A> v11;

	printFunc(L"\nconstruct v12\n");
	vector<A> v12 = vector<A>(MyAllocator());

	printFunc(L"\nconstruct v21\n");
	vector<A> v21(11);

	printFunc(L"\nconstruct v22\n");
	vector<A> v22(11, MyAllocator());

	printFunc(L"\nconstruct v31\n");
	vector<A> v31(11, A(444));

	printFunc(L"\nconstruct v32\n");
	vector<A> v32(11, A(555), MyAllocator());

	printFunc(L"\nconstruct v41\n");
	vector<A> v41(begin(data), end(data));

	printFunc(L"\nconstruct v42\n");
	vector<A> v42(begin(data), end(data), MyAllocator());

	printFunc(L"\nconstruct v51\n");
	vector<A> v51(v21);

	printFunc(L"\nconstruct v61\n");
	vector<A> v61(v22, MyAllocator());

	printFunc(L"\nconstruct v71\n");
	vector<A> v71(simstd::move(v31));

	printFunc(L"\nconstruct v81\n");
	vector<A> v81(simstd::move(v32), MyAllocator());

	printFunc(L"construct end\n");

	print_cont(L"v11", v11, printFunc);
	print_cont(L"v12", v12, printFunc);
	print_cont(L"v21", v21, printFunc);
	print_cont(L"v22", v22, printFunc);
	print_cont(L"v31", v31, printFunc);
	print_cont(L"v32", v32, printFunc);
	print_cont(L"v41", v41, printFunc);
	print_cont(L"v42", v42, printFunc);
	print_cont(L"v51", v51, printFunc);
	print_cont(L"v61", v61, printFunc);
	print_cont(L"v71", v71, printFunc);
	print_cont(L"v81", v81, printFunc);

//	return 0;

	vec_t v(10);
	printFunc(L"generate v\n");
	generate(begin(v), end(v), hh::random0p);
	printFunc(L"v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
	for (vec_t::iterator it = begin(v); it != end(v); ++it) {
		printFunc(L" %Id", *it->get());
	}
	printFunc(L" )\n");
	printFunc(L"nth_element:\n");
	simstd::nth_element(begin(v), begin(v) + 4, end(v), hh::Less);
	printFunc(L"v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
	for (vec_t::iterator it = begin(v); it != end(v); ++it) {
		printFunc(L" %Id", *it->get());
	}
	printFunc(L" )\n");
	printFunc(L"sorting back:\n");
	simstd::sort(rbegin(v), rend(v), hh::Less);
	printFunc(L"v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
	for (vec_t::iterator it = begin(v); it != end(v); ++it) {
		printFunc(L" %Id", *it->get());
	}
	printFunc(L" )\n");
	printFunc(L"shuffling:\n");
	simstd::random_shuffle(begin(v), end(v), hh::random1);
	printFunc(L"v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
	for (vec_t::iterator it = begin(v); it != end(v); ++it) {
		printFunc(L" %Id", *it->get());
	}
	printFunc(L" )\n");
	printFunc(L"sorting:\n");
	simstd::sort(begin(v), end(v), hh::Less);
	printFunc(L"v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
	for (vec_t::iterator it = begin(v); it != end(v); ++it) {
		printFunc(L" %Id", *it->get());
	}
	printFunc(L" )\n");
	while (simstd::next_permutation(begin(v), end(v))) {
//		printFunc(L"(");
//		for (vec_t::iterator it = begin(v); it != end(v); ++it)
//		{
//			printFunc(L" %Id", *it->get());
//		}
//		printFunc(L" )\n");
	}

//	return 0;

//	BaseCl b1(1);
//	BaseCl b2(2);
//	BaseCl b3(3);
//
//	movable_vector<BaseCl> mv1;
//	mv1.lock();
//	mv1.push_back(b1);
//	mv1.push_back(b2);
//	mv1.push_back(b3);
//	mv1.push_back(b3);
//	mv1.push_back(b3);
//	mv1.push_back(b3);
//	mv1.unlock();
//	//printFunc(L"mv1.capa(): %Id, mv1.size(): %Id\n", mv1.capacity(), mv1.size());
//
//	typedef vector<BaseCl> vect;
//	typedef vector<BaseCl>::iterator iter;
//	ssize_t m1[] = {0, 1, 2, 3, 4, 5};
//	//printFunc(L"construct v1\n");
//	vect v1;
//	//	v1.resize(5);
//	//	v1.resize(10, 9);
//	for (int i = 10; i < 30; ++i) {
//		//printFunc(L"v1.capa(): %Id, v1.size(): %Id\n", v1.capacity(), v1.size());
//		v1.push_back(i);
//	}
//	//printFunc(L"v1.capa(): %Id, v1.size(): %Id\n", v1.capacity(), v1.size());
//	//printFunc(L"construct v2\n");
//	vect v2(2, BaseCl(77));
//	v2.insert(end(v2), rbegin(v1), rend(v1));
//	//printFunc(L"construct v3\n");
//	vect v3(vector<int>::size_type(3), Derived(63));
//	v2.insert(begin(v2) + 10, v3.begin(), v3.end());
//	//printFunc(L"construct v4\n");
//	vect v4(m1, m1 + 6);
//	v4.insert(begin(v4), rbegin(m1), rend(m1));
//	//printFunc(L"v4.capa(): %Id, v4.size(): %Id (", v4.capacity(), v4.size());
//	for (iter it = begin(v4); it != end(v4); ++it) {
//		//printFunc(L" %Id", it->value());
//	}
//	//printFunc(L" )\n");
//	//printFunc(L"construct v5\n");
//	vect v5(v4);
//	//printFunc(L"v1.capa(): %Id, v1.size(): %Id (", v1.capacity(), v1.size());
//	for (iter it = begin(v1); it != end(v1); ++it) {
//		//printFunc(L" %Id", it->value());
//	}
//	//printFunc(L" )\n");
//	return 0;
//
//	//printFunc(L"before v2.capa(): %Id, v2.size(): %Id (", v2.capacity(), v2.size());
//	for (iter it = begin(v2); it != end(v2); ++it) {
//		//printFunc(L" %Id", it->value());
//	}
//	//printFunc(L" ) mid: %Id\n", (begin(v2) + v2.size() / 2)->value());
//	//	iter mid = simstd::_quick_sort_split(begin(v2), end(v2));
//	simstd::sort(begin(v2), end(v2));
//	//printFunc(L"after  v2.capa(): %Id, v2.size(): %Id (", v2.capacity(), v2.size());
//	for (iter it = begin(v2); it != end(v2); ++it) {
//		//printFunc(L" %Id", it->value());
//	}
//	//	//printFunc(L" ) mid: %Id\n", mid->value());
//	//printFunc(L" )\n");
//	//printFunc(L"v3.capa(): %Id, v3.size(): %Id (", v3.capacity(), v3.size());
//	for (iter it = begin(v3); it != end(v3); ++it) {
//		//printFunc(L" %Id", it->value());
//	}
//	//printFunc(L" )\n");
//	//	v1.insert(v1.end(), m1, m1 + 6);
//
//	//	v1.insert(v1.cend(), 5, 0);
//	//	simstd::vector<int> v2(5);
//	//	for (const int * it = cbegin(m1); it != cend(m1); ++it)
//	//	{
//	//		//printFunc(L"%Id\n", *it);
//	//	}
//	for (vect::reverse_iterator it = rbegin(v5); it != rend(v5); ++it) {
//		//printFunc(L" %Id", it->value());
//	}
//	//printFunc(L"\n");
//	//	//printFunc(L"v5.clear\n");
//	//	v5.clear();
//	//printFunc(L"v5.erase\n");
//	v5.erase(v5.begin() + 2);
//	//printFunc(L"v5.capa(): %Id, v5.size(): %Id (", v5.capacity(), v5.size());
//	for (iter it = begin(v5); it != end(v5); ++it) {
//		//printFunc(L" %Id", it->value());
//	}
//	//printFunc(L" )\n");
//	//printFunc(L"v5.insert\n");
//	//	iter it = v5.insert(v5.begin() + 2, (vect::size_type)1, 2);
//	//	v5.insert(v5.begin() + 2, 2);
//	//	v5.insert(v5.begin() + 3, 22);
//	//	iter it = v5.insert(v5.begin() + 4, 222);
//	//	int m2[] = {2, 22, 222};
//	//	iter it = v5.insert(v5.begin() + 2, m2, m2 + 3);
//	//printFunc(L"v5.capa(): %Id, v5.size(): %Id (", v5.capacity(), v5.size());
//	for (iter it = begin(v5); it != end(v5); ++it) {
//		//printFunc(L" %Id", it->value());
//	}
//	//printFunc(L" ) %Id\n", it->value());
	return 0;
}
