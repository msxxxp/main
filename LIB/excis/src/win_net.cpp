#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>
#include <basis/ext/pattern.hpp>
#include <excis/timer.hpp>
#include <excis/exception.hpp>

#include <basis/std/string>

namespace Ext {

	///========================================================================================== WinNet
	namespace WinNet {
		ustring GetCompName(COMPUTER_NAME_FORMAT cnf)
		{
			DWORD size = 0;
			::GetComputerNameExW(cnf, nullptr, &size);
			memory::auto_array<wchar_t> buf(size);
			::GetComputerNameExW(cnf, buf.data(), &size);
			return ustring(buf.data());
		}
	}

	///==================================================================================== WinSysTimers
	WinSysTimers::WinSysTimers()
	{
		memory::zero(*this);
		typedef LONG (WINAPI *PROCNTQSI)(UINT, PVOID, ULONG, PULONG);

		PROCNTQSI NtQuerySystemInformation;

		NtQuerySystemInformation = (PROCNTQSI)::GetProcAddress(::GetModuleHandleW(L"ntdll"), "NtQuerySystemInformation");

		if (!NtQuerySystemInformation)
			return;

		NtQuerySystemInformation(3, this, sizeof(*this), 0);
	}

	size_t WinSysTimers::Uptime(size_t del)
	{
		ULONGLONG start = liKeBootTime.QuadPart;
		ULONGLONG now = liKeSystemTime.QuadPart;
		ULONGLONG Result = (now - start) / 10000000LL;
		return Result / del;
	}

	ustring WinSysTimers::UptimeAsText()
	{
		size_t uptime = Uptime();
		size_t ud = uptime / (60 * 60 * 24);
		uptime %= (60 * 60 * 24);
		size_t uh = uptime / (60 * 60);
		uptime %= (60 * 60);
		size_t um = uptime / 60;
		size_t us = uptime % 60;
		ustring Result;
		if (ud) {
			Result += simstd::to_wstring(ud);
			Result += L"d ";
		}

		Result += simstd::to_wstring(uh);
		Result += L":";
		Result += simstd::to_wstring(um);
		Result += L":";
		Result += simstd::to_wstring(us);
		return Result;
	}

}
