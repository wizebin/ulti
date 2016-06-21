#ifndef ULTI_STATE
#define ULTI_STATE

#include "ULTI_Structs.h"
namespace ulti{

/**
Easy way to store and restore basic user options
**/
struct options{

    const options& operator =(const options& inopt);
};

/**
Meant to hold variables like screen width and height and other basics
**/
struct phystate{
    icoord scrn;
    const phystate& operator =(const phystate& inphys);
};

/**
Meant to hold the game's state. Defined by the game by overriding extrastate
which MUST override the tostring and fromstring methods. MUST *severe look.
**/
struct state{
    phystate ps;
    state();
    const state& operator =(const state& inst);
};

}
#endif
