#ifndef EXCIS_SERVICE_ITEM_HPP_
#define EXCIS_SERVICE_ITEM_HPP_

namespace service {

	class Item: pattern::Uncopyable {
		typedef Item this_type;

	public:
		~Item();

		Item(SC_HANDLE scm, const wchar_t* name, ACCESS_MASK access = SERVICE_QUERY_CONFIG);
		Item(this_type&& other);
		Item& operator =(this_type&& other);

		operator SC_HANDLE() const;

		void del();

		this_type& start();
		this_type& stop();
		this_type& restart();
		this_type& contin();
		this_type& pause();

		this_type& set_description(const wchar_t* info);
		this_type& set_delayed(bool state);

		this_type& wait_state(State state, DWORD dwTimeout);

		ustring get_description() const;
		bool get_delayed() const;
		Status get_status() const;
		State get_state() const;
		Start get_start_type() const;
		DWORD get_type() const;
		ustring get_user() const;

		static void del(SC_HANDLE scm, const wchar_t* name);
		static this_type start(SC_HANDLE scm, const wchar_t* name);
		static this_type stop(SC_HANDLE scm, const wchar_t* name);
		static this_type restart(SC_HANDLE scm, const wchar_t* name);
		static this_type contin(SC_HANDLE scm, const wchar_t* name);
		static this_type pause(SC_HANDLE scm, const wchar_t* name);

		static this_type set_config(SC_HANDLE scm, const wchar_t* name, const ConfigRequest& request);
		static this_type set_desription(SC_HANDLE scm, const wchar_t* name, const wchar_t* info);

		static Status get_status(SC_HANDLE scm, const wchar_t* name);
		static State get_state(SC_HANDLE scm, const wchar_t* name);
		static Start get_start_type(SC_HANDLE scm, const wchar_t* name);
		static ustring get_description(SC_HANDLE scm, const wchar_t* name);

	private:
		Item(SC_HANDLE svc);

		SC_HANDLE m_hndl;

		friend class CreateRequest;
	};

}

#endif
