#ifndef BASIS_OS_NTDLL_
#define BASIS_OS_NTDLL_

#include <basis/sys/linkage.hpp>

namespace os {

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

	struct ntdll_dll: private linkage::DynamicLibrary {
		typedef LONG (WINAPI *FNtSetInformationThread)(HANDLE, THREADINFOCLASS, PVOID, ULONG);
		typedef LONG (WINAPI *FNtQueryInformationThread)(HANDLE, THREADINFOCLASS, PVOID, ULONG, PULONG);

		DEFINE_FUNC(NtSetInformationThread);
		DEFINE_FUNC(NtQueryInformationThread);

		static ntdll_dll& inst();

	private:
		ntdll_dll();
	};

}

#endif
