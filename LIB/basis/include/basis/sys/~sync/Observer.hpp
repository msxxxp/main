#ifndef BASIS_SYS_SYNC_OBSERVER_HPP_
#define BASIS_SYS_SYNC_OBSERVER_HPP_

#include <basis/sys/sync.hpp>

namespace sync {

	class MessageManager {
	public:
		virtual ~MessageManager() = default;

		virtual void register_observer(Observable* subject, Observer* observer) = 0;

		virtual void unregister_observer(Observable* subject, Observer* observer) = 0;

		virtual void unregister_all(Observable* subject) = 0;

		virtual void unregister_all(Observer* observer) = 0;

		virtual void notify(const Observable* subject, const Message& event) const = 0;
	};

	MessageManager* get_simple_message_manager();

	///==================================================================================== Observer
	class Observer {
	public:
		virtual ~Observer();

		virtual void notify(const Message& event) = 0;

		Observer();

		Observer(MessageManager* manager);

	private:
		MessageManager* m_manager;
	};

	///================================================================================== Observable
	class Observable {
	public:
		virtual ~Observable();

		Observable();

		Observable(MessageManager* manager);

		void register_observer(Observer* observer);

		void unregister_observer(Observer* observer);

		void notify_all(const Message& event) const;

		void set_changed(bool changed) const;

		bool get_changed() const;

	private:
		MessageManager* m_manager;
		mutable bool m_changed;
	};

}

#endif
