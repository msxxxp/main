#ifndef _SQLITE_HPP_
#define _SQLITE_HPP_

#include <basis/ext/pattern.hpp>
#include <basis/simstd/memory>
#include <basis/simstd/string>

typedef simstd::wstring string;

namespace sqlite {

	struct sqlite3;
	struct sqlite3_stmt;

	class Statement;

	enum ColumnType {
		TYPE_INTEGER,
		TYPE_STRING,
		TYPE_BLOB,
		TYPE_UNKNOWN
	};

	string get_version();

	const wchar_t * to_str(int err);

	class Database: private pattern::Uncopyable {
	public:
		virtual ~Database();

		Database(const string& path);
		bool is_valid() const;

		bool exec(const char * command);
		bool transaction_begin();
		bool transaction_end();
		bool transaction_rollback();
		bool enable_foreign_keys_constraints();
		int64_t last_insert_id() const;
		Statement get_statement(const wchar_t * cmd);

	protected:
		bool close();

		string strPath;
		string m_Name;

	private:
		sqlite::sqlite3 *m_db;
	};

	typedef simstd::unique_ptr<Database> DatabaseHolder;

	DatabaseHolder open_database(const string& name);

	class Statement: private pattern::Uncopyable {
	public:
		~Statement();

		Statement(Statement && other);
		Statement& operator = (Statement && other);

		void swap(Statement & other);

		bool is_valid() const;

		bool finalize();
		void reset();
		bool step();
		void bind(int param, int32_t value);
		void bind(int param, int64_t value);
		void bind(int param, const wchar_t* str, bool copy = true);
		void bind(int param, const void* blob, size_t size, bool copy = true);

		size_t         get_count() const;
		const wchar_t* get_text16(int column);
		const char*    get_text8(int column);
		int32_t        get_int32(int column);
		int64_t        get_int64(int column);
		const char*    get_blob(int column);
		ColumnType     get_type(int colunmn);

	private:
		Statement(sqlite::sqlite3_stmt* stmt);

		sqlite::sqlite3_stmt* m_stmt;

		friend class Database;
	};
}

#endif

