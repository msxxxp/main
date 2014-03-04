#ifndef LIBBASE_FWD_HPP_
#define LIBBASE_FWD_HPP_

#if defined(NoStlString) || defined(USE_LITE_STRING) || defined(NO_STD_STRING)
	#include <simstl/iosfwd>
#else

	#ifdef _MSC_VER
//		#include <string>
	#else
//		#include <iosfwd>
	#endif

#endif

#endif
