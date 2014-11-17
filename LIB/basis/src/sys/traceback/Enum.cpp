#include <basis/os/dbghelp.hpp>
#include <basis/sys/traceback.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/string>

#if defined(__GNUC__)
#include <bfd.h>
#endif

namespace traceback {

	LogRegisterLocal(L"traceback");

	struct DebugSymbols {
		static DebugSymbols & inst(const wchar_t * path = nullptr)
		{
			static DebugSymbols instance(path);
			return instance;
		}

	private:
		~DebugSymbols()
		{
			bool ret = os::Dbghelp_dll::inst().SymCleanup(::GetCurrentProcess());
			LogTraceIf(ret);
			LogErrorIf(!ret, L"%s\n", totext::api_error().c_str());
			UNUSED(ret);
		}

		DebugSymbols(const wchar_t * path)
		{
			os::Dbghelp_dll::inst().SymSetOptions(os::Dbghelp_dll::inst().SymGetOptions() | /*SYMOPT_DEFERRED_LOADS | */SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_LOAD_LINES);
			bool ret = os::Dbghelp_dll::inst().SymInitializeW(::GetCurrentProcess(), path, TRUE);
			LogNoiseIf(ret, L"['%s']\n", path);
			LogErrorIf(!ret, L"['%s'] -> %s\n", path, totext::api_error().c_str());
			UNUSED(ret);

#if defined(__GNUC__)
			bfd_init();
#endif
		}
	};

	void init(const wchar_t * path)
	{
		DebugSymbols::inst(path);
	}

	///=============================================================================================
	Enum::Enum(size_t depth)
	{
		LogNoise(L"depth: %Iu\n", depth);

		{
			PVOID* tempFramesArray = memory::calloc<PVOID*>(get_max_depth());
			WORD sz = ::RtlCaptureStackBackTrace(1, simstd::min(depth, get_max_depth()), tempFramesArray, nullptr);
			for (WORD i = 0; i < sz; ++i) {
				emplace_back(tempFramesArray[i]);
//				LogDebug(L"frame[%d]: %p\n", (int)i, tempFramesArray[i]);
			}
			memory::free(tempFramesArray);
		}
		LogDebug(L"captured frames: %Iu\n", size());
	}

	Enum::Enum(PCONTEXT context, void* address, size_t depth)
	{
		LogNoise(L"context: %p address: %p depth: %Iu\n", context, address, depth);

		/* � ��� � Release-������������ ���������� ��������� ���� /Oy- (�� �������� ��������� �� ������), ����� ����� ������.
		 * http://www.bytetalk.net/2011/06/why-rtlcapturecontext-crashes-on.html
		 */
//		CONTEXT ctx;
//		memory::zero(ctx);
//		ctx.ContextFlags = CONTEXT_FULL;
//		::RtlCaptureContext(&ctx);

		STACKFRAME64 sf;
		memset(&sf, 0, sizeof(sf));

		DWORD machine = 0;
#ifdef _AMD64_
		sf.AddrPC.Offset = context->Rip;
		sf.AddrPC.Mode = AddrModeFlat;
		sf.AddrFrame.Offset = context->Rbp;
		sf.AddrFrame.Mode = AddrModeFlat;
		sf.AddrStack.Offset = context->Rsp;
		sf.AddrStack.Mode = AddrModeFlat;
		machine = IMAGE_FILE_MACHINE_AMD64;
		if (address)
			emplace_back(address);
#else
		sf.AddrPC.Offset = context->Eip;
		sf.AddrPC.Mode = AddrModeFlat;
		sf.AddrFrame.Offset = context->Ebp;
		sf.AddrFrame.Mode = AddrModeFlat;
		sf.AddrStack.Offset = context->Esp;
		sf.AddrStack.Mode = AddrModeFlat;
		machine = IMAGE_FILE_MACHINE_I386;
		if (address)
			emplace_back(address);
#endif

		while (depth-- > 0) {
			PFUNCTION_TABLE_ACCESS_ROUTINE64 tar = (PFUNCTION_TABLE_ACCESS_ROUTINE64)&os::Dbghelp_dll::inst().SymFunctionTableAccess64;
			PGET_MODULE_BASE_ROUTINE64 mbr = (PGET_MODULE_BASE_ROUTINE64)&os::Dbghelp_dll::inst().SymGetModuleBase64;
			BOOL res = os::Dbghelp_dll::inst().StackWalk64(machine, GetCurrentProcess(), GetCurrentThread(), &sf, (void*)context, nullptr, tar, mbr, nullptr);
#ifdef _AMD64_
			if (!res || sf.AddrReturn.Offset == 0)
				break;
			emplace_back(reinterpret_cast<void*>(sf.AddrReturn.Offset));
			LogDebug(L"frame: %p\n", (void*)sf.AddrReturn.Offset);
#else
			if (!res || sf.AddrPC.Offset == 0)
				break;
			emplace_back(reinterpret_cast<void*>(sf.AddrPC.Offset));
#endif
		}

		LogDebug(L"captured frames: %Iu\n", size());
	}

	size_t Enum::get_max_depth()
	{
		return 61;
	}

}
