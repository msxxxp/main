#ifndef BASIS_CONFIGURE_PRINTF_TRACE_HPP_
#define BASIS_CONFIGURE_PRINTF_TRACE_HPP_

//#include <stdio.h>

//#define TraceFunc() printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__)
//#define TraceFuncFormat(format, ...) printf(format, ##__VA_ARGS__)

#define TraceFunc()
#define TraceFuncFormat(format, ...)

#endif
