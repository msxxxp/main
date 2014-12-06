#include <tests.hpp>

#include <basis/sys/logger.hpp>
#include <basis/simstd/chrono>
#include <basis/simstd/vector>

template<typename C>
void printClockData()
{
	using namespace std;

	// if time unit is less or equal one millisecond
	typedef typename C::period P; // type of time unit
	if (ratio_less_equal<P, milli>::value) {
		// convert to and print as milliseconds
		typedef typename ratio_multiply<P, kilo>::type TT;
		LogReport(L"- precision: %f milliseconds\n", double(TT::num) / TT::den);
	} else {
		// print as seconds
		LogReport(L"- precision: %f seconds\n", double(P::num) / P::den);
	}
	LogReport(L"- is_steady: %d\n", C::is_steady);
}

void out_timepoint(const wchar_t* label, const std::chrono::system_clock::time_point& tp)
{
	std::time_t t = std::chrono::system_clock::to_time_t(tp);
	auto timetm = gmtime(&t);
	LogReportIf(timetm, L"%s GMT %04d-%02d-%02d %02d:%02d:%02d\n", label, timetm->tm_year + 1900, timetm->tm_mon + 1, timetm->tm_mday, timetm->tm_hour, timetm->tm_min, timetm->tm_sec);
//	auto timestr = std::asctime(timetm);
//	LogReportIf(timestr, L"%s %S", label, timestr);
//	auto timestr = std::ctime(&t); // convert to calendar time
//	LogReportIf(timestr, L"%s %S", label, timestr);
}

void test_chrono()
{
	using std::chrono::duration_cast;
	using std::chrono::microseconds;
	using std::chrono::milliseconds;
	using std::chrono::nanoseconds;
	using std::chrono::steady_clock;

	LogReport(L"system_clock: \n");
	printClockData<std::chrono::system_clock>();

	LogReport(L"high_resolution_clock:\n");
	printClockData<std::chrono::high_resolution_clock>();

	LogReport(L"steady_clock:\n");
	printClockData<std::chrono::steady_clock>();

	LogReport(L"perfomance_clock:\n");
	printClockData<simstd::chrono::perfomance_clock>();

	// print the epoch of this system clock:
	std::chrono::system_clock::time_point tp;
	out_timepoint(L"epoch: ", tp);

	// print current time:
	tp = std::chrono::system_clock::now();
	out_timepoint(L"now:   ", tp);

	// print maximum time of this system clock:
	tp = std::chrono::system_clock::time_point::max();
	out_timepoint(L"max:   ", tp);

	// print minimum time of this system clock:
	tp = std::chrono::system_clock::time_point::min();
	out_timepoint(L"min:   ", tp);

	for (uint64_t size = 1; size < 200000000; size *= 10) {
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
		LogReport(L"size1: %I64u, count1: %I64u ns, count1: %I64u ms\n", size, (uint64_t)duration_cast<nanoseconds>(elapsed1).count(), (uint64_t )duration_cast<milliseconds>(elapsed1).count());
//		LogReport(L"size2: %I64u, count2: %I64u ns, count2: %I64u ms\n", size, (uint64_t)duration_cast<nanoseconds>(elapsed2).count(), (uint64_t)duration_cast<milliseconds>(elapsed2).count());
		LogReport(L"size3: %I64u, count3: %I64u ns, count3: %I64u ms\n", size, (uint64_t)duration_cast<nanoseconds>(elapsed3).count(), (uint64_t )duration_cast<milliseconds>(elapsed3).count());
	}
}
