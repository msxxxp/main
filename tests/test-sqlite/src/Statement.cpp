#include "sqlite_pvt.hpp"

namespace sqlite {

	Statement::~Statement()
	{
		finalize();
	}

	Statement::Statement(sqlite::sqlite3_stmt* stmt) :
		m_stmt(stmt)
	{
	}

	Statement::Statement(Statement && other) :
		m_stmt(nullptr)
	{
		swap(other);
	}

	Statement & Statement::operator = (Statement && other)
	{
		Statement(simstd::move(other)).swap(*this);
		return *this;
	}

	void Statement::swap(Statement & other)
	{
		using simstd::swap;
		swap(m_stmt, other.m_stmt);
	}

	bool Statement::is_valid() const
	{
		return m_stmt;
	}

	bool Statement::finalize()
	{
		return is_valid() ? sqlite::sqlite3_finalize(m_stmt) == SQLITE_OK : true;
	}

	void Statement::reset()
	{
		sqlite::sqlite3_clear_bindings(m_stmt);
		sqlite::sqlite3_reset(m_stmt);
	}

	bool Statement::step()
	{
		return sqlite::sqlite3_step(m_stmt) == SQLITE_ROW;
	}

	void Statement::bind(int param, int32_t value)
	{
		sqlite::sqlite3_bind_int(m_stmt, param, value);
	}

	void Statement::bind(int param, int64_t value)
	{
		sqlite::sqlite3_bind_int64(m_stmt, param, value);
	}

	void Statement::bind(int param, const wchar_t* str, bool copy)
	{
		using sqlite::sqlite3_destructor_type;
		sqlite::sqlite3_bind_text16(m_stmt, param, str, -1, copy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}

	void Statement::bind(int param, const void *blob, size_t size, bool copy)
	{
		using sqlite::sqlite3_destructor_type;
		sqlite::sqlite3_bind_blob(m_stmt, param, blob, static_cast<int>(size), copy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}

	size_t Statement::get_count() const
	{
		return sqlite3_data_count(m_stmt);
	}

	const wchar_t* Statement::get_text16(int column)
	{
		return (const wchar_t*)sqlite::sqlite3_column_text16(m_stmt, column);
	}

	const char* Statement::get_text8(int column)
	{
		return (const char*)sqlite::sqlite3_column_text(m_stmt, column);
	}

	int32_t Statement::get_int32(int column)
	{
		return sqlite::sqlite3_column_int(m_stmt, column);
	}

	int64_t Statement::get_int64(int column)
	{
		return sqlite::sqlite3_column_int64(m_stmt, column);
	}

	const char* Statement::get_blob(int column)
	{
		return (const char *)sqlite::sqlite3_column_blob(m_stmt, column);
	}

	ColumnType Statement::get_type(int column)
	{
		switch (sqlite::sqlite3_column_type(m_stmt, column)) {
			case SQLITE_INTEGER:
				return TYPE_INTEGER;
			case SQLITE_TEXT:
				return TYPE_STRING;
			case SQLITE_BLOB:
				return TYPE_BLOB;
			default:
				return TYPE_UNKNOWN;
		}
	}

}
