#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/sys/sync.hpp>
#include <basis/sys/traceback.hpp>

#include <basis/simstd/string>

#include <excis/exception.hpp>
#include <excis/dacl.hpp>
#include <excis/sd.hpp>

#include <excis/auth.hpp>
#include <excis/service.hpp>
#include <excis/services.hpp>

#include <exception>
#include <typeinfo>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=d;prefix=fu;target=co");

		traceback::init();
	}
}

struct ServicesView: public sync::Observer
{
	ServicesView(Ext::Services * svcs):
		m_svcs(svcs)
	{
		LogTrace();
		m_svcs->register_observer(this);
	}

	void notify(const sync::Message& event) {
		UNUSED(event);
		LogReport(L"Services changed. size: %Iu\n", m_svcs->size());

		for (Ext::Services::size_type i = 0; i < m_svcs->size(); ++i) {
			LogReport(L"svc[%Iu] name: '%s'\n", i, (*m_svcs)[i].name.c_str());
		}
	}

private:
	Ext::Services* m_svcs;
};

void test_service()
{
	LogTrace();
	Ext::Services svcs(L"localhost");

	LogTrace();
	ServicesView obs(&svcs);

//	svcs.start_batch();

	LogTrace();
	svcs.set_type(Ext::Service::EnumerateType_t::SERVICES);

	LogTrace();
	svcs.set_type(Ext::Service::EnumerateType_t::DRIVERS);

	LogTrace();
	svcs.set_host(L"127.0.0.1");

//	svcs.stop_batch();

	LogTrace();
}

void test_auth()
{
	LogTrace();

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

void add_auth()
{
	LogTrace();
}

extern "C" int wmain(int argc, wchar_t * argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	exception::set_vectored_exception_filter();
	exception::set_unhandled_exception_filter();

	try {
		LogTrace();
//		test_service();
		test_auth();
//		add_auth();
	} catch (exception::AbstractError& e) {
		LogError(L"exception cought: %s, %s\n", e.what().c_str(), e.where().c_str());

		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogError(L"\t%s\n", mstr[i]);

		return e.code();
	} catch (std::exception& e) {
		LogError(L"std::exception [%S]:\n", typeid(e).name());
		LogError(L"What: %S\n", e.what());
		return 1;
	} catch (...) {
		LogError(L"cpp exception cought\n");
	}

	return 0;
}
