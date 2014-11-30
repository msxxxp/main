#include "logger_pvt.hpp"

#include <basis/sys/memory.hpp>
#include <basis/sys/sync.hpp>

#include <basis/simstd/memory>

namespace logger {

	namespace {
		struct LogToFile: public Target_i {
			~LogToFile() = default;

			LogToFile(const wchar_t* path, bool overwrite);

			void out(const Module * lgr, Level lvl, const wchar_t* str, size_t size) const override;

			void out(const Module * lgr, WORD color, Level lvl, const wchar_t* str, size_t size) const override;

			void out(const wchar_t* str, size_t size) const override;

			lock_type lock_scope() const override;

		private:
			mutable sync_type m_sync;
			memory::auto_close<HANDLE> m_file;
		};

//		LogToFile::~LogToFile()
//		{
//		}

		LogToFile::LogToFile(const wchar_t* path, bool overwrite) :
			m_file(::CreateFileW(path, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, overwrite ? CREATE_ALWAYS : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr))
		{
			if (m_file.is_valid()) {
				::SetFilePointer(m_file, 0, nullptr, FILE_END);
			}
		}

		void LogToFile::out(const Module * /*lgr*/, Level /*lvl*/, const wchar_t* str, size_t size) const
		{
			out(str, size);
		}

		void LogToFile::out(const Module * /*lgr*/, WORD /*color*/, Level /*lvl*/, const wchar_t* str, size_t size) const
		{
			out(str, size);
		}

		void LogToFile::out(const wchar_t* str, size_t size) const
		{
			if (m_file.is_valid()) {
				DWORD written = 0;
				auto lockScope(lock_scope());
				::WriteFile(m_file, str, size * sizeof(wchar_t), &written, nullptr);
			}
		}

		lock_type LogToFile::lock_scope() const
		{
			return simstd::auto_lock(m_sync);
		}

	}

	Target_t get_TargetToFile(const wchar_t* path, bool overwrite)
	{
		return Target_t(new LogToFile(path, overwrite));
	}

}

///========================================================================================= Logging
//void logFile(WIN32_FIND_DATA info) {
//	uint64_t size = HighLow64(info.nFileSizeHigh, info.nFileSizeLow);
//	logDebug(L"%s   found: \"%s\" (Size=%I64i,%s%s%s%s%s%s%s%s%s%s%s)\n", FILE_ATTRIBUTE_DIRECTORY
//	    & info.dwFileAttributes ? L"Dir " : L"File", info.cFileName, size, FILE_ATTRIBUTE_ARCHIVE
//	    & info.dwFileAttributes ? L"ARCHIVE " : L"", FILE_ATTRIBUTE_COMPRESSED
//	    & info.dwFileAttributes ? L"COMPRESSED " : L"", FILE_ATTRIBUTE_ENCRYPTED
//	    & info.dwFileAttributes ? L"ENCRYPTED " : L"", FILE_ATTRIBUTE_HIDDEN
//	    & info.dwFileAttributes ? L"HIDDEN " : L"",
//	         FILE_ATTRIBUTE_NORMAL & info.dwFileAttributes ? L"NORMAL " : L"",
//	         FILE_ATTRIBUTE_OFFLINE & info.dwFileAttributes ? L"OFFLINE " : L"",
//	         FILE_ATTRIBUTE_READONLY & info.dwFileAttributes ? L"READONLY " : L"",
//	         FILE_ATTRIBUTE_REPARSE_POINT & info.dwFileAttributes ? L"REPARSE_POINT " : L"",
//	         FILE_ATTRIBUTE_SPARSE_FILE & info.dwFileAttributes ? L"SPARSE " : L"",
//	         FILE_ATTRIBUTE_SYSTEM & info.dwFileAttributes ? L"SYSTEM " : L"",
//	         FILE_ATTRIBUTE_TEMPORARY & info.dwFileAttributes ? L"TEMP " : L"");
//}
