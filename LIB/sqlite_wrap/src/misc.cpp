#include "sqlite_pvt.hpp"

#include <basis/sys/logger.hpp>

#define ENUM_TO_STR(var, err) case err: var = L""#err; break

namespace sqlite {
	LogRegister(L"sqlite");

//	string get_version()
//	{
////		return L"SQLite, version " + wide(SQLITE_VERSION);
//		return L"";
//	}

	const wchar_t * to_str(int err)
	{
		const wchar_t* err_str = L"";
		switch (err)
		{
			ENUM_TO_STR(err_str, SQLITE_OK);
			ENUM_TO_STR(err_str, SQLITE_ERROR);
			ENUM_TO_STR(err_str, SQLITE_INTERNAL);
			ENUM_TO_STR(err_str, SQLITE_PERM);
			ENUM_TO_STR(err_str, SQLITE_ABORT);
			ENUM_TO_STR(err_str, SQLITE_BUSY);
			ENUM_TO_STR(err_str, SQLITE_LOCKED);
			ENUM_TO_STR(err_str, SQLITE_NOMEM);
			ENUM_TO_STR(err_str, SQLITE_READONLY);
			ENUM_TO_STR(err_str, SQLITE_INTERRUPT);
			ENUM_TO_STR(err_str, SQLITE_IOERR);
			ENUM_TO_STR(err_str, SQLITE_CORRUPT);
			ENUM_TO_STR(err_str, SQLITE_NOTFOUND);
			ENUM_TO_STR(err_str, SQLITE_FULL);
			ENUM_TO_STR(err_str, SQLITE_CANTOPEN);
			ENUM_TO_STR(err_str, SQLITE_PROTOCOL);
			ENUM_TO_STR(err_str, SQLITE_EMPTY);
			ENUM_TO_STR(err_str, SQLITE_SCHEMA);
			ENUM_TO_STR(err_str, SQLITE_TOOBIG);
			ENUM_TO_STR(err_str, SQLITE_CONSTRAINT);
			ENUM_TO_STR(err_str, SQLITE_MISMATCH);
			ENUM_TO_STR(err_str, SQLITE_MISUSE);
			ENUM_TO_STR(err_str, SQLITE_NOLFS);
			ENUM_TO_STR(err_str, SQLITE_AUTH);
			ENUM_TO_STR(err_str, SQLITE_FORMAT);
			ENUM_TO_STR(err_str, SQLITE_RANGE);
			ENUM_TO_STR(err_str, SQLITE_NOTADB);
			ENUM_TO_STR(err_str, SQLITE_NOTICE);
			ENUM_TO_STR(err_str, SQLITE_WARNING);
			ENUM_TO_STR(err_str, SQLITE_ROW);
			ENUM_TO_STR(err_str, SQLITE_DONE);
		}
		return err_str;
	}

	const char * error_message(int err)
	{
		return sqlite::sqlite3_errstr(err);
	}

}
