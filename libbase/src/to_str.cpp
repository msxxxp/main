#include <libbase/std.hpp>

namespace Base {
	PCWSTR to_str(WaitResult_t waitResult)
	{
		switch (waitResult) {
			case WaitResult_t::SUCCESS:
				return L"SUCCESS";
			case WaitResult_t::APC:
				return L"APC";
			case WaitResult_t::TIMEOUT:
				return L"TIMEOUT";
			case WaitResult_t::FAILED:
				return L"FAILED";
			case WaitResult_t::ABANDONED:
				return L"ABANDONED";
			default:
				break;
		}
		return L"unknown";
	}
}
