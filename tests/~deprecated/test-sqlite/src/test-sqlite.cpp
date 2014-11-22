#include <basis/sys/crt.hpp>
//#include <basis/sys/logger.hpp>

#include <sqlite_wrap/sqlite.hpp>
#include "sqlite3.h"

namespace {
	void setup_logger()
	{
//		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");
	}
}


struct SqliteIo: public sqlite3_io_methods
{
};

struct SqliteFile: public sqlite3_file
{
	int close();
	int read(void*, int iAmt, sqlite3_int64 iOfst);
	int write(const void*, int iAmt, sqlite3_int64 iOfst);
	int truncate(sqlite3_int64 size);
	int sync(int flags);
	int size(sqlite3_int64 *pSize);
	int lock(int);
	int unlock(int);
	int check_reserved_lock(int *pResOut);
	int file_control(int op, void *pArg);
	int sector_size();
	int device_characteristics();

	int shm_map(int iPg, int pgsz, int, void volatile**);
	int shm_lock(int offset, int n, int flags);
	void shm_barrier();
	int shm_unmap(int deleteFlag);

	int fetch(sqlite3_int64 iOfst, int iAmt, void **pp);
	int unfetch(sqlite3_int64 iOfst, void *p);
};

struct SqliteVfsWindows: public sqlite3_vfs
{
	SqliteVfsWindows();

	int open(const char *zName, sqlite3_file*, int flags, int *pOutFlags);
	int del(const char *zName, int syncDir);
	int access(const char *zName, int flags, int *pResOut);
	int full_path(const char *zName, int nOut, char *zOut);
	void *(*xDlOpen)(sqlite3_vfs*, const char *zFilename);
	void (*xDlError)(sqlite3_vfs*, int nByte, char *zErrMsg);
	void (*(*xDlSym)(sqlite3_vfs*,void*, const char *zSymbol))(void);
	void (*xDlClose)(sqlite3_vfs*, void*);
	int (*xRandomness)(sqlite3_vfs*, int nByte, char *zOut);
	int (*xSleep)(sqlite3_vfs*, int microseconds);
	int (*xCurrentTime)(sqlite3_vfs*, double*);
	int (*xGetLastError)(sqlite3_vfs*, int, char *);

	int (*xCurrentTimeInt64)(sqlite3_vfs*, sqlite3_int64*);

	int (*xSetSystemCall)(sqlite3_vfs*, const char *zName, sqlite3_syscall_ptr);
	sqlite3_syscall_ptr (*xGetSystemCall)(sqlite3_vfs*, const char *zName);
	const char *(*xNextSystemCall)(sqlite3_vfs*, const char *zName);
};

SqliteVfsWindows::SqliteVfsWindows()
{
	memset(static_cast<sqlite3_vfs*>(this), 0 , sizeof(sqlite3_vfs));
	iVersion = 3;
	szOsFile = sizeof(*this);
	mxPathname = 255;
	zName = "winVFS";
}

SqliteVfsWindows* vfsWindows = nullptr;

//int sqlite3_os_init(void)
//{
//	vfsWindows = new SqliteVfsWindows;
//	sqlite3_vfs_register(vfsWindows, 1);
//	return 0;
//}
//
//int sqlite3_os_end(void)
//{
//	sqlite3_vfs_unregister(vfsWindows);
//	delete vfsWindows;
//	vfsWindows = 0;
//	return 0;
//}

extern "C" int wmain(int argc, wchar_t * argv[])
{
	UNUSED(argc);
	UNUSED(argv);

//	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

//	crt::set_unhandled_exception_filter();

	setup_logger();

	{
		sqlite3 * db = nullptr;

		int err = sqlite3_open("asdasd", &db);

		if (err == 0) {
			err = sqlite3_exec(db, "PRAGMA quick_check;", nullptr, nullptr, nullptr);
		}
	}

//	auto db = sqlite::open_database(L"../qwe.db");
////	auto db = sqlite::open_database(L"src");
//	if (db) {
//		db->exec("CREATE TABLE IF NOT EXISTS test_table(Id INTEGER PRIMARY KEY, key TEXT NOT NULL);");
////		db->exec("CREATE TABLE IF NOT EXISTS test_table(key TEXT NOT NULL, name TEXT NOT NULL, value BLOB, PRIMARY KEY (key, name));");
//		db->exec("INSERT INTO test_table(key) VALUES('adasdasdasdadasasda');");
//		db->exec("INSERT INTO test_table(key) VALUES('ssfdfsdfsdfsdfsdfsd');");
//
//		{
//			auto stmt = db->get_statement(L"SELECT count(*) FROM test_table");
//			if (stmt && stmt->step()) {
//				;
////				LogReport(L"count: %I64d\n", stmt->get_int64(0));
//			}
//		}
//		{
//			auto stmt = db->get_statement(L"SELECT * FROM test_table");
//			while (stmt && stmt->step()) {
////				LogReport(L"id: %I64d\n", stmt->get_int64(0));
////				LogReport(L"key: %s\n", stmt->get_text16(1));
//			}
//		}
//	}

	return 0;
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	return wmain(0, nullptr);
}
