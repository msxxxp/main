#ifndef WAITER_HPP_
#define WAITER_HPP_

#include <dns_sd.h>

namespace waiter {
	void start_thread();

	void stop_thread();

	void add_ref(DNSServiceRef ref);

	void close_ref(DNSServiceRef ref);
}

#endif
