#include <basis/sys/logger.hpp>

#include <BonjurEventWaiter.hpp>

#include <set>
#include <thread>

class BonjurEventWaiter
{
public:
	BonjurEventWaiter();

	void add_ref(DNSServiceRef ref);

	void close_ref(DNSServiceRef ref);

	void wait();

private:
	std::set<DNSServiceRef> m_refs;
};

BonjurEventWaiter::BonjurEventWaiter()
{
}

void BonjurEventWaiter::add_ref(DNSServiceRef ref)
{

}

void BonjurEventWaiter::close_ref(DNSServiceRef ref)
{
}

void BonjurEventWaiter::wait()
{
	LogTrace();
	int dns_sd_fd = client ? DNSServiceRefSockFD(client) : -1;
	LogDebug(L"dns_sd_fd: %d\n", dns_sd_fd);
	//	int dns_sd_fd2 = client_pa ? DNSServiceRefSockFD(client_pa) : -1;
	int nfds = dns_sd_fd + 1;
	LogDebug(L"nfds: %d\n", nfds);

	fd_set readfds;
	struct timeval tv;
	int result;

	//	if (dns_sd_fd2 > dns_sd_fd)
	//		nfds = dns_sd_fd2 + 1;
	LogDebug(L"dns_sd_fd: %d\n", dns_sd_fd);

	while (true) {
		LogTrace();
		// 1. Set up the fd_set as usual here.
		// This example client has no file descriptors of its own,
		// but a real application would call FD_SET to add them to the set here
		FD_ZERO(&readfds);

		// 2. Add the fd for our client(s) to the fd_set
		if (client)
			FD_SET(dns_sd_fd, &readfds);
	//		if (client_pa)
	//			FD_SET(dns_sd_fd2, &readfds);

		// 3. Set up the timeout.
		tv.tv_sec = 100000000;
		tv.tv_usec = 0;

		result = select(nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv);
		LogDebug(L"result: %d\n", result);

		if (result > 0) {
			DNSServiceErrorType err = kDNSServiceErr_NoError;
			if (client && FD_ISSET(dns_sd_fd, &readfds))
				err = DNSServiceProcessResult(client);
	//			else if (client_pa && FD_ISSET(dns_sd_fd2, &readfds))
	//				err = DNSServiceProcessResult(client_pa);
			if (err) {
	//				fprintf(stderr, "DNSServiceProcessResult returned %d\n", err);
				break;
			}
	//		} else if (result == 0)
	//			myTimerCallBack();
	//		else {
	//			printf("select() returned %d errno %d %s\n", result, errno, strerror(errno));
	//			if (errno != EINTR)
	//				stopNow = 1;
		}
	}
}
