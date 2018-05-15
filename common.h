#ifndef __Common__
#define __Common__

#include <iostream>

//#define DEBUG

#ifdef DEBUG
#define dout std::cout
#else
#define dout 0 && std::cout
#endif

#endif // __Common__
