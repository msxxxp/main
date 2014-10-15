#include <basis/sys/crt.hpp>
#include <basis/sys/logger.hpp>

#include <sqlite_wrap/sqlite.hpp>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");
	}
}

//extern "C" void _pei386_runtime_relocator()
//{
//}

extern "C" int wmain(int argc, wchar_t * argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

//	crt::set_unhandled_exception_filter();

	setup_logger();

	auto db = sqlite::open_database(L"../qwe.db");
//	auto db = sqlite::open_database(L"src");
	if (db) {
		db->exec("CREATE TABLE IF NOT EXISTS test_table(Id INTEGER PRIMARY KEY, key TEXT NOT NULL);");
//		db->exec("CREATE TABLE IF NOT EXISTS test_table(key TEXT NOT NULL, name TEXT NOT NULL, value BLOB, PRIMARY KEY (key, name));");
		db->exec("INSERT INTO test_table(key) VALUES('adasdasdasdadasasda');");
		db->exec("INSERT INTO test_table(key) VALUES('ssfdfsdfsdfsdfsdfsd');");

		{
			auto stmt = db->get_statement(L"SELECT count(*) FROM test_table");
			if (stmt && stmt->step())
				LogReport(L"count: %I64d\n", stmt->get_int64(0));
		}
		{
			auto stmt = db->get_statement(L"SELECT * FROM test_table");
			while (stmt && stmt->step()) {
				LogReport(L"id: %I64d\n", stmt->get_int64(0));
				LogReport(L"key: %s\n", stmt->get_text16(1));
			}
		}
	}

	return 0;
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	return wmain(0, nullptr);
}
