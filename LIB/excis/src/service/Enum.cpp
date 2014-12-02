#include "service_pvt.hpp"

#include <excis/exception.hpp>

#include <basis/sys/logger.hpp>

#include <basis/simstd/algorithm>

namespace service {

	class Filter {
	public:
		~Filter();
		Filter(const ustring& host = ustring(), PCWSTR user = nullptr, PCWSTR pass = nullptr, EnumerateType = EnumerateType::SERVICES);

		const connection::Remote & get_connection() const;
		const Manager & get_read_manager() const;
		const Manager & get_write_manager() const;

		ustring get_host() const;
		void set_host(const ustring& host = ustring(), PCWSTR user = nullptr, PCWSTR pass = nullptr);

	private:
		size_t mutable m_writable:1;
		simstd::shared_ptr<connection::Remote> m_conn;
		simstd::shared_ptr<Manager> mutable m_scm;
	};

	Filter::~Filter()
	{
		LogTraceObjBegin();
		LogTraceObjEnd();
	}

	Filter::Filter(const ustring& host, PCWSTR user, PCWSTR pass, EnumerateType type) :
		m_writable(0)
	{
		LogTraceObjBegin();
		LogNoise(L"host: '%s', user: '%s', type: %Id\n", host.c_str(), user, (ssize_t)type);
		set_host(host, user, pass);
		LogTraceObjEnd();
	}

	const connection::Remote & Filter::get_connection() const
	{
		return *m_conn.get();
	}

	const Manager& Filter::get_read_manager() const
	{
		return *m_scm.get();
	}

	const Manager& Filter::get_write_manager() const
	{
		if (!m_writable) {
			Manager* tmp_manager = new Manager(m_conn.get(), SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CREATE_SERVICE);
			m_scm.reset(tmp_manager);
			m_writable = 1;
		}
		return *m_scm.get();
	}

	ustring Filter::get_host() const
	{
		return m_conn->get_host();
	}

	void Filter::set_host(const ustring& host, PCWSTR user, PCWSTR pass)
	{
		LogNoise(L"host: '%s', user: '%s'\n", host.c_str(), user);
		simstd::shared_ptr<connection::Remote> tmp_conn(connection::Remote::create(host, user, pass));
		simstd::shared_ptr<Manager> tmp_scm(new Manager(tmp_conn.get(), SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE));
		m_writable = 0;

		using simstd::swap;
		swap(m_scm, tmp_scm);
		swap(m_conn, tmp_conn);
	}

	///==================================================================================== Services
	Enum::~Enum()
	{
		LogTraceObjBegin();
		LogTraceObjEnd();
	}

	Enum::Enum(const ustring& host, PCWSTR user, PCWSTR pass) :
		m_filter(new Filter(host, user, pass)),
		m_type(EnumerateType::SERVICES),
		m_wait_timout(10 * 1000),
		m_wait_state(false),
		m_batch_started(false)
	{
		LogTraceObjBegin();
		LogNoise(L"host: '%s', user: '%s'\n", host.c_str(), user);
		LogTraceObjEnd();
	}

	EnumerateType Enum::get_type() const
	{
		return m_type;
	}

	void Enum::set_type(EnumerateType type)
	{
		LogNoise(L"type: 0x%X\n", (uint32_t)type);
		m_type = type;
		update();
		notify_changed();
	}

	ustring Enum::get_host() const
	{
		return m_filter->get_host();
	}

	void Enum::set_host(const ustring& host, PCWSTR user, PCWSTR pass)
	{
		m_filter->set_host(host, user, pass);
		update();
		notify_changed();
	}

