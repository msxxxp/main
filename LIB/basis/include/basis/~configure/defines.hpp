#ifndef BASIS_CONFIGURE_DEFINES_HPP_
#define BASIS_CONFIGURE_DEFINES_HPP_

#ifndef MEMORY_LOGGING
#   ifdef NDEBUG
#       define MEMORY_LOGGING 0
#   else
#       define MEMORY_LOGGING 2
#   endif
#endif

#endif
