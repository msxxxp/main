#include "../../logger_pvt.hpp"

#include <basis/sys/sync.hpp>
#include <basis/sys/cstr.hpp>

#include <basis/simstd/memory>

#include "event_msg.h"

namespace logger {

	namespace {
		WORD const LogLevelTypes[(int)Level::Force + 1] = {
			EVENTLOG_SUCCESS,
			EVENTLOG_SUCCESS,
			EVENTLOG_INFORMATION_TYPE,
			EVENTLOG_INFORMATION_TYPE,
			EVENTLOG_WARNING_TYPE,
			EVENTLOG_WARNING_TYPE,
			EVENTLOG_ERROR_TYPE,
			EVENTLOG_ERROR_TYPE,
			EVENTLOG_INFORMATION_TYPE,
		};

		struct LogToSys: public Target_i {
			~LogToSys();

			LogToSys(const wchar_t * name, const wchar_t * path);

			void out(const Module_i * lgr, Level lvl, const wchar_t * str, size_t size) const override;

			void out(const Module_i * lgr, WORD color, Level lvl, const wchar_t * str, size_t size) const override;

			void out(const wchar_t * str, size_t size) const override;

			lock_type lock_scope() const override;

		private:
			static void app_register(const wchar_t * name, const wchar_t * path);

			mutable sync_type m_sync;
			HANDLE m_hndl;
		};

		LogToSys::~LogToSys()
		{
			::DeregisterEventSource(m_hndl);
		}

		LogToSys::LogToSys(const wchar_t * name, const wchar_t * path)
		{
			app_register(name, path);
			m_hndl = ::RegisterEventSourceW(nullptr, name);
		}

		void LogToSys::out(const Module_i * /*lgr*/, Level lvl, const wchar_t * str, size_t /*size*/) const
		{
			//			PSID user = nullptr;
			//			HANDLE token;
			//			PTOKEN_USER token_user = nullptr;
			//			if (OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, FALSE, &token)) {
			//				token_user = (PTOKEN_USER)DefaultTokenInformation(token, TokenUser);
			//				if (token_user)
			//					user = token_user->User.Sid;
			//				CloseHandle(token);
			//			}
			::ReportEventW(m_hndl, LogLevelTypes[(int)lvl], 0, EV_MSG_STRING, nullptr, 1, 0, &str, nullptr);
			//			free(token_user);
		}

		void LogToSys::out(const Module_i * /*lgr*/, WORD /*color*/, Level lvl, const wchar_t * str, size_t /*size*/) const
		{
			::ReportEventW(m_hndl, LogLevelTypes[(int)lvl], 0, EV_MSG_STRING, nullptr, 1, 0, &str, nullptr);
		}

		void LogToSys::out(const wchar_t * str, size_t /*size*/) const
		{
			::ReportEventW(m_hndl, LogLevelTypes[(int)defaults::get_level()], 0, EV_MSG_STRING, nullptr, 1, 0, &str, nullptr);
		}

		lock_type LogToSys::lock_scope() const
		{
			return simstd::auto_lock(m_sync);
		}

		void LogToSys::app_register(const wchar_t * name, const wchar_t * path)
		{
			wchar_t path_buf[MAX_PATH_LEN], *fullpath = path_buf;
			if (cstr::is_empty(path)) {
				::GetModuleFileNameW(0, path_buf, lengthof(path_buf));
			} else {
				fullpath = (wchar_t *)path;
			}

			auto key1 = L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\";
			auto key_length = cstr::length(key1);
			wchar_t value[MAX_PATH_LEN];
			cstr::copy(value, key1, lengthof(value));
			cstr::cat(value, name, lengthof(value) - key_length);
			HKEY hKey = nullptr;
			::RegCreateKeyW(HKEY_LOCAL_MACHINE, value, &hKey);
			// Add the Event ID message-file name to the subkey.
			::RegSetValueExW(hKey, L"EventMessageFile", 0, REG_EXPAND_SZ, (LPBYTE)fullpath, (DWORD)((cstr::length(fullpath) + 1) * sizeof(wchar_t)));
			// Set the supported types flags.
			DWORD dwData = EVENTLOG_SUCCESS | EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
			::RegSetValueExW(hKey, L"TypesSupported", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData));
			::RegCloseKey(hKey);
		}

	}

	Target_t get_TargetToSys(const wchar_t * name, const wchar_t * path)
	{
		return Target_t(new LogToSys(name, path));
	}

}
