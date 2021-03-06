﻿#ifndef BASIS_CONFIGURE_OBLIGATORY_TYPES_HPP_
#define BASIS_CONFIGURE_OBLIGATORY_TYPES_HPP_

#include <stdint.h>

typedef const void * PCVOID;

template<unsigned long N>
struct bin {
	enum {
		value = (N % 10) + 2 * bin<N / 10>::value
	};
};

template<>
struct bin<0> {
	enum {
		value = 0
	};
};

#endif
