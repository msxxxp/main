#include <tests.hpp>

#include <basis/sys/crypt.hpp>
#include <basis/sys/logger.hpp>

void test_crypt()
{
	LogInfo(L"\n");

	auto prov = crypt::provider();

	auto hash = crypt::hash(prov);
	for (int i = 0; i < 200 && hash->process(reinterpret_cast<const void*>(&i), sizeof(i)); ++i) {
	}

	char hashBuf[64];
	hash->get_hash(hashBuf, sizeof(hashBuf));

	LogReport(L"hash: %S\n", hashBuf);
}
