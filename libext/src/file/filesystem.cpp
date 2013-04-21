#include <libbase/std.hpp>
#include <libbase/memory.hpp>
#include <libbase/path.hpp>
#include <libbase/pcstr.hpp>
#include <libext/dll.hpp>
#include <libext/file.hpp>
#include <libext/exception.hpp>

namespace Ext {

	///================================================================================= File system
	namespace {
//		struct LargeInteger {
//			LargeInteger() {
//				m_data.QuadPart = 0LL;
//			}
//
//			LargeInteger(uint64_t in) {
//				m_data.QuadPart = in;
//			}
//
//			operator LARGE_INTEGER() const {
//				return m_data;
//			}
//
//			PLARGE_INTEGER operator&() const {
//				return (PLARGE_INTEGER)&m_data;
//			}
//
//			operator uint64_t() const {
//				return (uint64_t)m_data.QuadPart;
//			}
//		private:
//			LARGE_INTEGER m_data;
//		};

//		struct TemporarySetAttributes: private Base::Uncopyable {
//			~TemporarySetAttributes()
//			{
//				if (m_attr != INVALID_FILE_ATTRIBUTES)
//					::SetFileAttributesW(m_path.c_str(), m_attr);
//			}
//			TemporarySetAttributes(const ustring & path, DWORD attr):
//				m_path(path),
//				m_attr(FS::get_attr(m_path))
//			{
//				if (m_attr != INVALID_FILE_ATTRIBUTES && (m_attr & attr) != attr)
//					::SetFileAttributesW(m_path.c_str(), attr);
//			}
//			void cancel_restore()
//			{
//				m_attr = INVALID_FILE_ATTRIBUTES;
//			}
//		private:
//			ustring m_path;
//			DWORD m_attr;
//		};

		///=================================================================================== psapi_dll
		struct psapi_dll: private DynamicLibrary {
			typedef DWORD (WINAPI *FGetMappedFileNameW)(HANDLE, LPVOID, LPWSTR, DWORD);

			DEFINE_FUNC(GetMappedFileNameW);

			static psapi_dll & inst() {
				static psapi_dll ret;
				return ret;
			}

		private:
			psapi_dll():
				DynamicLibrary(L"psapi.dll") {
				GET_DLL_FUNC(GetMappedFileNameW);
			}
		};
	}

	namespace FS {
		bool is_exist(PCWSTR path) {
			DWORD attr = ::GetFileAttributesW(path);
			if (attr != INVALID_FILE_ATTRIBUTES)
				return true;
			DWORD err = ::GetLastError();
			if (err != ERROR_FILE_NOT_FOUND)
				CheckApiError(err);
			return false;
		}

		DWORD get_attr_nt(PCWSTR path) {
			return ::GetFileAttributesW(path);
		}

		DWORD get_attr(PCWSTR path) {
			DWORD ret = get_attr_nt(path);
			CheckApi(ret != INVALID_FILE_ATTRIBUTES);
			return ret;
		}

		void set_attr(PCWSTR path, DWORD attr) {
			CheckApi(::SetFileAttributesW(path, attr));
		}

		bool is_file(PCWSTR path) {
			return 0 == (get_attr(path) & FILE_ATTRIBUTE_DIRECTORY);
		}

		bool is_dir(PCWSTR path) {
			return 0 != (get_attr(path) & FILE_ATTRIBUTE_DIRECTORY);
		}

		void del_sh(PCWSTR path) {
			SHFILEOPSTRUCTW sh;

			sh.hwnd = nullptr;
			sh.wFunc = FO_DELETE;
			sh.pFrom = path;
			sh.pTo = nullptr;
			sh.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
			sh.hNameMappings = 0;
			sh.lpszProgressTitle = nullptr;
			CheckApiError(::SHFileOperationW(&sh));
		}

		void del_recycle(PCWSTR path) {
			SHFILEOPSTRUCTW	info = {0};
			info.wFunc	= FO_DELETE;
			info.pFrom	= path;
			info.fFlags	= FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
			CheckApiError(::SHFileOperationW(&info));
		}

		void del_on_reboot(PCWSTR path) {
			CheckApi(::MoveFileExW(path, nullptr, MOVEFILE_DELAY_UNTIL_REBOOT));
		}

		bool del_nt(PCWSTR path)
		{
			DWORD attr = get_attr_nt(path);
			if (attr != INVALID_FILE_ATTRIBUTES) {
				if (attr & FILE_ATTRIBUTE_DIRECTORY) {
					return Directory::del_nt(path);
				} else {
					return File::del_nt(path);
				}
			}
			return false;
		}

		void del(PCWSTR path) {
			CheckApi(del_nt(path));
		}

		ustring device_path_to_disk(PCWSTR path) {
			wchar_t local_disks[MAX_PATH] = {0}, *p = local_disks;
			CheckApi(::GetLogicalDriveStringsW(sizeofa(local_disks) - 1, local_disks));
			wchar_t drive[3] = L" :";
			wchar_t device[MAX_PATH];
			while (*p) {
				*drive = *p;
				CheckApi(::QueryDosDeviceW(drive, device, sizeofa(device)));
				if (Base::Str::find(path, device) == path) {
					wchar_t new_path[Base::MAX_PATH_LEN];
					_snwprintf(new_path, sizeofa(new_path), L"%s%s", drive, path + Base::Str::length(device));
					return ustring(new_path);
				}
				while (*p++);
			};
			return ustring(path);
		}

		ustring get_path(HANDLE hndl) {
			CheckHandle(hndl);
			Base::auto_close<HANDLE> hmap(CheckHandleErr(::CreateFileMappingW(hndl, nullptr, PAGE_READONLY, 0, 1, nullptr)));
			Base::auto_close<PVOID const> view(CheckPointerErr(::MapViewOfFile(hmap, FILE_MAP_READ, 0, 0, 1)), ::UnmapViewOfFile);
			wchar_t path[Base::MAX_PATH_LEN];
			CheckApi(psapi_dll::inst().GetMappedFileNameW(::GetCurrentProcess(), view, path, sizeofa(path)));
			return device_path_to_disk(path);
		}
	}
}
