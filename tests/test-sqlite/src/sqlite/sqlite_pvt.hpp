#ifndef _SQLITE_PVT_HPP_
#define _SQLITE_PVT_HPP_

#include <basis/sys/logger.hpp>

#include <sqlite.hpp>

namespace sqlite
{
#	include <sqlite3.h>

	LogDeclare();

	class StatementImpl: public Statement_i {
	public:
		~StatementImpl();
		StatementImpl(sqlite3_stmt* stmt);

		bool is_valid() const;

		void reset() override;
		bool step() override;
		void bind(int param) override;
		void bind(int param, int32_t value) override;
		void bind(int param, int64_t value) override;
		void bind(int param, double value) override;
		void bind(int param, const char* str, bool copy) override;
		void bind(int param, const wchar_t* str, bool copy) override;
		void bind(int param, const void* blob, size_t size, bool copy) override;

		const wchar_t* get_text16(int column) const override;
		const char*    get_text8(int column) const override;
		int32_t        get_int32(int column) const override;
		int64_t        get_int64(int column) const override;
		const void*    get_blob(int column) const override;
		size_t         get_size(int column) const override;

		size_t         get_count() const override;
		const wchar_t* get_name(int column) const override;
		ColumnType     get_type(int colunmn) const override;

	private:
		bool finalize();

		sqlite3_stmt* m_stmt;
	};

	class DatabaseImpl: public Database_i {
	public:
		virtual ~DatabaseImpl();

		DatabaseImpl(const wchar_t* path);

		bool is_valid() const;

		bool exec(const char * sql) override;

		bool transaction_begin() override;
		bool transaction_end() override;
		bool transaction_rollback() override;

		bool enable_foreign_keys_constraints() override;

		int64_t last_insert_rowid() const override;

		Statement get_statement(const wchar_t * sql) override;

	protected:
		bool close();

	private:
		sqlite3 *m_db;
	};

}


#endif
