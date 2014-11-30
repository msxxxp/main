/**
	win_httpmgr
	Interface to IIS
	@classes	()
	@author		Copyright � 2009 Andrew Grechkin
	@link		(httpapi)
	@link		(wsock32)
**/

#include <excis/httpmgr.hpp>

#include <basis/sys/sstr.hpp>

#include <excis/exception.hpp>

#include <basis/simstd/string>

namespace Ext {

namespace Http {
#ifndef _MSC_VER
	///====================================================================================== HttpBindIP
	HttpBindIP::~HttpBindIP() {
		delete pIpPort;
	}

	HttpBindIP::HttpBindIP(const ustring& ipport) {
		pIpPort = new (sockaddr);
		ustring	port = ipport;
		ustring	ip = sstr::CutWord(port, L":");
		Assign(ip, port);
	}

	HttpBindIP::HttpBindIP(const ustring& ip, const ustring& port) {
		pIpPort = new (sockaddr);
		Assign(ip, port);
	}

	HttpBindIP::HttpBindIP(const HTTP_SERVICE_CONFIG_SSL_KEY & in) {
		pIpPort = new (sockaddr);
		memory::copy(pIpPort, in.pIpPort, sizeof(sockaddr));
	}

	HttpBindIP::HttpBindIP(const HttpBindIP & in) {
		pIpPort = new (sockaddr);
		memory::copy(pIpPort, in.pIpPort, sizeof(sockaddr));
	}

	HttpBindIP & HttpBindIP::operator=(const HttpBindIP & in) {
		if (this != &in) {
			HttpBindIP(in).swap(*this);
		}
		return *this;
	}

	bool HttpBindIP::operator==(const HttpBindIP & rhs) const {
		sockaddr_in *sa1 = (sockaddr_in*)pIpPort;
		sockaddr_in *sa2 = (sockaddr_in*)rhs.pIpPort;
		return (sa1->sin_port == sa2->sin_port) && (sa1->sin_addr.s_addr == sa2->sin_addr.s_addr);
	}

	ustring HttpBindIP::get_ip() const {
		sockaddr_in * tmp = (sockaddr_in*)pIpPort;
		return sstr::cp2w(inet_ntoa(tmp->sin_addr), CP_UTF8);
	}

	ustring HttpBindIP::get_port() const {
		sockaddr_in * tmp = (sockaddr_in*)pIpPort;
		return simstd::to_wstring(ntohs(tmp->sin_port));
	}

	ustring	HttpBindIP::as_str() const {
		return get_ip() + L":" + get_port();
	}

	bool HttpBindIP::copy(HTTP_SERVICE_CONFIG_SSL_KEY & out) const {
		out.pIpPort = memory::malloc<PSOCKADDR>(sizeof(SOCKADDR), HEAP_ZERO_MEMORY);
		memory::copy(out.pIpPort, pIpPort, sizeof(*pIpPort));
		return true;
	}

	void HttpBindIP::swap(HttpBindIP & rhs) {
		using simstd::swap;
		swap(pIpPort, rhs.pIpPort);
	}

	bool HttpBindIP::is_valid(const ustring& ip) {
		in_addr	addr;
		addr.s_addr = inet_addr(sstr::utf8(ip).c_str());
		return addr.s_addr != INADDR_NONE;
	}

	bool HttpBindIP::Assign(const ustring& ip, const ustring& port) {
		u_short	prt = htons(cstr::to_uint32(port.c_str()));
		if (is_valid(ip) && prt) {
			sockaddr_in *tmp = (sockaddr_in*)pIpPort;
			tmp->sin_port		= prt;
			tmp->sin_addr.s_addr = inet_addr(sstr::utf8(ip).c_str());
			tmp->sin_family		= AF_INET;
			return true;
		}
		return false;
	}

	///=================================================================================== HttpBindParam
	HttpBindParam::~HttpBindParam() {
		memory::free(pSslHash);
	}

	HttpBindParam::HttpBindParam(const ustring& hash) {
		memory::zero(this, sizeof(*this));
		PBYTE buf;
		size_t size;
		to_hash(hash, buf, size);
		pSslHash = buf;
		SslHashLength = size;
		pSslCertStoreName = (PWSTR)L"MY";
	}

