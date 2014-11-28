#include <basis/configure.hpp>
#include <basis/sys/sync.hpp>

namespace sync {

	Observer::~Observer()
	{
		m_manager->unregister_all(this);
	}

	Observer::Observer() :
		m_manager(MessageManager::get_default())
	{
	}

}
