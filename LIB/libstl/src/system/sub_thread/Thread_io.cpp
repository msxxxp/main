#include <system/configure.hpp>
#include <system/thread.hpp>
#include <system/totext.hpp>

#include <liblog/logger.hpp>

namespace thread {

	namespace {
		logger::Module_i * get_logger_module()
		{
			auto static module = logger::get_module(L"threads");
			return module;
		}
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
		ULONG p = (ULONG)prio;
		NTSTATUS ret = NtSetInformationThread(m_handle, ThreadIoPriority, &p, sizeof(p));
		LogNoiseIf(!ret, L"id: %u, prio: '%s'\n", m_id, to_str(prio));
		LogErrorIf( ret, L"id: %u, prio: '%s' -> %s\n", m_id, to_str(prio), totext::nt_status(ret).c_str());
		return ret;
	}

	Thread::IoPriority_t Thread::get_io_priority() const
	{
		ULONG prio = 0;
		NTSTATUS ret = NtQueryInformationThread(m_handle, ThreadIoPriority, &prio, sizeof(prio), nullptr);
		LogNoiseIf(!ret, L"id: %u -> '%s'\n", m_id, to_str((Thread::IoPriority_t)prio));
		LogErrorIf( ret, L"id: %u -> '%s'\n", m_id,  totext::nt_status(ret).c_str());
		return (Thread::IoPriority_t)prio;
	}

}
