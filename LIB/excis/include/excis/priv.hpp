#ifndef _WIN_DEF_PRIV_H_
#define _WIN_DEF_PRIV_H_

#include <basis/configure.hpp>
#include <basis/sys/memory.hpp>
#include <basis/simstd/iosfwd>
#include <basis/ext/pattern.hpp>

namespace Ext {

	///==================================================================================== WinToken
	struct WinToken: private pattern::Uncopyable {
		WinToken(ACCESS_MASK mask = TOKEN_QUERY);

		WinToken(HANDLE process, ACCESS_MASK mask);

		operator HANDLE() const
		{
			return m_token;
		}

		static bool check_membership(PSID sid, HANDLE token = nullptr);

	private:
		memory::auto_close<HANDLE> m_token;
	};

	///===================================================================================== WinPriv
	namespace WinPriv {
		bool is_exist(HANDLE token, const LUID & priv);
		bool is_exist(HANDLE token, const wchar_t* priv_name);
		bool is_exist(const LUID & priv);
		bool is_exist(const wchar_t* priv_name);

		bool is_enabled(HANDLE token, const LUID & priv);
		bool is_enabled(HANDLE token, const wchar_t* priv_name);
		bool is_enabled(const LUID & priv);
		bool is_enabled(const wchar_t* priv_name);

		void modify(HANDLE token, const LUID & priv, bool enable);
		void modify(HANDLE token, const wchar_t* priv_name, bool enable);
		void modify(const LUID & priv, bool enable);
		void modify(const wchar_t* priv_name, bool enable);

		inline void disable(const LUID & in)
		{
			modify(in, false);
		}
		inline void disable(const wchar_t* in)
		{
			modify(in, false);
		}

		inline void enable(const LUID & in)
		{
			modify(in, true);
		}
		inline void enable(const wchar_t* in)
		{
			modify(in, true);
		}

		ustring GetName(const wchar_t* priv_name);
	}

	///=================================================================================== Privilege
	struct Privilege: private pattern::Uncopyable {
		~Privilege();

		explicit Privilege(const wchar_t* priv_name);

	private:
		TOKEN_PRIVILEGES m_tp;
		bool m_disable;
	};

}

#endif
