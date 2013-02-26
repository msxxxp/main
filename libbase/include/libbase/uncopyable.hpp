#ifndef _LIBBASE_UNCOPYABLE_HPP_
#define _LIBBASE_UNCOPYABLE_HPP_

namespace Base {

	class Uncopyable {
		typedef Uncopyable this_type;

	protected:
		~Uncopyable()
		{
		}

		Uncopyable()
		{
		}

	private:
#ifdef _MSC_VER
		Uncopyable(const this_type &);
#else
		Uncopyable(const this_type &) = delete;
#endif

#ifdef _MSC_VER
		this_type & operator = (const this_type &);
#else
		this_type & operator = (const this_type &) = delete;
#endif
	};

}

#endif
