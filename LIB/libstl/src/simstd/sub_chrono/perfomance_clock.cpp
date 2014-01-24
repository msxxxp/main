#include <system/configure.hpp>
#include <simstd/chrono>

namespace simstd {
	namespace chrono {

		namespace {
			LARGE_INTEGER get_frequency()
			{
				LARGE_INTEGER ret;
				::QueryPerformanceFrequency(&ret);
				return ret;
			}
		}

		perfomance_clock::time_point perfomance_clock::now() noexcept
		{
			static LARGE_INTEGER perfomanceFrequency = get_frequency();    // Counts per second

			LARGE_INTEGER perfomanceNow;
			::QueryPerformanceCounter(&perfomanceNow);

			using std::chrono::duration_cast;
			using std::chrono::nanoseconds;
			using std::chrono::seconds;
			return time_point(duration((perfomanceNow.QuadPart * duration_cast<nanoseconds>(seconds(1))) / perfomanceFrequency.QuadPart));
		}

	}    // namespace chrono
}    // namespace Base
