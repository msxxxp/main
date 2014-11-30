#include <basis/sys/thread.hpp>

namespace totext {

	const wchar_t* c_str(thread::Priority prio)
	{
		switch (prio) {
			case thread::Priority::IDLE:
				return L"Idle";
			case thread::Priority::LOWEST:
				return L"Lowest";
			case thread::Priority::BELOW_NORMAL:
				return L"Below normal";
			case thread::Priority::NORMAL:
				return L"Normal";
			case thread::Priority::ABOVE_NORMAL:
				return L"Above normal";
			case thread::Priority::HIGHEST:
				return L"Highest";
			case thread::Priority::TIME_CRITICAL:
				return L"Time critical";
		}
		return L"unknown";
	}

	const wchar_t* c_str(thread::IoPriority prio)
	{
		switch (prio) {
			case thread::IoPriority::VERY_LOW:
				return L"Very low";
			case thread::IoPriority::LOW:
				return L"Low";
			case thread::IoPriority::NORMAL:
				return L"Normal";
			case thread::IoPriority::HIGH:
				return L"High";
			case thread::IoPriority::CRITICAL:
				return L"Critical";
		}
		return L"unknown";
	}

}
