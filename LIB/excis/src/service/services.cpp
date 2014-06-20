#include <basis/sys/logger.hpp>

#include <excis/connection.hpp>
#include <excis/services.hpp>
#include <excis/exception.hpp>

#include <basis/std/algorithm>
#include <basis/std/string>

namespace Ext {

	struct Services::Filter {
		~Filter();
		Filter(const ustring & host = ustring(), PCWSTR user = nullptr, PCWSTR pass = nullptr, Service::EnumerateType_t = Service::EnumerateType_t::SERVICES);

		const connection::Remote & get_connection() const;
		const Service::Manager & get_read_manager() const;
		const Service::Manager & get_write_manager() const;

		ustring get_host() const;
		void set_host(const ustring & host = ustring(), PCWSTR user = nullptr, PCWSTR pass = nullptr);

	private:
		size_t                                    mutable m_writable:1;
		simstd::shared_ptr<connection::Remote>            m_conn;
		simstd::shared_ptr<Ext::Service::Manager> mutable m_scm;
	};

	Services::Filter::~Filter()
	{
		LogTraceObjBegin();
		LogTraceObjEnd();
	}

	Services::Filter::Filter(const ustring & host, PCWSTR user, PCWSTR pass, Service::EnumerateType_t type) :
		m_writable(0)
	{
		LogTraceObjBegin();
		LogNoise(L"host: '%s', user: '%s', type: %Id\n", host.c_str(), user, (ssize_t)type);
		set_host(host, user, pass);
		LogTraceObjEnd();
	}

	const connection::Remote & Services::Filter::get_connection() const
	{
		return *m_conn.get();
	}

	const Service::Manager & Services::Filter::get_read_manager() const
	{
		return *m_scm.get();
	}

	const Service::Manager & Services::Filter::get_write_manager() const
	{
		if (!m_writable) {
			Service::Manager * tmp_manager = new Service::Manager(m_conn.get(), SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CREATE_SERVICE);
			m_scm.reset(tmp_manager);
			m_writable = 1;
		}
		return *m_scm.get();
	}

	ustring Services::Filter::get_host() const
	{
		return m_conn->get_host();
	}

	void Services::Filter::set_host(const ustring & host, PCWSTR user, PCWSTR pass)
	{
		LogNoise(L"host: '%s', user: '%s'\n", host.c_str(), user);
		simstd::shared_ptr<connection::Remote> tmp_conn(connection::Remote::create(host, user, pass));
		simstd::shared_ptr<Ext::Service::Manager> tmp_scm(new Ext::Service::Manager(tmp_conn.get(), SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE));
		m_writable = 0;

		using simstd::swap;
		swap(m_scm, tmp_scm);
		swap(m_conn, tmp_conn);
	}

	///==================================================================================== Services
	Services::~Services()
	{
		LogTraceObjBegin();
		LogTraceObjEnd();
	}

	Services::Services(const ustring & host, PCWSTR user, PCWSTR pass) :
		m_filter(new Services::Filter(host, user, pass)),
		m_type(Service::EnumerateType_t::SERVICES),
		m_wait_timout(10 * 1000),
		m_wait_state(false),
		m_batch_started(false)
	{
		LogTraceObjBegin();
		LogNoise(L"hast: '%s', user: '%s'\n", host.c_str(), user);
		LogTraceObjEnd();
	}

	Service::EnumerateType_t Services::get_type() const
	{
		return m_type;
	}

	void Services::set_type(Service::EnumerateType_t type)
	{
		LogNoise(L"type: 0x%X\n", (uint32_t)type);
		m_type = type;
		update();
		notify_changed();
	}

	ustring Services::get_host() const
	{
		return m_filter->get_host();
	}

	void Services::set_host(const ustring & host, PCWSTR user, PCWSTR pass)
	{
		m_filter->set_host(host, user, pass);
		update();
		notify_changed();
	}

	void Services::update()
	{
		// filter is changed
		LogNoise(L"type: 0x%X\n", (uint32_t)get_type());
		DWORD dwBufNeed = 0, dwNumberOfService = 0;
		::EnumServicesStatusExW(m_filter->get_read_manager(), SC_ENUM_PROCESS_INFO, (DWORD)get_type(), SERVICE_STATE_ALL, nullptr, 0, &dwBufNeed, &dwNumberOfService, nullptr, nullptr);
		CheckApi(::GetLastError() == ERROR_MORE_DATA);

		memory::auto_buf<LPENUM_SERVICE_STATUS_PROCESSW> enum_svc(dwBufNeed);
		CheckApi(::EnumServicesStatusExW(m_filter->get_read_manager(), SC_ENUM_PROCESS_INFO, (DWORD)get_type(), SERVICE_STATE_ALL, (PBYTE)enum_svc.data(), enum_svc.size(), &dwBufNeed, &dwNumberOfService, nullptr, nullptr));
		clear();
		for (ULONG i = 0; i < dwNumberOfService; ++i) {
			emplace_back(m_filter->get_read_manager(), enum_svc.data()[i]);
		}
		LogNoise(L"cached: %Iu\n", size());
//		notify_changed();
	}

