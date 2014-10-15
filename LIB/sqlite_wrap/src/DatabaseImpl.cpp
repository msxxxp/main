#include "sqlite_pvt.hpp"

#include <basis/sys/logger.hpp>

namespace sqlite {
	namespace internal {
		bool database_close(sqlite3 * db)
		{
			int err = sqlite3_close(db);
			bool ret = err == SQLITE_OK;
			LogErrorIf(!ret, L"error: %s\n", to_str(err));
			return ret;
		}

		sqlite3 * database_open(const wchar_t* path)
		{
			sqlite3 * db = nullptr;

			int err = sqlite3_open16(path, &db);

			if (err == SQLITE_OK) {
				err = sqlite3_exec(db, "PRAGMA quick_check;", nullptr, nullptr, nullptr);
			}

			if (err != SQLITE_OK) {
				LogErrorIf(err != SQLITE_OK, L"error: %s (%S)\n", to_str(err), error_message(err));
				database_close(db);
				return nullptr;
			}

			return db;
		}
	}

	DatabaseImpl::~DatabaseImpl()
	{
		close();
		LogTraceObj();
	}

	bool DatabaseImpl::is_valid() const
	{
		return m_db;
	}

	DatabaseImpl::DatabaseImpl(const wchar_t* path) :
		m_db(internal::database_open(path))
	{
		LogTraceObj();
		LogDebug(L"[%s] -> %p\n", path, m_db);
		if (is_valid()) {
			sqlite3_extended_result_codes(m_db, 1);
			sqlite3_busy_timeout(m_db, 500);
		}
	}

	bool DatabaseImpl::close()
	{
		bool ret = internal::database_close(m_db);
		m_db = nullptr;
		return ret;
	}

	bool DatabaseImpl::exec(const char * command)
	{
		int err = sqlite3_exec(m_db, command, nullptr, nullptr, nullptr);

		bool ret = err == SQLITE_OK;
		LogErrorIf(!ret, L"error: %s (%S)\n", to_str(err), command);
		return ret;
	}

	bool DatabaseImpl::transaction_begin()
	{
		return exec("BEGIN TRANSACTION;");
	}

	bool DatabaseImpl::transaction_end()
	{
		return exec("END TRANSACTION;");
	}

	bool DatabaseImpl::transaction_rollback()
	{
		return exec("ROLLBACK TRANSACTION;");
	}

	bool DatabaseImpl::enable_foreign_keys_constraints()
	{
		return exec("PRAGMA foreign_keys = ON;");
	}

	int64_t DatabaseImpl::last_insert_rowid() const
	{
		return sqlite3_last_insert_rowid(m_db);
	}

	Statement DatabaseImpl::get_statement(const wchar_t * cmd)
	{
		sqlite3_stmt * stmt = nullptr;
		int err = sqlite3_prepare16_v2(m_db, cmd, -1, &stmt, nullptr);

		if (err == SQLITE_OK) {
			return Statement(new StatementImpl(stmt));
		}

		LogError(L"error: %p, %s", stmt, to_str(err));
		return Statement();
	}

	Database open_database(const wchar_t* path)
	{
		simstd::unique_ptr<DatabaseImpl> tmp(new DatabaseImpl(path));
		return tmp->is_valid() ? Database(simstd::move(tmp)) : Database();
	}

}
