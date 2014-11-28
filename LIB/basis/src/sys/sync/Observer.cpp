#include <basis/configure.hpp>
#include <basis/sys/sync.hpp>

namespace sync {

	Observer::~Observer()
	{
		m_manager->unregister_observer(this);
	}

	Observer::Observer() :
		m_manager(MessageManager::get_default())
	{
	}

	Observer::Observer(const Observable* subject):
		Observer()
	{
		subject->register_observer(this);
	}
}

