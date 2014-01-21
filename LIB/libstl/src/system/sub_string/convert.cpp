//#include <system/configure.hpp>
//#include <libbase/cstr.hpp>
//#include <libbase/string.hpp>
//
//namespace Base {
//	namespace String {
//
//		astring w2cp(PCWSTR in, UINT cp)
//		{
//			auto_array<CHAR> buf(Cstr::convert(in, cp));
//			Cstr::convert(buf.data(), buf.size(), in, cp);
//			return astring(&buf[0]);
//		}
//
//		ustring cp2w(PCSTR in, UINT cp)
//		{
//			auto_array<wchar_t> buf(Cstr::convert(in, cp));
//			Cstr::convert(buf.data(), buf.size(), in, cp);
//			return ustring(buf.data());
//		}
//
//	}
//}
