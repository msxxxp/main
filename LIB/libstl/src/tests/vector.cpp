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
//			printFunc("%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl() :
//			m_val()
//		{
//			printFunc("%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl(int val) :
//			m_val(val)
//		{
//			printFunc("%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl(const BaseCl & other) :
//			m_val(other.m_val)
//		{
//			//printFunc("%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl & operator =(const BaseCl & other)
//		{
//			m_val = other.m_val;
//			//printFunc("%s\n", __PRETTY_FUNCTION__);
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
//typedef simstd::shared_ptr<ssize_t> Value;
//typedef simstd::movable_vector<Value> vec_t;
//
//namespace hh {
//
//	inline ssize_t random0()
//	{
////		printFunc("%s\n", __PRETTY_FUNCTION__);
//		return rand() % 100;
//	}
//
//	inline simstd::shared_ptr<ssize_t> random0p()
//	{
////		printFunc("%s\n", __PRETTY_FUNCTION__);
//		return simstd::shared_ptr<ssize_t>(new ssize_t(rand() % 100));
//	}
//
//	inline ssize_t random1(ssize_t l)
//	{
////		printFunc("%s\n", __PRETTY_FUNCTION__);
//		return rand() % l;
//	}
//
//	bool Less(const Value & a, const Value & b);
//}
//
//bool hh::Less(const Value & a, const Value & b)
//{
////	printFunc("%s\n", __PRETTY_FUNCTION__);
//	return *a < *b;
//}

