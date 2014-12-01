#ifndef EXCIS_SERVICE_MANAGER_HPP_
#define EXCIS_SERVICE_MANAGER_HPP_

namespace service {

	class Manager;
	class Item;
	class Enum;
	class Filter;
	class CreateRequest;
	class ConfigRequest;
	class ConfigLogonRequest;

	enum class EnumerateType: size_t {
		SERVICES = SERVICE_WIN32,
		DRIVERS = SERVICE_DRIVER,
//		ADAPTERS = SERVICE_ADAPTER,
	};

	enum class Type: size_t {
		KERNEL_DRIVER = SERVICE_KERNEL_DRIVER,
		FILE_SYSTEM_DRIVER = SERVICE_FILE_SYSTEM_DRIVER,
		ADAPTER = SERVICE_ADAPTER,
		RECOGNIZER_DRIVER = SERVICE_RECOGNIZER_DRIVER,
		WIN32_OWN_PROCESS = SERVICE_WIN32_OWN_PROCESS,
		WIN32_SHARE_PROCESS = SERVICE_WIN32_SHARE_PROCESS,
		INTERACTIVE_PROCESS = SERVICE_INTERACTIVE_PROCESS,
	};

	enum class State: size_t {
		STOPPED = 0x00000001,
		STARTING = 0x00000002,
		STOPPING = 0x00000003,
		STARTED = 0x00000004,
		CONTINUING = 0x00000005,
		PAUSING = 0x00000006,
		PAUSED = 0x00000007,
	};

	enum class Start: size_t {
		BOOT = SERVICE_BOOT_START,
		SYSTEM = SERVICE_SYSTEM_START,
		AUTO = SERVICE_AUTO_START,
		AUTO_DELAYED = SERVICE_AUTO_START | 0x10000,
		DEMAND = SERVICE_DEMAND_START,
		DISABLED = SERVICE_DISABLED,
	};

	enum class Error: size_t {
		IGNORE_ERROR = SERVICE_ERROR_IGNORE,
		NORMAL = SERVICE_ERROR_NORMAL,
		SEVERE = SERVICE_ERROR_SEVERE,
		CRITICAL = SERVICE_ERROR_CRITICAL,
	};

	class Manager: pattern::Uncopyable {
	public:
		~Manager();

		Manager(connection::Remote* conn = nullptr, ACCESS_MASK acc = SC_MANAGER_CONNECT);

		Manager(Manager&& other);

		Manager& operator =(Manager&& other);

		void swap(Manager& other);

		SC_HANDLE get_handle() const;

		void reconnect(connection::Remote* conn = nullptr, ACCESS_MASK acc = SC_MANAGER_CONNECT);

		bool is_exist(const wchar_t* name) const;

	private:
		SC_HANDLE m_hndl;
	};

	class CreateRequest: private pattern::Uncopyable {
	public:
		CreateRequest(const ustring& name, const ustring& binaryPathName);
		void set_type(Type n);
		void set_start(Start n);
		void set_error_control(Error n);
		void set_group(const wchar_t* n);
		void set_tag(DWORD& n);
		void set_dependencies(const wchar_t* n);
		void set_display_name(const wchar_t* n);
//		const wchar_t* get_name() const;

		Item execute(const Manager& manager) const;

	private:
		ustring name;
		ustring displayName;
		ustring binaryPathName;
		const wchar_t* dependencies;
		const wchar_t* loadOrderGroup;
		const wchar_t* login;
		const wchar_t* passw;
		PDWORD tagId;
		DWORD serviceType;
		DWORD startType;
		DWORD errorControl;
	};

	class ConfigRequest: private pattern::Uncopyable {
	public:
		ConfigRequest();
		void set_type(Type n, Type o);
		void set_start(Start n, Start o);
		void set_error_control(Error n, Error o);
		void set_path(const wchar_t* n, const wchar_t* o);
		void set_group(const wchar_t* n, const wchar_t* o);
		void set_tag(DWORD& n, DWORD o);
		void set_dependencies(const wchar_t* n, const wchar_t* o);
		void set_display_name(const wchar_t* n, const wchar_t* o);
		void set_login(const wchar_t* user, const wchar_t* pass = nullptr);
		void log() const;

		Item& execute(Item& svc) const;

	private:
		const wchar_t* binaryPathName;
		const wchar_t* loadOrderGroup;
		const wchar_t* dependencies;
		const wchar_t* displayName;
		const wchar_t* login;
		const wchar_t* passw;
		PDWORD tagId;
		DWORD serviceType;
		DWORD startType;
		DWORD errorControl;
	};

	struct Status: public SERVICE_STATUS_PROCESS {
	};

	///====================================================================================== Info
	struct Info {
		ustring name;
		ustring displayName;
		ustring binaryPathName;
		ustring loadOrderGroup;
		ustring serviceStartName;
		ustring description;
		Start startType;
		Error errorControl;
		DWORD tagId;
		cstr::mstring dependencies;
		SERVICE_STATUS_PROCESS status;

		Info(SC_HANDLE scm, const ENUM_SERVICE_STATUS_PROCESSW& st);
		Info(const wchar_t* _name, const Item& svc);

		bool operator <(const Info& rhs) const;
		bool operator ==(const ustring& nm) const;

		Type get_type() const {
			return (Type)status.dwServiceType;
		}

		State get_state() const {
			return (State)status.dwCurrentState;
		}

		bool is_service() const {
			return static_cast<DWORD>(get_type()) & (SERVICE_WIN32_OWN_PROCESS | SERVICE_WIN32_SHARE_PROCESS);
		}

		bool is_disabled() const {
			return startType == Start::DISABLED;
		}
	};

}

#endif
