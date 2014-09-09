#ifndef _LIBEXT_SERVICES_HPP_
#define _LIBEXT_SERVICES_HPP_

#include <basis/configure.hpp>
#include <basis/sys/sync.hpp>
#include <excis/service.hpp>

#include <basis/std/iosfwd>
#include <basis/std/memory>
#include <basis/std/vector>

namespace Ext {

	struct Services: public sync::Observable, private simstd::vector<Service::Info_t> {

		struct Filter;

		typedef Service::Info_t value_type;
		typedef simstd::vector<Service::Info_t> base_type;

		typedef base_type::size_type size_type;
		typedef base_type::iterator iterator;
		typedef base_type::const_iterator const_iterator;

		using base_type::begin;
		using base_type::cbegin;
		using base_type::end;
		using base_type::cend;
		using base_type::size;
		using base_type::empty;
		using base_type::operator[];

		~Services();
		Services(const ustring & host = ustring(), PCWSTR user = nullptr, PCWSTR pass = nullptr);

		Service::EnumerateType_t get_type() const;
		void set_type(Service::EnumerateType_t type);

		ustring get_host() const;
		void set_host(const ustring & host = ustring(), PCWSTR user = nullptr, PCWSTR pass = nullptr);

		void update();

		iterator find(const ustring & name);
		const_iterator find(const ustring & name) const;

		void add(const Service::Create_t & info);

		void del(iterator it);

		void start(iterator it);
		void stop(iterator it);
		void restart(iterator it);
		void contin(iterator it);
		void pause(iterator it);
		void set_config(iterator it, const Service::Config_t & info);
		void set_logon(iterator it, const Service::Logon_t & info);

		void start_batch();
		void notify_changed();
		void stop_batch();

		void set_wait_state(bool new_state);
		bool get_wait_state() const;

		void set_wait_timeout(size_t timeout_msec);
		size_t get_wait_timeout() const;

	private:
		simstd::shared_ptr<Filter> m_filter;
		Service::EnumerateType_t   m_type;
		size_t                     m_wait_timout;
		size_t                     m_wait_state :1;
		size_t                     m_batch_started :1;
	};

}

#endif