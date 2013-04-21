#include <libbase/std.hpp>
#include <libbase/memory.hpp>
#include <libext/filesystem.hpp>
#include <libext/exception.hpp>

namespace Ext {

	namespace FS {
		Stat::Stat(PCWSTR path)
		{
			Base::auto_close<HANDLE> hndl(FS::HandleRead(path));
			refresh(hndl);
		}

		bool Stat::refresh(HANDLE hndl)
		{
			return CheckApi(::GetFileInformationByHandle(hndl, this));
		}

		Stat & Stat::operator =(HANDLE hndl)
		{
			refresh(hndl);
			return *this;
		}

		Stat & Stat::operator =(PCWSTR path)
		{
			Base::auto_close<HANDLE> hndl(FS::HandleRead(path));
			refresh(hndl);
			return *this;
		}
	}

}
