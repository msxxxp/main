#include <tests.hpp>

//#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/chrono>
#include <basis/simstd/mutex>
#include <basis/simstd/vector>

#include <iostream>

template<typename V, typename R>
std::ostream& operator <<(std::ostream& s, const std::chrono::duration<V, R>& d)
{
	s << "[" << d.count() << " of " << R::num << "/" << R::den << "]";
	return s;
}

template<typename C>
void printClockData()
{
	using namespace std;

	cout << "- precision: ";

	// if time unit is less or equal one millisecond
	typedef typename C::period P; // type of time unit
	if (ratio_less_equal<P, milli>::value) {
		// convert to and print as milliseconds
		typedef typename ratio_multiply<P, kilo>::type TT;
		cout << fixed << double(TT::num) / TT::den << " milliseconds" << endl;
	} else {
		// print as seconds
		cout << fixed << double(P::num) / P::den << " seconds" << endl;
	}
	cout << "- is_steady: " << boolalpha << C::is_steady << endl;
}

std::string asString(const std::chrono::system_clock::time_point& tp)
{
	// convert to system time:
	std::time_t t = std::chrono::system_clock::to_time_t(tp);
	std::string ts = std::asctime(gmtime(&t));
//	std::string ts = std::ctime(&t); // convert to calendar time
	ts.resize(ts.size() - 1);         // skip trailing newline
	return ts;
}

void test_chrono()
{
	using std::chrono::duration_cast;
	using std::chrono::microseconds;
	using std::chrono::milliseconds;
	using std::chrono::nanoseconds;
	using std::chrono::steady_clock;

	std::cout << "system_clock: " << std::endl;
	printClockData<std::chrono::system_clock>();

	std::cout << "\nhigh_resolution_clock: " << std::endl;
	printClockData<std::chrono::high_resolution_clock>();

	std::cout << "\nsteady_clock: " << std::endl;
	printClockData<std::chrono::steady_clock>();

	std::cout << "\nperfomance_clock: " << std::endl;
	printClockData<simstd::chrono::perfomance_clock>();

	// print the epoch of this system clock:
	std::chrono::system_clock::time_point tp;
	std::cout << "epoch: " << asString(tp) << std::endl;

	// print current time:
	tp = std::chrono::system_clock::now();
	std::cout << "now:   " << asString(tp) << std::endl;

	// print maximum time of this system clock:
	tp = std::chrono::system_clock::time_point::max();
	std::cout << "max:   " << asString(tp) << std::endl;

	// print minimum time of this system clock:
	tp = std::chrono::system_clock::time_point::min();
	std::cout << "min:   " << asString(tp) << std::endl;

	for (uint64_t size = 1; size < 100000000; size *= 5) {
		auto start1 = std::chrono::steady_clock::now();
//		auto start2 = std::chrono::system_clock::now();
		auto start3 = simstd::chrono::perfomance_clock::now();
		simstd::vector<int> v(size, 42);
		auto end1 = std::chrono::steady_clock::now();
//		auto end2 = std::chrono::system_clock::now();
		auto end3 = simstd::chrono::perfomance_clock::now();

		auto elapsed1 = end1 - start1;
//		auto elapsed2 = end2 - start2;
		auto elapsed3 = end3 - start3;
		LogDebug(L"size1: %I64u, count1: %I64u, count1: %I64u\n", size, (uint64_t )duration_cast<nanoseconds>(elapsed1).count(), (uint64_t )duration_cast<milliseconds>(elapsed1).count());
//		LogDebug(L"size2: %I64u, count2: %I64u, count2: %I64u\n", size, (uint64_t)duration_cast<nanoseconds>(elapsed2).count(), (uint64_t)duration_cast<milliseconds>(elapsed2).count());
		LogDebug(L"size3: %I64u, count3: %I64u, count3: %I64u\n", size, (uint64_t )duration_cast<nanoseconds>(elapsed3).count(), (uint64_t )duration_cast<milliseconds>(elapsed3).count());
	}
}
