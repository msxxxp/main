#include <basis/sys/sync.hpp>

namespace totext {

	const wchar_t * c_str(sync::WaitResult_t waitResult)
	{
		switch (waitResult) {
			case sync::WaitResult_t::SUCCESS:
				return L"SUCCESS";
			case sync::WaitResult_t::APC:
				return L"APC";
			case sync::WaitResult_t::TIMEOUT:
				return L"TIMEOUT";
			case sync::WaitResult_t::FAILED:
				return L"FAILED";
			case sync::WaitResult_t::ABANDONED:
				return L"ABANDONED";
		}
		return L"unknown";
	}

}
