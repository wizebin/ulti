#ifndef ULTI_RANDOM
#define ULTI_RANDOM

#include "ULTI_Types.h"
#include <time.h>
#include <cstdlib>

/**
    This component is currently garbage
**/

namespace ulti{

extern ui32 lrn;
extern ui32 lpn;
extern ui32 lcnt;
//ui32 lpn;
//ui32 lcnt = 0;
void seedrnd(ui32 in);
ui32 seqrand();
//ui32 lrn;
ui32 lowrand(int low, int high);
bool chance(int percent);

int rndrange(int low, int high);
}

#endif
