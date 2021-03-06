﻿#ifndef EXCIS_CONNECTION_REMOTE_HPP_
#define EXCIS_CONNECTION_REMOTE_HPP_

#include <excis/connection.hpp>

namespace connection {

	struct Remote {
		static Remote* create();

		static Remote* create(const ustring& host, const wchar_t* user = nullptr, const wchar_t* pass = nullptr);

		virtual ~Remote() = default;

		virtual ustring get_host() const = 0;

		virtual void disconnect() = 0;

		void connect();

		void connect(const ustring& host, const wchar_t* user = nullptr, const wchar_t* pass = nullptr);

	private:
		virtual void do_connect(const ustring& host, const wchar_t* user, const wchar_t* pass) = 0;
	};

}

#endif
