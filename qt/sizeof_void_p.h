#ifndef SIZEOF_VOID_P
#ifdef __SIZEOF_POINTER__
#define SIZEOF_VOID_P  __SIZEOF_POINTER__
#else
#include <giac/config.h>
#endif
#endif
