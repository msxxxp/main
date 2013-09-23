#include <libcom/guid.hpp>
#include <libext/exception.hpp>
#include <libbase/std.hpp>
#include <libbase/string.hpp>

namespace Com {

	WinGUID::WinGUID()
	{
		CheckCom(::CoCreateGuid(this));
	}

	void WinGUID::init(PCWSTR str)
	{
		CheckCom(::CLSIDFromString((PWSTR)str, this));
	}

	void WinGUID::init(const PROPVARIANT & prop)
	{
		if (prop.vt == VT_BSTR) {
			size_t len = ::SysStringByteLen(prop.bstrVal);
			if (len == sizeof(*this)) {
				Memory::copy(this, prop.bstrVal, len);
				return;
			}
		}
//		else if (prop.is_str()) {
//			init(prop.as_str());
//		}
		CheckCom(E_INVALIDARG);
	}

	ustring WinGUID::as_str(const GUID & guid)
	{
		wchar_t buf[64];
		CheckApi(::StringFromGUID2(guid, buf, Base::lengthof(buf)));
		return ustring(buf);
	}

}
