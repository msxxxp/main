#ifndef _SQLITE_HPP_
#define _SQLITE_HPP_

#include <basis/ext/pattern.hpp>
#include <basis/simstd/memory>

namespace sqlite {

	struct sqlite3;
	struct sqlite3_stmt;

	class Database_i;
	class Statement_i;

	typedef simstd::unique_ptr<Database_i> Database;
	typedef simstd::unique_ptr<Statement_i> Statement;

	enum class ColumnType: ssize_t {
		Integer,
		Text,
		Blob,
		Unknown,
	};

//	string get_version();

	const wchar_t * to_str(int err);

	const char * error_message(int err);

	class Statement_i {
	public:
		virtual ~Statement_i() = default;

		virtual void reset() = 0;
		virtual bool step() = 0;
		virtual void bind(int param) = 0;
		virtual void bind(int param, int32_t value) = 0;
		virtual void bind(int param, int64_t value) = 0;
		virtual void bind(int param, double value) = 0;
		virtual void bind(int param, const char* str, bool copy = true) = 0;
		virtual void bind(int param, const wchar_t* str, bool copy = true) = 0;
		virtual void bind(int param, const void* blob, size_t size, bool copy = true) = 0;

		virtual const wchar_t* get_text16(int column) const = 0;
		virtual const char*    get_text8(int column) const = 0;
		virtual int32_t        get_int32(int column) const = 0;
		virtual int64_t        get_int64(int column) const = 0;
		virtual const void*    get_blob(int column) const = 0;
		virtual size_t         get_size(int column) const = 0;

		virtual size_t         get_count() const = 0;
		virtual const wchar_t* get_name(int column) const = 0;
		virtual ColumnType     get_type(int colunmn) const = 0;
	};

	class Database_i {
	public:
		virtual ~Database_i() = default;

		virtual bool exec(const char * sql) = 0;

		virtual bool transaction_begin() = 0;
		virtual bool transaction_end() = 0;
		virtual bool transaction_rollback() = 0;

		virtual bool enable_foreign_keys_constraints() = 0;

		virtual int64_t last_insert_rowid() const = 0;

		virtual Statement get_statement(const wchar_t * sql) = 0;
	};

	Database open_database(const wchar_t* path);

}

#endif

