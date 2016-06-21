/**
    Extremely simple text system for generic graphics, very low overhead and poor graphics, but will allow
    you to have some text on generic surface pixel buffers

    further character sets can be defined, everything uses the interpolation system found in the graphics
    header, and a list of vertices (points)

    to use currently create a new charset object and setfont(), the use writemsg to apply text to a surface
**/

#ifndef ULTI_TEXT_H
#define ULTI_TEXT_H

#include "ULTI_Graphics.h"
#include "ULTI_Types.h"
#include "ULTI_Structs.h"
#include <string>
#include <vector>
#include <sstream>
namespace ulti{

class charset{
protected:
    std::vector<double> pairs[255];
    std::vector<surface> chars;
    int fontsize;
    ui32 color;
    ui32 bcolor;
    int thick;
    int tabspacenum;
public:
    icoord getdem(std::string& str);
    void setfont(int size, ui32 icolor, ui32 backcolor, int thickness);
    void writemsg(std::string msg,int tx, int ty, surface& sd, int hspacing = 3, int vspacing = 9, bool wordwrap = false);
    charset();
};

char shiftval(char ch);
int pairsurface(surface& sfc, std::vector<double>& pairs, ui32 color = 0xff000000, ui32 backcolor=0x00000000, int thick = 1);

template<typename T>
std::string tostring(T thing){
    std::stringstream ss;
    ss << thing;
    return ss.str();
}
bool strcasecmp(char* one, char* two);

}
#endif
