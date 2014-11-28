#include <basis/configure.hpp>
#include <basis/sys/sync.hpp>

namespace sync {

	Observable::~Observable()
	{
		m_manager->unregister_all(this);
	}

	Observable::Observable() :
		m_manager(MessageManager::get_default()),
		m_changed(false)
	{
	}

	void Observable::register_observer(Observer * observer) const
	{
		m_manager->register_observer(this, observer);
	}

	void Observable::unregister_observer(Observer * observer) const
	{
		m_manager->unregister_observer(this, observer);
	}

	void Observable::notify_observers(const Message & event) const
	{
		if (m_changed) {
			m_manager->notify(this, event);
			m_changed = false;
		}
	}

	void Observable::set_changed(bool changed) const
	{
		m_changed = changed;
	}

	bool Observable::get_changed() const
	{
		return m_changed;
	}

}
