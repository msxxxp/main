#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/traceback.hpp>
#include <basis/simstd/string>

#include <excis/exception.hpp>
#include <excis/service.hpp>

struct ServicesView: public sync::Observer
{
	~ServicesView()
	{
		LogTraceObj();
	}

	ServicesView(service::Enum* enumSvc):
		m_svcs(enumSvc)
	{
		LogTraceObj();
		m_svcs->register_observer(this);
	}

	void notify(const sync::Message& event) {
		UNUSED(event);
		LogReport(L"Services changed. type: %d, size: %Iu\n", (int)m_svcs->get_type(), m_svcs->size());

		for (service::Enum::size_type i = 0; i < m_svcs->size(); ++i) {
			LogReport(L"svc[%Iu] name: '%s'\n", i, (*m_svcs)[i].name.c_str());
		}
	}

private:
	service::Enum* m_svcs;
};

void test_service()
{
	LogAtten(L"\n");

	service::Enum svcs;//(L"localhost");

	LogTrace();
	ServicesView obs(&svcs);

//	svcs.start_batch();

	LogTrace();
	svcs.set_type(service::EnumerateType::SERVICES);

	LogTrace();
	svcs.set_type(service::EnumerateType::DRIVERS);

	LogTrace();
	svcs.set_host(L"localhost");

//	svcs.stop_batch();

	LogTrace();
}
