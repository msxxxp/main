#include <basis/sys/logger.hpp>
#include <basis/std/string>

#include <excis/connection.hpp>
#include <excis/dll.hpp>
#include <excis/exception.hpp>

namespace {
	struct Mpr_dll: private Ext::DynamicLibrary {
		typedef DWORD (WINAPI *FWNetAddConnection2W)(LPNETRESOURCEW, LPCWSTR, LPCWSTR, DWORD);
		typedef DWORD (WINAPI *FWNetCancelConnection2W)(LPCWSTR, DWORD, WINBOOL);

		DEFINE_FUNC(WNetAddConnection2W);
		DEFINE_FUNC(WNetCancelConnection2W);

		static Mpr_dll & inst()
		{
			static Mpr_dll ret;
			return ret;
		}

	private:
		Mpr_dll() :
			Ext::DynamicLibrary(L"mpr.dll")
		{
			GET_DLL_FUNC(WNetAddConnection2W);
			GET_DLL_FUNC(WNetCancelConnection2W);
		}
	};

	ustring make_IPC_string(const ustring & host)
	{
		PCWSTR prefix = (host[0] != PATH_SEPARATOR_C || host[1] != PATH_SEPARATOR_C) ? NETWORK_PATH_PREFIX : EMPTY_STR;

		wchar_t ipc[MAX_PATH];
		safe_snprintf(ipc, lengthof(ipc), L"%s%s%s", prefix, host.c_str(), L"\\IPC$");

		return ustring(ipc);
	}
}

namespace connection {

	struct RemoteImpl: public Remote {
		~RemoteImpl();

		RemoteImpl(const ustring & host, const wchar_t * user, const wchar_t * pass);

		void disconnect() override;

		ustring get_host() const override;

	private:
		void do_connect(const ustring & host, const wchar_t * user, const wchar_t * pass) override;

		ustring m_host;
		bool m_connected;
	};

	RemoteImpl::~RemoteImpl()
	{
		LogTraceObjBegin();
		disconnect();
		LogTraceObjEnd();
	}

	RemoteImpl::RemoteImpl(const ustring & host, const wchar_t * user, const wchar_t * pass) :
		m_connected(false)
	{
		LogTraceObjBegin();
		do_connect(host, user, pass);
		LogTraceObjEnd();
	}

	void RemoteImpl::disconnect()
	{
		if (m_connected) {
			ustring ipc = make_IPC_string(m_host);
			LogDebug(L"'%s'\n", m_host.c_str());
			CheckApiError(Mpr_dll::inst().WNetCancelConnection2W(ipc.c_str(), 0, FALSE));
			m_connected = false;
		}
		m_host.clear();
	}

	ustring RemoteImpl::get_host() const
	{
		return m_host;
	}

	void RemoteImpl::do_connect(const ustring & host, PCWSTR user, PCWSTR pass)
	{
		disconnect();
		if (!host.empty()) {
			NETRESOURCE NetRes;
			memory::zero(NetRes);
			ustring ipc = make_IPC_string(host);
			LogDebug(L"'%s:%s@%s' \n", user, pass, host.c_str());
			NetRes.dwType = RESOURCETYPE_ANY;
			NetRes.lpRemoteName = (PWSTR)ipc.c_str();
			if (cstr::is_empty(user)) {
				user = nullptr;
				pass = nullptr;
			}
			CheckApiError(Mpr_dll::inst().WNetAddConnection2W(&NetRes, pass, user, 0));
			m_host = host;
			m_connected = true;
		}
		//	CheckApiError(ERROR_BAD_NETPATH);
	}
}

namespace connection {
	Remote::~Remote()
	{
	}

	void Remote::connect()
	{
		do_connect(ustring(), nullptr, nullptr);
	}

	void Remote::connect(const ustring & host, const wchar_t * user, const wchar_t * pass)
	{
		do_connect(host, user, pass);
	}

	Remote * Remote::create()
	{
		return new RemoteImpl(ustring(), nullptr, nullptr);
	}

	Remote * Remote::create(const ustring & host, const wchar_t * user, const wchar_t * pass)
	{
		return new RemoteImpl(host, user, pass);
	}
}