	void Enum::update()
	{
		// filter is changed
		LogNoise(L"type: 0x%X\n", (uint32_t)get_type());
		DWORD bufNeed = 0, dwNumberOfService = 0;
		::EnumServicesStatusExW(m_filter->get_read_manager(), SC_ENUM_PROCESS_INFO, (DWORD)get_type(), SERVICE_STATE_ALL, nullptr, 0, &bufNeed, &dwNumberOfService, nullptr, nullptr);
		CheckApi(::GetLastError() == ERROR_MORE_DATA);

		memory::auto_buf<LPENUM_SERVICE_STATUS_PROCESSW> enum_svc(bufNeed);
		CheckApi(::EnumServicesStatusExW(m_filter->get_read_manager(), SC_ENUM_PROCESS_INFO, (DWORD)get_type(), SERVICE_STATE_ALL, (PBYTE)enum_svc.data(), enum_svc.size(), &bufNeed, &dwNumberOfService, nullptr, nullptr));

		clear();
		for (ULONG i = 0; i < dwNumberOfService; ++i) {
			emplace_back(m_filter->get_read_manager(), enum_svc.data()[i]);
		}
		LogNoise(L"cached: %Iu\n", size());
//		notify_changed();
	}

	Enum::iterator Enum::find(const ustring& name)
	{
		return simstd::find(begin(), end(), name);
	}

	Enum::const_iterator Enum::find(const ustring& name) const
	{
		return simstd::find(begin(), end(), name);
	}

	void Enum::add(const CreateRequest& request)
	{
		emplace_back(request.get_name(), request.execute(m_filter->get_write_manager()));
		notify_changed();
	}

	void Enum::del(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s\n", it->name.c_str());
//			try {
			Item::del(m_filter->get_read_manager(), it->name.c_str());
			erase(it);
			notify_changed();
//			} catch (Abstract & e) {
//				Rethrow(e, msg);
//			}
		}
	}

	void Enum::start(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s size: %Iu\n", it->name.c_str(), size());
			Item svc(Item::start(m_filter->get_read_manager(), it->name.c_str()));
			if (m_wait_state) {
				svc.wait_state(State::STARTED, m_wait_timout);
			}
			*it = Info(it->name.c_str(), svc);
			notify_changed();
		}
	}

	void Enum::stop(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s size: %Iu\n", it->name.c_str(), size());
			Item svc(Item::stop(m_filter->get_read_manager(), it->name.c_str()));
			if (m_wait_state) {
				svc.wait_state(State::STOPPED, m_wait_timout);
			}
			*it = Info(it->name.c_str(), svc);
			notify_changed();
		}
	}

	void Enum::restart(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s\n", it->name.c_str());
			Item svc(Item::restart(m_filter->get_read_manager(), it->name.c_str()));
			if (m_wait_state) {
				svc.wait_state(State::STARTED, m_wait_timout);
			}
			*it = Info(it->name.c_str(), svc);
			notify_changed();
		}
	}

	void Enum::contin(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s\n", it->name.c_str());
			Item svc(Item::contin(m_filter->get_read_manager(), it->name.c_str()));
			if (m_wait_state) {
				svc.wait_state(State::STARTED, m_wait_timout);
			}
			*it = Info(it->name.c_str(), svc);
			notify_changed();
		}
	}

	void Enum::pause(iterator it)
	{
		if (it != end()) {
			LogNoise(L"%s\n", it->name.c_str());
			Item svc(Item::pause(m_filter->get_read_manager(), it->name.c_str()));
			if (m_wait_state) {
				svc.wait_state(State::PAUSED, m_wait_timout);
			}
			*it = Info(it->name.c_str(), svc);
			notify_changed();
		}
	}

	void Enum::set_config(iterator it, const ConfigRequest& request)
	{
		if (it != end()) {
			LogNoise(L"%s\n", it->name.c_str());
			request.log();
			*it = Info(it->name.c_str(), Item::set_config(m_filter->get_read_manager(), it->name.c_str(), request));
			notify_changed();
		}
	}

	void Enum::start_batch()
	{
		LogTrace();
		m_batch_started = true;
	}

	void Enum::notify_changed()
	{
		set_changed(true);
		if (!m_batch_started) {
			LogTrace();
			notify_observers(sync::Message());
		}
	}

	void Enum::stop_batch()
	{
		LogTrace();
		m_batch_started = false;
		notify_observers(sync::Message());
	}

	void Enum::set_wait_state(bool new_state)
	{
		m_wait_state = new_state;
	}

	bool Enum::get_wait_state() const
	{
		return m_wait_state;
	}

	void Enum::set_wait_timeout(size_t timeout_msec)
	{
		m_wait_timout = timeout_msec;
	}

	size_t Enum::get_wait_timeout() const
	{
		return m_wait_timout;
	}

}
