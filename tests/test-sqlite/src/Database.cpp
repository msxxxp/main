#include "sqlite_pvt.hpp"

#include <basis/sys/logger.hpp>

#define ENUM_TO_STR(var, err) case err: var = L""#err; break

namespace sqlite {
	LogRegister(L"sqlite");

	string get_version()
	{
//		return L"SQLite, version " + wide(SQLITE_VERSION);
		return L"";
	}

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

	namespace internal {
		bool database_close(sqlite3 * db)
		{
			int err = sqlite::sqlite3_close(db);
			bool ret = err == SQLITE_OK;
			LogErrorIf(!ret, L"error: %s\n", to_str(err));
			return ret;
		}

		sqlite3 * database_open(const string& path)
		{
			sqlite3 * db = nullptr;

			int err = sqlite::sqlite3_open16(path.c_str(), &db);

			if (err == SQLITE_OK) {
				err = sqlite::sqlite3_exec(db, "PRAGMA quick_check;", nullptr, nullptr, nullptr);
			}

			if (err != SQLITE_OK) {
				LogErrorIf(err != SQLITE_OK, L"error: %s (%S)\n", to_str(err), error_message(err));
				database_close(db);
				return nullptr;
			}

			return db;
		}
	}

	Database::~Database()
	{
		close();
		LogTraceObj();
	}

	bool Database::is_valid() const
	{
		return m_db;
	}

	Database::Database(const string& path) :
		m_db(internal::database_open(path))
	{
		LogTraceObj();
		LogDebug(L"[%s] -> %p\n", path.c_str(), m_db);
		if (is_valid()) {
			sqlite::sqlite3_extended_result_codes(m_db, 1);
			sqlite::sqlite3_busy_timeout(m_db, 500);
		}
	}

	bool Database::close()
	{
		bool ret = internal::database_close(m_db);
		m_db = nullptr;
		return ret;
	}

	bool Database::exec(const char * command)
	{
		int err = sqlite::sqlite3_exec(m_db, command, nullptr, nullptr, nullptr);

		bool ret = err == SQLITE_OK;
		LogErrorIf(!ret, L"error: %s (%S)\n", to_str(err), command);
		return ret;
	}

	bool Database::transaction_begin()
	{
		return exec("BEGIN TRANSACTION;");
	}

	bool Database::transaction_end()
	{
		return exec("END TRANSACTION;");
	}

	bool Database::transaction_rollback()
	{
		return exec("ROLLBACK TRANSACTION;");
	}

	bool Database::enable_foreign_keys_constraints()
	{
		return exec("PRAGMA foreign_keys = ON;");
	}

	int64_t Database::last_insert_id() const
	{
		return sqlite::sqlite3_last_insert_rowid(m_db);
	}

	Statement Database::get_statement(const wchar_t * cmd)
	{
		sqlite3_stmt * stmt = nullptr;
		int err = sqlite::sqlite3_prepare16_v2(m_db, cmd, -1, &stmt, nullptr);

		LogErrorIf(err != SQLITE_OK, L"error: %s", to_str(err));
		return simstd::move(Statement(stmt));
	}

	DatabaseHolder open_database(const string& path)
	{
		simstd::unique_ptr<Database> tmp(new Database(path));
		return tmp->is_valid() ? DatabaseHolder(simstd::move(tmp)) : DatabaseHolder();
	}

}