	Services::iterator Services::find(const ustring & name)
	{
		return simstd::find(begin(), end(), name);
	}

	Services::const_iterator Services::find(const ustring & name) const
	{
		return simstd::find(begin(), end(), name);
	}

	void Services::add(const Service::Create_t & info)
	{
//		try {
		emplace_back(info.get_name(), m_filter->get_write_manager().create_service(info));
		notify_changed();
//		} catch (AbstractError &e) {
//			Rethrow(e, L"Unable to create service");
//		}
	}

	void Services::del(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s\n", it->name.c_str());
			//			try {
			Service::del(m_filter->get_read_manager(), it->name.c_str());
			erase(it);
			notify_changed();
			//			} catch (AbstractError & e) {
			//				Rethrow(e, msg);
			//			}
		}
	}

	void Services::start(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s size: %Iu\n", it->name.c_str(), size());
			Service svc(Service::start(m_filter->get_read_manager(), it->name.c_str()));
			if (m_wait_state) {
				svc.wait_state(Service::State_t::STARTED, m_wait_timout);
			}
			*it = Service::Info_t(it->name.c_str(), svc);
			notify_changed();
		}
	}

	void Services::stop(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s size: %Iu\n", it->name.c_str(), size());
			Service svc(Service::stop(m_filter->get_read_manager(), it->name.c_str()));
			if (m_wait_state) {
				svc.wait_state(Service::State_t::STOPPED, m_wait_timout);
			}
			*it = Service::Info_t(it->name.c_str(), svc);
			notify_changed();
		}
	}

	void Services::restart(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s\n", it->name.c_str());
			Service svc(Service::restart(m_filter->get_read_manager(), it->name.c_str()));
			if (m_wait_state) {
				svc.wait_state(Service::State_t::STARTED, m_wait_timout);
			}
			*it = Service::Info_t(it->name.c_str(), svc);
			notify_changed();
		}
	}

	void Services::contin(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s\n", it->name.c_str());
			Service svc(Service::contin(m_filter->get_read_manager(), it->name.c_str()));
			if (m_wait_state) {
				svc.wait_state(Service::State_t::STARTED, m_wait_timout);
			}
			*it = Service::Info_t(it->name.c_str(), svc);
			notify_changed();
		}
	}

	void Services::pause(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s\n", it->name.c_str());
			Service svc(Service::pause(m_filter->get_read_manager(), it->name.c_str()));
			if (m_wait_state) {
				svc.wait_state(Service::State_t::PAUSED, m_wait_timout);
			}
			*it = Service::Info_t(it->name.c_str(), svc);
			notify_changed();
		}
	}

	void Services::set_config(iterator it, const Service::Config_t & info)
	{
		if (it != end()) {
			LogNoise(L"%s\n", it->name.c_str());
			info.log();
			*it = Service::Info_t(it->name.c_str(), Service::set_config(m_filter->get_read_manager(), it->name.c_str(), info));
			notify_changed();
		}
	}

	void Services::set_logon(iterator it, const Service::Logon_t& info)
	{
		if (it != end()) {
			LogNoise(L"%s\n", it->name.c_str());
			*it = Service::Info_t(it->name.c_str(), Service::set_logon(m_filter->get_read_manager(), it->name.c_str(), info));
			notify_changed();
		}
	}

	void Services::start_batch()
	{
		LogTrace();
		m_batch_started = true;
	}

	void Services::notify_changed()
	{
		set_changed(true);
		if (!m_batch_started) {
			LogTrace();
			notify_all(sync::Message());
		}
	}

	void Services::stop_batch()
	{
		LogTrace();
		m_batch_started = false;
		notify_all(sync::Message());
	}

	void Services::set_wait_state(bool new_state)
	{
		m_wait_state = new_state;
	}

	bool Services::get_wait_state() const
	{
		return m_wait_state;
	}

	void Services::set_wait_timeout(size_t timeout_msec)
	{
		m_wait_timout = timeout_msec;
	}

	size_t Services::get_wait_timeout() const
	{
		return m_wait_timout;
	}

}
