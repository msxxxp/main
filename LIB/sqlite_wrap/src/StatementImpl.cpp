#include "sqlite_pvt.hpp"

namespace sqlite {

	StatementImpl::~StatementImpl()
	{
		finalize();
	}

	StatementImpl::StatementImpl(sqlite3_stmt* stmt) :
		m_stmt(stmt)
	{
	}

	bool StatementImpl::is_valid() const
	{
		return m_stmt;
	}

	bool StatementImpl::finalize()
	{
		return is_valid() ? sqlite3_finalize(m_stmt) == SQLITE_OK : true;
	}

	void StatementImpl::reset()
	{
		sqlite3_clear_bindings(m_stmt);
		sqlite3_reset(m_stmt);
	}

	bool StatementImpl::step()
	{
		return sqlite3_step(m_stmt) == SQLITE_ROW;
	}

	void StatementImpl::bind(int param)
	{
		sqlite3_bind_null(m_stmt, param);
	}

	void StatementImpl::bind(int param, int32_t value)
	{
		sqlite3_bind_int(m_stmt, param, value);
	}

	void StatementImpl::bind(int param, int64_t value)
	{
		sqlite3_bind_int64(m_stmt, param, value);
	}

	void StatementImpl::bind(int param, double value)
	{
		sqlite3_bind_double(m_stmt, param, value);
	}

	void StatementImpl::bind(int param, const char* str, bool copy)
	{
		using sqlite::sqlite3_destructor_type;
		sqlite3_bind_text(m_stmt, param, str, -1, copy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}

	void StatementImpl::bind(int param, const wchar_t* str, bool copy)
	{
		using sqlite::sqlite3_destructor_type;
		sqlite3_bind_text16(m_stmt, param, str, -1, copy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}

	void StatementImpl::bind(int param, const void *blob, size_t size, bool copy)
	{
		using sqlite::sqlite3_destructor_type;
		sqlite3_bind_blob(m_stmt, param, blob, static_cast<int>(size), copy ? SQLITE_TRANSIENT : SQLITE_STATIC);
	}

	const wchar_t* StatementImpl::get_text16(int column) const
	{
		return static_cast<const wchar_t*>(sqlite3_column_text16(m_stmt, column));
	}

	const char* StatementImpl::get_text8(int column) const
	{
		return reinterpret_cast<const char*>(sqlite3_column_text(m_stmt, column));
	}

	int32_t StatementImpl::get_int32(int column) const
	{
		return sqlite3_column_int(m_stmt, column);
	}

	int64_t StatementImpl::get_int64(int column) const
	{
		return sqlite3_column_int64(m_stmt, column);
	}

	const void* StatementImpl::get_blob(int column) const
	{
		return sqlite3_column_blob(m_stmt, column);
	}

	size_t StatementImpl::get_size(int column) const
	{
		return sqlite3_column_bytes(m_stmt, column);
	}

	size_t StatementImpl::get_count() const
	{
//		return sqlite3_data_count(m_stmt);
		return sqlite3_column_count(m_stmt);
	}

	const wchar_t* StatementImpl::get_name(int column) const
	{
		return static_cast<const wchar_t*>(sqlite3_column_name16(m_stmt, column));
	}

	ColumnType StatementImpl::get_type(int column) const
	{
		switch (sqlite3_column_type(m_stmt, column)) {
			case SQLITE_INTEGER:
				return ColumnType::Integer;
			case SQLITE_TEXT:
				return ColumnType::Text;
			case SQLITE_BLOB:
				return ColumnType::Blob;
			default:
				return ColumnType::Unknown;
		}
	}

}
