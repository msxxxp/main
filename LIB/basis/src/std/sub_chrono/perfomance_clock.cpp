#include <basis/sys/configure.hpp>
#include <basis/std/chrono>

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

			return time_point(duration((perfomanceNow.QuadPart * std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds(1))) / perfomanceFrequency.QuadPart));
		}

	}    // namespace chrono
}    // namespace simstd
