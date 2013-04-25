#include <libbase/std.hpp>
#include <libbase/logger.hpp>

struct Settings
{
	~Settings()
	{
		LogTrace();
	}

	Settings():
		m_isNetworkAllowed(false),
		m_isSystem(false),
		m_numberOfErrorsToStop(20),
		m_numberOfListeners(0),
		m_numberOfWorkers(1),
		m_listenerPriority(Base::Thread::Priority_t::HIGHEST),
		m_workerPriority(Base::Thread::Priority_t::LOWEST),
		m_workerIoPriority(Base::Thread::IoPriority_t::LOW)
	{
		LogTrace();
	}

	static Settings & inst()
	{
		static Settings instance;
		return instance;
	}

	void init(PCWSTR commandLine)
	{
		init_from_commline(commandLine);
		init_from_register();
	}

	bool isNetworkAllowed() const
	{
		return	m_isNetworkAllowed;
	}

	size_t numberOfListeners() const
	{
		return	m_numberOfListeners;
	}

	size_t numberOfWorkers() const
	{
		return	m_numberOfWorkers;
	}

	Base::Thread::Priority_t listenerPriority() const
	{
		return	m_listenerPriority;
	}

	Base::Thread::Priority_t workerPriority() const
	{
		return	m_workerPriority;
	}

	Base::Thread::IoPriority_t workerIoPriority() const
	{
		return	m_workerIoPriority;
	}


private:
	void init_from_commline(PCWSTR commandLine)
	{
		LogNoise(L"'%s'\n", commandLine);
	}

	void init_from_register()
	{
		LogTrace();
	}

	uint64_t m_isNetworkAllowed:1;
	uint64_t m_isSystem:1;
	size_t m_numberOfErrorsToStop;
	size_t m_numberOfListeners;
	size_t m_numberOfWorkers;
	Base::Thread::Priority_t m_listenerPriority;
	Base::Thread::Priority_t m_workerPriority;
	Base::Thread::IoPriority_t m_workerIoPriority;
};
