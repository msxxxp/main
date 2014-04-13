#include <system/thread.hpp>

namespace thread {

	const wchar_t * to_str(Priority prio)
	{
		switch (prio) {
			case Priority::IDLE:
				return L"Idle";
			case Priority::LOWEST:
				return L"Lowest";
			case Priority::BELOW_NORMAL:
				return L"Below normal";
			case Priority::NORMAL:
				return L"Normal";
			case Priority::ABOVE_NORMAL:
				return L"Above normal";
			case Priority::HIGHEST:
				return L"Highest";
			case Priority::TIME_CRITICAL:
				return L"Time critical";
		}
		return L"unknown";
	}

	const wchar_t * to_str(IoPriority prio)
	{
		switch (prio) {
			case IoPriority::VERY_LOW:
				return L"Very low";
			case IoPriority::LOW:
				return L"Low";
			case IoPriority::NORMAL:
				return L"Normal";
			case IoPriority::HIGH:
				return L"High";
			case IoPriority::CRITICAL:
				return L"Critical";
		}
		return L"unknown";
	}

}
