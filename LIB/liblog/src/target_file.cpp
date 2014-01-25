#include <liblog/logger.hpp>

#include <system/memory.hpp>
#include <system/sync.hpp>
#include <simstd/memory>

namespace logger {

	struct LogToFile: public Target_i {
		~LogToFile();

		LogToFile(const wchar_t * path, bool overwrite);

		void out(const Module_i * lgr, Level lvl, const wchar_t * str, size_t size) const override;

		void out(const wchar_t * str, size_t size) const override;

		sync::ScopeGuard lock_scope() const override;

	private:
		memory::auto_destroy<sync::SyncUnit_i*> m_sync;
		memory::auto_close<HANDLE> m_file;
	};

	LogToFile::~LogToFile()
	{
	}

	LogToFile::LogToFile(const wchar_t * path, bool overwrite) :
		m_sync(sync::get_CritSection()),
		m_file(::CreateFileW(path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, overwrite ? CREATE_ALWAYS : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr))
	{
		if (m_file.is_valid()) {
			::SetFilePointer(m_file, 0, nullptr, FILE_END);
		}
	}

	void LogToFile::out(const Module_i * /*lgr*/, Level /*lvl*/, const wchar_t * str, size_t size) const
	{
		out(str, size);
	}

	void LogToFile::out(const wchar_t * str, size_t size) const
	{
		if (m_file.is_valid()) {
			DWORD written = 0;
			auto lockScope(m_sync->lock_scope());
			::WriteFile(m_file, str, size * sizeof(wchar_t), &written, nullptr);
		}
	}

	sync::ScopeGuard LogToFile::lock_scope() const
	{
		return m_sync->lock_scope();
	}

	Target_t get_TargetToFile(const wchar_t * path, bool overwrite)
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
