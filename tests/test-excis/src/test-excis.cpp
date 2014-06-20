#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/sys/sync.hpp>

#ifdef DEBUG
#	include <basis/sys/traceback.hpp>
#endif

#include <basis/std/string>


#include <excis/exception.hpp>

#include <excis/dacl.hpp>
#include <excis/sd.hpp>

#include <excis/service.hpp>
#include <excis/services.hpp>

#include <exception>
#include <typeinfo>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");

#ifdef DEBUG
		traceback::init();
#endif
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

	void notify(const sync::Message & event) {
		UNUSED(event);
		LogReport(L"Services changed. size: %Iu\n", m_svcs->size());
	}

private:
	Ext::Services * m_svcs;
};

void test_service() {
	LogTrace();

//	Ext::RemoteConnection rc(L"localhost");
//	Ext::RemoteConnection * prc = nullptr;
//
//	LogDebug(L"host1: '%s'\n", rc.get_host().c_str());
//	LogDebug(L"host2: '%s'\n", prc->get_host().c_str());
//
//	LogTrace();
//	Ext::Service::Manager scm(&rc);
//
//	LogDebug(L"svc: %d\n", scm.is_exist(L"FARBCopy"));

	LogTrace();
	Ext::Services svcs(L"localhost");

	LogTrace();
	ServicesView obs(&svcs);

//	svcs.start_batch();

	LogTrace();
	svcs.set_type(Ext::Service::EnumerateType_t::SERVICES);

	LogTrace();
	svcs.set_type(Ext::Service::EnumerateType_t::DRIVERS);

//	svcs.stop_batch();

	LogTrace();
}

extern "C" int wmain(int argc, wchar_t * argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	setup_logger();

	try {
		LogTrace();
		test_service();
	} catch (Ext::AbstractError & e) {
		LogError(L"exception cought: %s, %s\n", e.what().c_str(), e.where());
		return e.code();
	} catch (std::exception & e) {
		LogError(L"std::exception [%S]:\n", typeid(e).name());
		LogError(L"What: %S\n", e.what());
		return 1;
	}

	return 0;
}
