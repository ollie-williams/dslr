#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>

#define __MACOS__
#include "EDSDK/Header/EDSDK.h"

#define WHERESTR  "[file %s, line %d]: "
#define WHEREARG  __FILE__, __LINE__
#define DEBUGPRINT2(...)       fprintf(stderr, __VA_ARGS__)
#define DEBUGPRINT(_fmt, ...)  DEBUGPRINT2(WHERESTR _fmt, WHEREARG, __VA_ARGS__)
#define DEBUGSTREAM std::cerr << __FILE__ << " (" << __LINE__ << ") "
