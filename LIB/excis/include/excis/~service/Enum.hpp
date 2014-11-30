#ifndef EXCIS_SERVICE_ENUM_HPP_
#define EXCIS_SERVICE_ENUM_HPP_

namespace service {

	struct Enum: public sync::Observable, private simstd::vector<Info> {

		typedef Info value_type;
		typedef simstd::vector<Info> base_type;

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

		~Enum();
		Enum(const ustring& host = ustring(), PCWSTR user = nullptr, PCWSTR pass = nullptr);

		EnumerateType get_type() const;
		void set_type(EnumerateType type);

		ustring get_host() const;
		void set_host(const ustring& host = ustring(), PCWSTR user = nullptr, PCWSTR pass = nullptr);

		void update();

		iterator find(const ustring& name);
		const_iterator find(const ustring& name) const;

		void add(const CreateRequest& info);

		void del(iterator it);

		void start(iterator it);
		void stop(iterator it);
		void restart(iterator it);
		void contin(iterator it);
		void pause(iterator it);
		void set_config(iterator it, const ConfigRequest& info);
		void set_logon(iterator it, const ConfigLogonRequest& info);

		void start_batch();
		void notify_changed();
		void stop_batch();

		void set_wait_state(bool new_state);
		bool get_wait_state() const;

		void set_wait_timeout(size_t timeout_msec);
		size_t get_wait_timeout() const;

	private:
		simstd::shared_ptr<Filter> m_filter;
		EnumerateType              m_type;
		size_t                     m_wait_timout;
		size_t                     m_wait_state :1;
		size_t                     m_batch_started :1;
	};

}

#endif
