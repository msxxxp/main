#ifndef PATTERNS_UNCOPYABLE_HPP_
#define PATTERNS_UNCOPYABLE_HPP_

namespace Pattern {
	class Uncopyable {
	protected:
		~Uncopyable() = default;

		Uncopyable() = default;

	private:
#ifdef _MSC_VER
		Uncopyable(const Uncopyable &);
#else
		Uncopyable(const Uncopyable &) = delete;
#endif

#ifdef _MSC_VER
		Uncopyable & operator = (const Uncopyable &);
#else
		Uncopyable & operator = (const Uncopyable &) = delete;
#endif
	};
}

#endif