ssize_t simstd::Test::_vector(simstd::Test::aPrintFunc /*printFunc*/)
{
//	srand(time(0));
//
//	//	{
//	////		auto_ptr<Base> a(new Base(5));
//	//		Base * b = new Base;
//	//		printFunc("%s: b: %p\n", __PRETTY_FUNCTION__, b);
//	////		shared_ptr<Base> s1(b);
//	//		shared_ptr<Base> s1(b, simstd::default_delete<Base[]>());
//	//		shared_ptr<Base> s2(s1);
//	//		printFunc("%s: get(): %p\n", __PRETTY_FUNCTION__, s1.get());
//	//		printFunc("%s: count(): %Iu\n", __PRETTY_FUNCTION__, s1.use_count());
//	//	}
//	//	printFunc("%s end\n", __PRETTY_FUNCTION__);
//	//	return 0;
//
//	printFunc("construct v\n");
//	vec_t v(10);
//	printFunc("generate v\n");
//	simstd::generate(begin(v), end(v), hh::random0p);
//	printFunc("v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
//	for (vec_t::iterator it = begin(v); it != end(v); ++it) {
//		printFunc(" %Id", *it->get());
//	}
//	printFunc(" )\n");
//	printFunc("nth_element:\n");
//	simstd::nth_element(begin(v), begin(v) + 4, end(v), hh::Less);
//	printFunc("v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
//	for (vec_t::iterator it = begin(v); it != end(v); ++it) {
//		printFunc(" %Id", *it->get());
//	}
//	printFunc(" )\n");
//	printFunc("sorting back:\n");
//	simstd::sort(rbegin(v), rend(v), hh::Less);
//	printFunc("v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
//	for (vec_t::iterator it = begin(v); it != end(v); ++it) {
//		printFunc(" %Id", *it->get());
//	}
//	printFunc(" )\n");
//	printFunc("shuffling:\n");
//	simstd::random_shuffle(begin(v), end(v), hh::random1);
//	printFunc("v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
//	for (vec_t::iterator it = begin(v); it != end(v); ++it) {
//		printFunc(" %Id", *it->get());
//	}
//	printFunc(" )\n");
//	printFunc("sorting:\n");
//	simstd::sort(begin(v), end(v), hh::Less);
//	printFunc("v.capa(): %Id, v.size(): %Id (", v.capacity(), v.size());
//	for (vec_t::iterator it = begin(v); it != end(v); ++it) {
//		printFunc(" %Id", *it->get());
//	}
//	printFunc(" )\n");
//	while (simstd::next_permutation(begin(v), end(v))) {
////		printFunc("(");
////		for (vec_t::iterator it = begin(v); it != end(v); ++it)
////		{
////			printFunc(" %Id", *it->get());
////		}
////		printFunc(" )\n");
//	}
//
//	return 0;
//
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
//	//printFunc("mv1.capa(): %Id, mv1.size(): %Id\n", mv1.capacity(), mv1.size());
//
//	typedef vector<BaseCl> vect;
//	typedef vector<BaseCl>::iterator iter;
//	ssize_t m1[] = {0, 1, 2, 3, 4, 5};
//	//printFunc("construct v1\n");
//	vect v1;
//	//	v1.resize(5);
//	//	v1.resize(10, 9);
//	for (int i = 10; i < 30; ++i) {
//		//printFunc("v1.capa(): %Id, v1.size(): %Id\n", v1.capacity(), v1.size());
//		v1.push_back(i);
//	}
//	//printFunc("v1.capa(): %Id, v1.size(): %Id\n", v1.capacity(), v1.size());
//	//printFunc("construct v2\n");
//	vect v2(2, BaseCl(77));
//	v2.insert(end(v2), rbegin(v1), rend(v1));
//	//printFunc("construct v3\n");
//	vect v3(vector<int>::size_type(3), Derived(63));
//	v2.insert(begin(v2) + 10, v3.begin(), v3.end());
//	//printFunc("construct v4\n");
//	vect v4(m1, m1 + 6);
//	v4.insert(begin(v4), rbegin(m1), rend(m1));
//	//printFunc("v4.capa(): %Id, v4.size(): %Id (", v4.capacity(), v4.size());
//	for (iter it = begin(v4); it != end(v4); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" )\n");
//	//printFunc("construct v5\n");
//	vect v5(v4);
//	//printFunc("v1.capa(): %Id, v1.size(): %Id (", v1.capacity(), v1.size());
//	for (iter it = begin(v1); it != end(v1); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" )\n");
//	return 0;
//
//	//printFunc("before v2.capa(): %Id, v2.size(): %Id (", v2.capacity(), v2.size());
//	for (iter it = begin(v2); it != end(v2); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" ) mid: %Id\n", (begin(v2) + v2.size() / 2)->value());
//	//	iter mid = simstd::_quick_sort_split(begin(v2), end(v2));
//	simstd::sort(begin(v2), end(v2));
//	//printFunc("after  v2.capa(): %Id, v2.size(): %Id (", v2.capacity(), v2.size());
//	for (iter it = begin(v2); it != end(v2); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//	//printFunc(" ) mid: %Id\n", mid->value());
//	//printFunc(" )\n");
//	//printFunc("v3.capa(): %Id, v3.size(): %Id (", v3.capacity(), v3.size());
//	for (iter it = begin(v3); it != end(v3); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" )\n");
//	//	v1.insert(v1.end(), m1, m1 + 6);
//
//	//	v1.insert(v1.cend(), 5, 0);
//	//	simstd::vector<int> v2(5);
//	//	for (const int * it = cbegin(m1); it != cend(m1); ++it)
//	//	{
//	//		//printFunc("%Id\n", *it);
//	//	}
//	for (vect::reverse_iterator it = rbegin(v5); it != rend(v5); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc("\n");
//	//	//printFunc("v5.clear\n");
//	//	v5.clear();
//	//printFunc("v5.erase\n");
//	v5.erase(v5.begin() + 2);
//	//printFunc("v5.capa(): %Id, v5.size(): %Id (", v5.capacity(), v5.size());
//	for (iter it = begin(v5); it != end(v5); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" )\n");
//	//printFunc("v5.insert\n");
//	//	iter it = v5.insert(v5.begin() + 2, (vect::size_type)1, 2);
//	//	v5.insert(v5.begin() + 2, 2);
//	//	v5.insert(v5.begin() + 3, 22);
//	//	iter it = v5.insert(v5.begin() + 4, 222);
//	//	int m2[] = {2, 22, 222};
//	//	iter it = v5.insert(v5.begin() + 2, m2, m2 + 3);
//	//printFunc("v5.capa(): %Id, v5.size(): %Id (", v5.capacity(), v5.size());
//	for (iter it = begin(v5); it != end(v5); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" ) %Id\n", it->value());
	return 0;
}
