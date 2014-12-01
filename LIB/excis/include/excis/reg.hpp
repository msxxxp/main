/**
	win_reg

	@classes ()
	@author © 2012 Andrew Grechkin
	@link ()
 **/

#ifndef WIN_REG_HPP
#define WIN_REG_HPP

#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>

#include <basis/simstd/iosfwd>

namespace Ext {

//	struct WinReg: private pattern::Uncopyable {
//		~WinReg() {
//			CloseKey();
//		}
//
//		WinReg():
//			hKeyOpend(0),
//			hKeyReq(HKEY_CURRENT_USER) {
//		}
//
//		WinReg(HKEY hkey, const ustring& path):
//			hKeyOpend(0),
//			hKeyReq(hkey),
//			m_path(path) {
//		}
//
//		WinReg(const ustring& path);
//
//		ustring path() const {
//			return m_path;
//		}
//
//		void path(const ustring& path) {
//			m_path = path;
//		}
//
//		void key(HKEY hkey) {
//			hKeyReq = hkey;
//		}
//
//		bool Add(const ustring& name) const;
//
//		bool Del(const ustring& name) const;
//
//		void Set(const ustring& name, const wchar_t* value) const;
//
//		void Set(const ustring& name, const ustring& value) const {
//			Set(name, value.c_str());
//		}
//
//		void Set(const ustring& name, int value) const;
//
//		bool Get(const ustring& name, ustring &value, const ustring& def) const;
//
//		bool Get(const ustring& name, int &value, int def) const;
//
//	private:
//		HKEY mutable hKeyOpend;
//		HKEY hKeyReq;
//		ustring m_path;
//
//		void CloseKey() const;
//
//		bool OpenKey(ACCESS_MASK acc) const {
//			return OpenKey(hKeyReq, m_path, acc);
//		}
//
//		bool OpenKey(HKEY hkey, const ustring& path, ACCESS_MASK acc) const;
//
//		template <typename Type>
//		void SetRaw(const ustring& name, const Type &value, DWORD type = REG_BINARY) const {
//			if (OpenKey(KEY_WRITE)) {
//				::RegSetValueExW(hKeyOpend, name.c_str(), 0, type, (PBYTE)(&value), sizeof(value));
//				CloseKey();
//			}
//		}
//
//		template <typename Type>
//		bool GetRaw(const ustring& name, Type &value, const Type &def) const {
//			bool Result = OpenKey(KEY_READ);
//			value = def;
//			if (Result) {
//				DWORD	size = sizeof(value);
//				Result = ::RegQueryValueExW(hKeyOpend, name.c_str(), nullptr, nullptr, (PBYTE)(&value), &size) == ERROR_SUCCESS;
//				CloseKey();
//			}
//			return Result;
//		}
//	};


	///==================================================================================== Register
	struct Register {
		~Register();

		Register(const wchar_t* path, HKEY hkey = HKEY_LOCAL_MACHINE, ACCESS_MASK acc = KEY_READ);

		Register(Register && right);

		Register & operator = (Register & right);

		static bool is_exist_key(const wchar_t* path, HKEY hkey = HKEY_LOCAL_MACHINE);

//		bool del(FARSETTINGS_SUBFOLDERS root = FSSF_ROOT);
//
//		bool del(const wchar_t* name, FARSETTINGS_SUBFOLDERS root = FSSF_ROOT);
//
		size_t get(const wchar_t* name, PVOID value, size_t size) const;

		ustring get(const wchar_t* name, const wchar_t* def) const;

		uint64_t get(const wchar_t* name, uint64_t def) const;

		int64_t get(const wchar_t* name, int64_t def) const;

		uint32_t get(const wchar_t* name, uint32_t def) const;

		int32_t get(const wchar_t* name, int32_t def) const;

		bool get(const wchar_t* name, bool def) const;

		void set(const wchar_t* name, const void* value, size_t size);

		void set(const wchar_t* name, const wchar_t* value);

		void set(const wchar_t* name, uint64_t value);

		operator HKEY () const;

		Register open_subkey(const wchar_t* path, ACCESS_MASK acc = 0) const;

		Register open_subkey_stored_in_key(const wchar_t* path, ACCESS_MASK acc = 0) const;

	private:
		HKEY m_hndl;
		ACCESS_MASK m_access;

		Register(const Register &);
		Register & operator = (const Register &);
	};

}

#endif
