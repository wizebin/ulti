#ifndef ULTI_STRUCTS_H
#define ULTI_STRUCTS_H

#include "ULTI_Types.h"
#include "ULTI_Utility.h"
#include <sstream>
#include <functional>

namespace ulti{
struct icoord;

struct coord{
  double x,y,z;
  const coord& operator =(const coord& toset);
  bool operator ==(const coord& tocheck);
  bool operator !=(const coord& tocheck);
  coord();
  coord(double tx, double ty, double tz = 0);
  coord(icoord coor);
};

struct icoord{
  int x,y,z;
  const icoord& operator =(const icoord& toset);
  bool operator ==(const icoord& tocheck);
  bool operator !=(const icoord& tocheck);
  icoord();
  icoord(int tx, int ty, int tz = 0);
  icoord(coord cor);
};

struct dcoord{
    coord pos;
    coord vel;
    const dcoord& operator =(const dcoord& toset);
    bool operator ==(const dcoord& tocheck);
    bool operator !=(const dcoord& tocheck);
    dcoord();
    dcoord(double ix, double iy, double vx, double vy);
    dcoord(const dcoord& dc);
};

enum valuetype{
    notype,

    stringtype,
    inttype,
    doubletype,
    longtype,
    longlongtype,
    shorttype,
    chartype,
    booltype,

    bytetype,
    ushorttype,
    uinttype,
    ulongtype,
    ulonglongtype,

    maptype,
    arraytype,
    xmltype,//uses value as tag name, children as children, and ray as properties
};

/**
    Generic mapped tuple-like struct. Cannot hold arbitrary classes, only int/string/double types
    this type is pretty well fleshed out and can do things like this:

    element a = 5;
    a = "hello";
    a["john"] = 44;
    a[12] = {55,"hi",12,"invictus"};

    a.setStr("{'Hello':4}");

    int num = a["hello"].getT<int>();

    some things may be changed, especially overloading the () operator for basic return types so the
    getT<type>() won't be necessary
**/

struct element{
    std::map<std::string,element> children;
    std::vector<element> ray;
    int valtype;
    std::string value;
    std::wstring wvalue;
    ui64 ulval;
    void* extra;

    template<typename T>
    T getT(){
        return *(T*)(&ulval);
    }
    template<typename T>
    T safeGetT(){
        if (sizeof(ulval)>sizeof(T))
            return getT<T>();
        return T();
    }
    template<typename T>
    void setT(const T& in){
        (*(T*)(&ulval))=in;
    }
    template<typename T>
    bool safeSetT(const T& in){
        if (sizeof(ulval)>sizeof(T)){
            setT<T>(in);
            return true;
        }
        return false;
    }

    std::string getStr() const; //json format is default
    std::string getJson() const;
    element& setStr(const std::string& setter); //json format
    element& setXml(const std::string& setter);
    std::string getXml() const;

    element();
    element(valuetype typ);
    element(const std::map<std::string,element>& val);
    element(const std::string& val);
    element(int val);
    element(short val);
    element(long val);
    element(long long val);
    element(double val);
    element(char val);
    element(ui64 val);
    element(unsigned long val);
    element(ui32 val);
    element(ui16 val);
    element(ui8 val);
    element(const char* val);
    element(const std::vector<element>& val);
    element(std::initializer_list<element> c);

    element& operator=(const element& in);
    element& operator=(const std::string& in);
    element& operator=(const int& in);
    element& operator=(const short& in);
    element& operator=(const long& in);
    element& operator=(const long long& in);
    element& operator=(const double& in);
    element& operator=(const char& in);
    element& operator=(const ui64& in);
    element& operator=(const unsigned long& in);
    element& operator=(const ui32& in);
    element& operator=(const ui16& in);
    element& operator=(const ui8& in);
    element& operator=(const std::map<std::string,element>& in);
    element& operator=(const char* in);
    element& operator=(const std::vector<element>& in);
    element& operator=(std::initializer_list<element> in);

    bool operator==(const element& in)const;
    bool operator==(const std::string& in)const;
    bool operator==(const int& in);
    bool operator==(const short& in);
    bool operator==(const long& in);
    bool operator==(const long long& in);
    bool operator==(const double& in);
    bool operator==(const char& in);
    bool operator==(const ui64& in);
    bool operator==(const unsigned long& in);
    bool operator==(const ui32& in);
    bool operator==(const ui16& in);
    bool operator==(const ui8& in);
    bool operator==(const bool& in);
    bool operator==(const std::map<std::string,element>& in);
    bool operator==(const char* in);
    bool operator==(const std::vector<element>& in);
    //bool operator==(std::initializer_list<element> in);

    bool operator!=(const element& in)const;
    bool operator!=(const std::string& in)const;
    bool operator!=(const int& in);
    bool operator!=(const short& in);
    bool operator!=(const long& in);
    bool operator!=(const long long& in);
    bool operator!=(const double& in);
    bool operator!=(const char& in);
    bool operator!=(const ui64& in);
    bool operator!=(const unsigned long& in);
    bool operator!=(const ui32& in);
    bool operator!=(const ui16& in);
    bool operator!=(const ui8& in);
    bool operator!=(const bool& in);
    bool operator!=(const std::map<std::string,element>& in);
    bool operator!=(const char* in);
    bool operator!=(const std::vector<element>& in);

    friend std::ostream& operator<<(std::ostream& os, const element& dt);
    friend std::istream& operator>>(std::ostream& os, element& dt);

    element& operator << (const ulti::element& prop);

    element& operator[] (const int dex);
    element& operator[] (const std::string& dex);
    element& back();
    element& front();
    void push_back(const element& in);
    void pop_back();
    bool erase(const int dex);
    bool erase(const std::string& dex);
    bool checkKey(const std::string& key)const;

    void clear();

    bool ismap()const;
    bool isray()const;

    long size()const;
    valuetype getType()const;
    void setType(valuetype);

    element keys();

    void* getExtra();
    void setExtra(void* ex);
    virtual ~element();

};

//simply lays out a json element in readable terms
std::string stringlayout(ulti::element& el, int depth = 0);

}

#endif
