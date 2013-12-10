#ifndef LIBBASE_CHRONO_HPP_
#define LIBBASE_CHRONO_HPP_

#include <chrono>

namespace Base {
	namespace chrono {

		struct perfomance_clock {
			typedef std::chrono::nanoseconds duration;
			typedef duration::rep rep;
			typedef duration::period period;
			typedef std::chrono::time_point<perfomance_clock, duration> time_point;

			static constexpr bool is_steady = true;

			static time_point now() noexcept;

		};

	}
}

#endif /* LIBBASE_CHRONO_HPP_ */
