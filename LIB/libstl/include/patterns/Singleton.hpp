#ifndef PATTERNS_SINGLETON_HPP_
#define PATTERNS_SINGLETON_HPP_

namespace Pattern {

	template <typename T>
	struct Singleton : private T {
		typedef T implementation_type;
		typedef Singleton<T> this_type;
		static T & instance() {
			static this_type instance;
			return instance;
		}

	private:
		~Singleton() {
		}

		Singleton() {
		}
	};

}

#endif