	bool HttpBindParam::copy(HTTP_SERVICE_CONFIG_SSL_PARAM & out) const {
		memory::zero(&out, sizeof(out));

//		out.AppId = GUID_of_application;
		out.DefaultFlags = HTTP_SERVICE_CONFIG_SSL_FLAG_NEGOTIATE_CLIENT_CERT;
		out.pSslCertStoreName = (PWSTR)L"MY";

		out.SslHashLength = SslHashLength;
		out.pSslHash = memory::malloc<PVOID>(out.SslHashLength, HEAP_ZERO_MEMORY);
		memory::copy(out.pSslHash, pSslHash, out.SslHashLength);
		return true;
	}

	ustring as_str(const HTTP_SERVICE_CONFIG_SSL_PARAM & m_data) {
		return to_str((PBYTE)m_data.pSslHash, m_data.SslHashLength);
	}

	///==================================================================================== SslQuery
	SslQuery::~SslQuery() {
		memory::free(KeyDesc.pIpPort);
	}

	SslQuery::SslQuery() {
		QueryDesc = HttpServiceConfigQueryNext;
		memory::zero(&KeyDesc, sizeof(KeyDesc));
		dwToken = 0;
	}

	SslQuery::SslQuery(const HttpBindIP & ip) {
		QueryDesc = HttpServiceConfigQueryExact;
		ip.copy(KeyDesc);
		dwToken = 0;
	}

	SslQuery & SslQuery::operator ++() {
		++dwToken;
		return *this;
	}

	///====================================================================================== Server
	SslSet::~SslSet() {
		delete	KeyDesc.pIpPort;
	}

	SslSet::SslSet(const HttpBindIP & ip, const HttpBindParam & param) {
		memory::zero(&KeyDesc, sizeof(KeyDesc));
		memory::zero(&ParamDesc, sizeof(ParamDesc));
		ip.copy(KeyDesc);
		param.copy(ParamDesc);
	}

	///====================================================================================== Server
	Server::~Server() {
		::HttpTerminate(HTTP_INITIALIZE_CONFIG, NULL);
	}

	Server::Server() {
		HTTPAPI_VERSION	httpVer = HTTPAPI_VERSION_1;
		CheckApiError(::HttpInitialize(httpVer, HTTP_INITIALIZE_CONFIG, nullptr));
	}

	bool Server::get_ssl(const HttpBindIP & ip, memory::auto_buf<PHTTP_SERVICE_CONFIG_SSL_SET> & info) const {
		SslQuery query(ip);
		return get_ssl(query, info);
	}

	bool Server::get_ssl(SslQuery & query, memory::auto_buf<PHTTP_SERVICE_CONFIG_SSL_SET> & info) const {
		ULONG ReturnLength = 0;
		ULONG err =::HttpQueryServiceConfiguration(NULL, HttpServiceConfigSSLCertInfo, &query, sizeof(query),
		                                           info.data(), info.size(), &ReturnLength, NULL);
		if (err == ERROR_MORE_DATA) {
			info.reserve(ReturnLength);
			err = ::HttpQueryServiceConfiguration(NULL, HttpServiceConfigSSLCertInfo, &query, sizeof(query),
			                                      info.data(), info.size(), &ReturnLength, NULL);
		}
		if (err == NO_ERROR) {
			++query;
			return true;
		}
		return false;
	}

	void Server::set(const SslSet & info) const {
		CheckApiError(::HttpSetServiceConfiguration(NULL, HttpServiceConfigSSLCertInfo, (PVOID)&info, sizeof(info), NULL));
	}

	void Server::del(const HttpBindIP & ip) const {
		HTTP_SERVICE_CONFIG_SSL_SET	info;
		memory::zero(info);
		info.KeyDesc = ip;
		CheckApiError(::HttpDeleteServiceConfiguration(NULL, HttpServiceConfigSSLCertInfo, (PVOID) &info, sizeof(info), NULL));
	}

	bool Server::find(const ustring& hash) const {
		SslQuery query;
		memory::auto_buf<PHTTP_SERVICE_CONFIG_SSL_SET> info;
		while (get_ssl(query, info)) {
			if (hash == as_str(info->ParamDesc))
				return true;
		}
		return false;
	}

	bool Server::is_exist(const ustring& ip, const ustring& port) {
		memory::auto_buf<PHTTP_SERVICE_CONFIG_SSL_SET> info(512);
		return get_ssl(HttpBindIP(ip, port), info) || get_ssl(HttpBindIP(L"0.0.0.0", port), info);
	}
#endif
}

}
