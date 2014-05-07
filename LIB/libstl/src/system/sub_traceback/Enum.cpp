#include <system/traceback.hpp>
#include <system/totext.hpp>

#include <liblog/logger.hpp>

#include <simstd/algorithm>
#include <simstd/string>

#include <dbghelp.h>

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
			bool ret = ::SymCleanup(::GetCurrentProcess());
			LogTraceIf(ret);
			LogErrorIf(!ret, L"%s\n", totext::api_error().c_str());
			UNUSED(ret);
		}

		DebugSymbols(const wchar_t * path)
		{
			::SymSetOptions(::SymGetOptions() | /*SYMOPT_DEFERRED_LOADS | */SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_LOAD_LINES);
			bool ret = ::SymInitializeW(::GetCurrentProcess(), path, TRUE);
			LogTraceIf(ret);
			LogErrorIf(!ret, L"%s\n", totext::api_error().c_str());
			UNUSED(ret);

#if defined(__GNUC__)
			bfd_init();
#endif
		}
	};

	///=============================================================================================
	Enum::Enum(const wchar_t * path, size_t depth)
	{
		DebugSymbols::inst(path);
		LogNoise(L"path: '%s' depth: %Iu\n", path, depth);

		{
			void * tempFramesArray[get_max_depth()];
			WORD sz = ::RtlCaptureStackBackTrace(1, simstd::min(depth, get_max_depth()), tempFramesArray, nullptr);
			for (WORD i = 0; i < sz; ++i) {
				emplace_back(tempFramesArray[i]);
//				LogDebug(L"frame[%d]: %p\n", (int)i, tempFramesArray[i]);
			}

		}

		{
//			CONTEXT ctx;
//			memory::zero(ctx);
//			ctx.ContextFlags = CONTEXT_FULL;
//			::RtlCaptureContext(&ctx);
//
//			/* � ��� � Release-������������ ���������� ��������� ���� /Oy- (�� �������� ��������� �� ������), ����� ����� ������.
//			 * http://www.bytetalk.net/2011/06/why-rtlcapturecontext-crashes-on.html
//			 */
//			STACKFRAME64 sf;
//			memset(&sf, 0, sizeof(sf));
//
//			DWORD machine = 0;
//#ifdef _AMD64_
//			sf.AddrPC.Offset = ctx.Rip;
//			sf.AddrPC.Mode = AddrModeFlat;
//			sf.AddrFrame.Offset = ctx.Rbp;
//			sf.AddrFrame.Mode = AddrModeFlat;
//			sf.AddrStack.Offset = ctx.Rsp;
//			sf.AddrStack.Mode = AddrModeFlat;
//			machine = IMAGE_FILE_MACHINE_AMD64;
//#else
//			sf.AddrPC.Offset = ctx.Eip;
//			sf.AddrPC.Mode = AddrModeFlat;
//			sf.AddrFrame.Offset = ctx.Ebp;
//			sf.AddrFrame.Mode = AddrModeFlat;
//			sf.AddrStack.Offset = ctx.Esp;
//			sf.AddrStack.Mode = AddrModeFlat;
//			machine = IMAGE_FILE_MACHINE_I386;
//#endif
//
//			while (depth-- > 0) {
//				BOOL res = StackWalk64(machine, GetCurrentProcess(), GetCurrentThread(), &sf, (void*)&ctx, nullptr, &SymFunctionTableAccess64, &SymGetModuleBase64, nullptr);
//#ifdef _AMD64_
//				if (!res || sf.AddrReturn.Offset == 0)
//					break;
//				emplace_back(sf.AddrReturn.Offset);
//				LogDebug(L"frame: %p\n", (void*)sf.AddrReturn.Offset);
//#else
//				if (!res || sf.AddrPC.Offset == 0)
//					break;
//				emplace_back((size_t)sf.AddrPC.Offset);
//#endif
//			}
		}

		LogDebug(L"captured frames: %Iu\n", size());
	}

	size_t Enum::get_max_depth()
	{
		return 61;
	}

}
