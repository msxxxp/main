#ifndef BASIS_SYS_SYNC_OBSERVER_HPP_
#define BASIS_SYS_SYNC_OBSERVER_HPP_

#include <basis/sys/sync.hpp>

namespace sync {

	class MessageManager {
	public:
		static MessageManager* get_default();

		virtual ~MessageManager() = default;

		virtual void register_observer(const Observable* subject, Observer* observer) = 0;

		virtual void unregister_observer(const Observable* subject, const Observer* observer) = 0;

		virtual void unregister_all(const Observable* subject) = 0;

		virtual void unregister_all(const Observer* observer) = 0;

		virtual void notify(const Observable* subject, const Message& event) const = 0;
	};

	///================================================================================== Observable
	class Observable {
	public:
		virtual ~Observable();

		Observable();

		void register_observer(Observer* observer) const;

		void unregister_observer(Observer* observer) const;

		void notify_observers(const Message& event) const;

	protected:
		void set_changed(bool changed) const;

		bool get_changed() const;

	private:
		mutable MessageManager* m_manager;
		mutable bool m_changed;
	};

	///==================================================================================== Observer
	class Observer {
	public:
		virtual ~Observer();

		virtual void notify(const Message& event) = 0;

		Observer();

	private:
		MessageManager* m_manager;
	};

}

#endif
