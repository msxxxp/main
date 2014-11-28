#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/traceback.hpp>
#include <basis/simstd/string>

#include <excis/exception.hpp>
#include <excis/service.hpp>
#include <excis/services.hpp>

struct ServicesView: public sync::Observer
{
	~ServicesView()
	{
		LogTraceObj();
	}

	ServicesView(Ext::Services* svcs):
		m_svcs(svcs)
	{
		LogTraceObj();
		m_svcs->register_observer(this);
	}

	void notify(const sync::Message& event) {
		UNUSED(event);
		LogReport(L"Services changed. type: %d, size: %Iu\n", (int)m_svcs->get_type(), m_svcs->size());

		for (Ext::Services::size_type i = 0; i < m_svcs->size(); ++i) {
			LogReport(L"svc[%Iu] name: '%s'\n", i, (*m_svcs)[i].name.c_str());
		}
	}

private:
	Ext::Services* m_svcs;
};

void test_service()
{
	LogAtten(L"\n");

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
