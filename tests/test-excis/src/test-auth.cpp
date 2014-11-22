#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/string>

#include <excis/auth.hpp>

void add_auth()
{
	LogTrace();
}

void test_auth()
{
	LogAtten(L"\n");

	auth::Credentials creds;

	LogReport(L"count: %Iu\n", creds.size());
	for (size_t i = 0; i < creds.size(); ++i) {
		auth::Credential cred = creds.at(i);
		LogReport(L"cred[%Iu]: name: '%s'\n", i, cred.name().c_str());
		LogInfo(L"cred[%Iu]: alia: '%s'\n", i, cred.alias().c_str());
		LogInfo(L"cred[%Iu]: comm: '%s'\n", i, cred.comment().c_str());
		LogInfo(L"cred[%Iu]: user: '%s'\n", i, cred.user().c_str());
		LogInfo(L"cred[%Iu]: pass: '%s'\n", i, cred.pass().c_str());
		LogInfo(L"cred[%Iu]: size: 0x%IX\n", i, cred.pass_size());
		LogInfo(L"cred[%Iu]: flag: 0x%IX\n", i, cred.flags());
		LogInfo(L"cred[%Iu]: type: 0x%IX\n", i, cred.type());
		LogInfo(L"cred[%Iu]: pers: 0x%IX\n", i, cred.persist());
	}
}
