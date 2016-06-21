#include "ULTI_State.h"

namespace ulti{

const options& options::operator =(const options& inopt){

    return *this;
}

const phystate& phystate::operator =(const phystate& inphys){
    scrn=inphys.scrn;
    return *this;
}

const state& state::operator =(const state& inst){
    ps = inst.ps;
    return *this;
}

state::state(){

}

}
