#include <basis/configure.hpp>
#include <basis/sys/sync.hpp>

namespace sync {

	Observer::~Observer()
	{
		m_manager->unregister_all(this);
	}

	Observer::Observer() :
		m_manager(get_simple_message_manager())
	{
	}

	Observer::Observer(MessageManager* manager) :
		m_manager(manager)
	{
	}


}
