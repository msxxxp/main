#include <libbase/thread.hpp>

#include <libbase/err.hpp>
#include <libbase/string.hpp>
#include <libbase/logger.hpp>

namespace Base {
	static Logger::Module_i * get_logger_module()
	{
		auto static module = Logger::get_module(L"threads");
		return module;
	}

	enum THREADINFOCLASS {
		ThreadBasicInformation,
		ThreadTimes,
		ThreadPriority,
		ThreadBasePriority,
		ThreadAffinityMask,
		ThreadImpersonationToken,
		ThreadDescriptorTableEntry,
		ThreadEnableAlignmentFaultFixup,
		ThreadEventPair_Reusable,
		ThreadQuerySetWin32StartAddress,
		ThreadZeroTlsCell,
		ThreadPerformanceCount,
		ThreadAmILastThread,
		ThreadIdealProcessor,
		ThreadPriorityBoost,
		ThreadSetTlsArrayAddress,
		ThreadIsIoPending,
		ThreadHideFromDebugger,
		ThreadBreakOnTermination,
		ThreadSwitchLegacyState,
		ThreadIsTerminated,
		ThreadLastSystemCall,
		ThreadIoPriority,
		ThreadCycleTime,
		ThreadPagePriority,
		ThreadActualBasePriority,
		ThreadTebInformation,
		ThreadCSwitchMon,
		ThreadCSwitchPmu,
		ThreadWow64Context,
		ThreadGroupInformation,
		ThreadUmsInformation,
		ThreadCounterProfiling,
		ThreadIdealProcessorEx,
		MaxThreadInfoClass
	};

	typedef LONG NTSTATUS;

	extern "C" {
		NTSYSCALLAPI
		NTSTATUS
		NTAPI
		NtSetInformationThread(IN HANDLE ThreadHandle, IN THREADINFOCLASS ThreadInformationClass, IN PVOID ThreadInformation, IN ULONG ThreadInformationLength);

		NTSTATUS
		WINAPI
		NtQueryInformationThread(IN HANDLE ThreadHandle, IN THREADINFOCLASS ThreadInformationClass, PVOID ThreadInformation, IN ULONG ThreadInformationLength, PULONG ReturnLength);
	}

	bool Thread::set_io_priority(Thread::IoPriority_t prio)
	{
		LogNoise(L"id: %u, prio: %s\n", m_id, to_str(prio));
		ULONG p = (ULONG)prio;
		NTSTATUS ret = NtSetInformationThread(m_handle, ThreadIoPriority, &p, sizeof(p));
		LogErrorIf(ret, L"-> %s\n", NTStatusAsStr(ret).c_str());
		return ret;
	}

	Thread::IoPriority_t Thread::get_io_priority() const
	{
		LogNoise(L"id: %u\n", m_id);
		ULONG p = 0;
		NTSTATUS ret = NtQueryInformationThread(m_handle, ThreadIoPriority, &p, sizeof(p), nullptr);
		LogErrorIf(ret, L"-> %s\n", NTStatusAsStr(ret).c_str());
		return (Thread::IoPriority_t)p;
	}

}
