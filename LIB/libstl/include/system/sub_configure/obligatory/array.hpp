#ifndef LIBSTL_SYSTEM_CONFIGURE_OBLIGATORY_ARRAY_HPP_
#define LIBSTL_SYSTEM_CONFIGURE_OBLIGATORY_ARRAY_HPP_

template<typename Type, size_t N>
size_t lengthof(Type (&)[N])
{
	return N;
}

#endif
