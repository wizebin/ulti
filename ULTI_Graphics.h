#ifndef ULTI_GRAPHICS
#define ULTI_GRAPHICS

#include "ULTI_Types.h"
#include "ULTI_Object.h"
#include <cmath>

namespace ulti{

class surface;
//bool collide(surface& sa, surface& sb);

//generic pixel buffer type object, this is the base for all ulti::graphics
class surface{ //to apply to SDL use &surfacename[0] for the pointer
protected:
    ui32 dsize;
    ui16 sx,sy;
    ui32* px;
public:
    ui32 size()const;
    ui16 width()const;
    ui16 height()const;
    void clear(ui32 color=0xff444444);
    void drawline(int x1, int y1, int x2, int y2, ui32 color, int thick=1);
    void draw(surface& scr, int ix, int iy);
    void setpx(ui32* pixels, int ix, int iy, int sx, int sy);
    void drawrect(int x1, int y1, int x2, int y2, ui32 color);
    void resize(int tx, int ty); //expensive (relatively)
    surface(int tx, int ty);
    surface();
    ~surface();
    surface(const surface& in);
    surface& operator=(const surface& in);
    ui32& operator[](ui32 index);
    ui32& pixel(ui16 tx, ui16 ty);
    ui32 getPixel(ui16 tx, ui16 ty)const;
};

//small class for moving objects, this really is redundant and shouldn't be defined by the library,
//may be removed
class sprite: public object{
protected:
    surface sfc;
    coord pos;
    coord size; //not sure if coord or icoord
public:
    surface& getsfc();
    coord getpos();
    coord getsize();
    int width();
    int height();
    void setpos(coord ipos);
    void resize(coord isize);
    virtual void apply(surface& sur);
};

struct color{
	unsigned char r,g,b,a;
};

color getColor(ui32 val);
ui32 col(ui32 r, ui32 g, ui32 b, ui32 a = 255); //argb format (default)
ui32 colargb(ui32 r, ui32 g, ui32 b, ui32 a=255);
ui32 colrgba(ui32 r, ui32 g, ui32 b, ui32 a=255);

//invert color
ui32 invcol(ui32 colr);

//apply src to dest based on the alpha of src (not right yet)
ui32 trn(ui32 src, ui32 dest);

//interpolate on whole values from x1,y1 to x2,y2 and call fnc(curx,cury) for each intermediate value
void interpolate(int x1, int y1, int x2, int y2, void(*fnc)(int,int));

//check collision between two bodies
bool collide(float x1,float y1,float sx1,float sy1, float x2, float y2, float sx2, float sy2);

//Not My Code
int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);
surface loadPng(const std::string& filename);
int loadPng(const std::string& filename, surface& sur);
}
#endif
