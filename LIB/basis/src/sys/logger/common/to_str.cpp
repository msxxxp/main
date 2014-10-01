#include "../logger_pvt.hpp"

#define ENUM_TO_STR(enumVar) #enumVar

namespace logger {

	const wchar_t * const LogLevelNames[(int)Level::Force + 1] = {
		L"TR",
		L"DB",
		L"IN",
		L"RP",
		L"AT",
		L"WA",
		L"ER",
		L"FA",
		L"AL",
		L"EM",
		L"  ",
	};

	const wchar_t * to_str(Level lvl)
	{
		return LogLevelNames[(int)lvl];
	}

	const wchar_t * to_str(Prefix::flags fl)
	{
		const wchar_t * ret = L"manual";

		switch (fl) {
			case Prefix::Lite:
				ret = L"lite";
				break;
			case Prefix::Medium:
				ret = L"medium";
				break;
			case Prefix::Full:
				ret = L"full";
				break;
			default:
				break;
		}

		return ret;
	}


}
