#include "ULTI_Random.h"

namespace ulti{

ui32 lrn;
ui32 lpn;
ui32 lcnt;

ui32 lowrand(int low, int high){
    ui32 rn = (time(0)+(lrn+(seqrand()))) % (high-low) + low;
    lrn=rn;
    return rn;
}

ui32 seqrand(){
    int ret;
    lpn+=lcnt/2;
    ret=lpn^(lcnt);
    lcnt++;
    ret=ret<<lcnt%32;
    return ret;
}

void seedrnd(ui32 in){
    lpn=in;
}

bool chance(int percent){
    int tmp=seqrand()+rand();
    tmp %= 100; tmp = 100-tmp;
    return tmp<percent;
}

int rndrange(int low, int high){
    return (rand() % (high-low+1)+low);
}
}
