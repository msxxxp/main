#ifndef _LIBBASE_MESSAGEMANAGER_PVT_HPP_
#define _LIBBASE_MESSAGEMANAGER_PVT_HPP_

namespace sync {

	struct Message;
	class Observer;
	class Observable;

	class MessageManager {
	public:
		virtual ~MessageManager();

		virtual void register_observer(Observable * subject, Observer * observer) = 0;

		virtual void unregister_observer(Observable * subject, Observer * observer) = 0;

		virtual void unregister_all(Observable * subject) = 0;

		virtual void unregister_all(Observer * observer) = 0;

		virtual void notify(const Observable * subject, const Message & event) const = 0;
	};

}

#endif