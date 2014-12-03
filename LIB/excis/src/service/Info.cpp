#include "service_pvt.hpp"

#include <excis/exception.hpp>

#include <basis/sys/logger.hpp>

namespace service {

	Info::Info(SC_HANDLE scm, const ENUM_SERVICE_STATUS_PROCESSW& st):
		name(st.lpServiceName),
		displayName(st.lpDisplayName),
		startType(Start::DISABLED),
		errorControl(Error::IGNORE_ERROR),
		tagId(0),
		status(st.ServiceStatusProcess)
	{
		try {
			Item svc(scm, name.c_str(), SERVICE_QUERY_CONFIG);
			auto conf(QueryConfig(svc));
			binaryPathName = conf->lpBinaryPathName;
			loadOrderGroup = conf->lpLoadOrderGroup;
			dependencies = conf->lpDependencies;
			serviceStartName = conf->lpServiceStartName;
			startType = (conf->dwStartType == (DWORD)Start::AUTO && svc.get_delayed()) ? Start::AUTO_DELAYED : static_cast<Start>(conf->dwStartType);
			errorControl = (Error)conf->dwErrorControl;
			tagId = conf->dwTagId;
			description = svc.get_description();
		} catch (exception::Abstract & e) {
			LogWarn(L"exception cought: %s, %s\n", e.what(), e.where());
			LogWarn(L"service: %s unavalible\n", name.c_str());
			// skip query info
		}
	}

	Info::Info(const wchar_t* name, const Item& svc):
		name(name),
		startType(Start::DISABLED),
		errorControl(Error::IGNORE_ERROR),
		tagId(0)
	{
		LogTrace();
		memset(&status, 0, sizeof(status));
		try {
			auto conf(QueryConfig(svc));
			displayName = conf->lpDisplayName;
			status = svc.get_status();
			binaryPathName = conf->lpBinaryPathName;
			loadOrderGroup = conf->lpLoadOrderGroup;
			dependencies = conf->lpDependencies;
			serviceStartName = conf->lpServiceStartName;
			startType = (conf->dwStartType == (DWORD)Start::AUTO && svc.get_delayed()) ? Start::AUTO_DELAYED : static_cast<Start>(conf->dwStartType);
			errorControl = static_cast<Error>(conf->dwErrorControl);
			tagId = conf->dwTagId;
			description = svc.get_description();
		} catch (exception::Abstract& e) {
			LogWarn(L"exception cought: %s, %s\n", e.what(), e.where());
			LogWarn(L"service: %s unavalible handle: %p\n", name, (SC_HANDLE)svc);
			// skip query info
		}
	}

	bool Info::operator <(const Info& other) const {
		return name < other.name;
	}

	bool Info::operator ==(const ustring& name) const {
		return this->name == name;
	}

}
