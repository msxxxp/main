#ifndef BASIS_CONFIGURE_OBLIGATORY_ARRAY_HPP_
#define BASIS_CONFIGURE_OBLIGATORY_ARRAY_HPP_

template<typename Type, size_t N>
size_t lengthof(Type (&)[N])
{
	return N;
}

#endif
