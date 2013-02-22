#include <libbase/std.hpp>

namespace Base {
	PCWSTR to_str(WaitResult_t waitResult)
	{
		switch (waitResult) {
			case WaitResult_t::SUCCESS:
				return L"SUCCESS";
				break;
			case WaitResult_t::APC:
				return L"APC";
				break;
			case WaitResult_t::TIMEOUT:
				return L"TIMEOUT";
				break;
			case WaitResult_t::FAILED:
				return L"FAILED";
				break;
			case WaitResult_t::ABANDONED:
				return L"ABANDONED";
				break;
			default:
				break;
		}
		return L"unknown";
	}
}
