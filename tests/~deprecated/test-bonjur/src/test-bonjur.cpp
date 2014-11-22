#include <dns_sd.h>
#include <waiter.hpp>

#include <basis/sys/logger.hpp>

//class A {
//public:
//	A(const char * name);
//
//	static void reply_callback(DNSServiceRef ref, DNSServiceFlags flags, uint32_t ifIndex, DNSServiceErrorType errorCode, const char *fullname, uint16_t rrtype, uint16_t rrclass, uint16_t rdlen,
//	                           const void *rdata, uint32_t ttl, void *context)
//	{
//		reinterpret_cast<A*>(context)->reply(ref, flags, ifIndex, errorCode, fullname, rrtype, rrclass, rdlen, rdata, ttl);
//	}
//
//	void reply(DNSServiceRef ref, DNSServiceFlags flags, uint32_t ifIndex, DNSServiceErrorType errorCode, const char *fullname, uint16_t rrtype, uint16_t rrclass, uint16_t rdlen, const void *rdata,
//	           uint32_t ttl)
//	{
//		LogDebug(L"ref:    %p\n", ref);
//		LogDebug(L"flags:  0x%X\n", flags);
//		LogDebug(L"index:  0x%X\n", ifIndex);
//		LogDebug(L"error:  0x%X\n", errorCode);
//		LogDebug(L"name:   '%S'\n", fullname);
////		LogDebug(L"type:   '%S'\n", replyType);
////		LogDebug(L"dom:    '%S'\n", replyDomain);
////		LogDebug(L"conext: %p\n", context);
//	}
//
//private:
//	DNSServiceRef m_ref;
//};
//
//A::A(const char * name) :
//	m_ref(nullptr)
//{
//	DNSServiceQueryRecord(&m_ref, 0, 0, name, kDNSServiceType_PTR, kDNSServiceClass_IN, reply_callback, this);
//}

static void DNSSD_API browse_reply(DNSServiceRef ref, const DNSServiceFlags flags, uint32_t ifIndex, DNSServiceErrorType errorCode, const char *replyName, const char *replyType,
                                   const char *replyDomain, void *context)
{
	LogDebug(L"ref:    %p\n", ref);
	LogDebug(L"flags:  0x%X\n", flags);
	LogDebug(L"index:  0x%X\n", ifIndex);
	LogDebug(L"error:  0x%X\n", errorCode);
	LogDebug(L"name:   '%S'\n", replyName);
	LogDebug(L"type:   '%S'\n", replyType);
	LogDebug(L"dom:    '%S'\n", replyDomain);
	LogDebug(L"conext: %p\n", context);

	new A(replyName);
//	char *op = (flags & kDNSServiceFlagsAdd) ? "Add" : "Rmv";
//	if (num_printed++ == 0)
//		printf("Timestamp     A/R Flags if %-25s %-25s %s\n", "Domain", "Service Type", "Instance Name");
//	printtimestamp();
//	if (errorCode)
//		printf("Error code %d\n", errorCode);
//	else
//		printf("%s%6X%3d %-25s %-25s %s\n", op, flags, ifIndex, replyDomain, replyType, replyName);
//	if (!(flags & kDNSServiceFlagsMoreComing))
//		fflush (stdout);

// To test selective cancellation of operations of shared sockets,
// cancel the current operation when we've got a multiple of five results
//if (operation == 'S' && num_printed % 5 == 0) DNSServiceRefDeallocate(sdref);
}

void setup_logger()
{
	LogSetOptions(L"logger:///default?level=tr;prefix=fu;target=co");
}

int main()
{
	setup_logger();

	LogTrace();
	{
		uint32_t version;
		uint32_t size = sizeof(version);
		DNSServiceErrorType err = DNSServiceGetProperty(kDNSServiceProperty_DaemonVersion, &version, &size);
		LogErrorIf(err != kDNSServiceErr_NoError, L"%d\n", err);
		LogReportIf(err == kDNSServiceErr_NoError, L"Bonjour version is %d.%d\n", version / 10000, version / 100 % 100);
	}

	waiter::start_thread();

//	typ = (argc < opi + 1) ? "" : argv[opi + 0];
//	dom = (argc < opi + 2) ? "" : argv[opi + 1];  // Missing domain argument is the same as empty string i.e. use system default(s)
//	typ = gettype(buffer, typ);
//	if (dom[0] == '.' && dom[1] == 0)
//		dom[0] = 0;   // We allow '.' on the command line as a synonym for empty string
//	printf("Browsing for %s%s%s\n", typ, dom[0] ? "." : "", dom);

	LogTrace();
	DNSServiceRef client;
	DNSServiceErrorType err = DNSServiceBrowse(&client, 0, 0, "_services._dns-sd._udp.", "", browse_reply, nullptr);

	LogError(L"%d\n", err);

	if (err == kDNSServiceErr_NoError)
		waiter::add_ref(client);

	return 0;
}
