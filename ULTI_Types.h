#ifndef ULTI_TYPES
#define ULTI_TYPES
#include <string>
#include <map>
#include <vector>
namespace ulti{

//similar definitions are also include in stdint.h

typedef unsigned char byte;
typedef unsigned char ui8;
typedef unsigned short ui16;
typedef unsigned int ui32;
typedef unsigned long long ui64;

struct GenericType{
    std::string name;
    std::string type;
    std::string source;
    std::vector<ui8> data;
    long ordinal;
    long siz;
    bool ownedByParent;//Memory Management Convenience
    GenericType* parent;
    std::vector<GenericType*> children;
    std::map<std::string,std::string> extra;
    GenericType& operator=(const GenericType& ityp);
    GenericType(const GenericType& ityp);
    GenericType();
    ~GenericType();//Deletes All Children Owned By This (recursive!)
};

}

template<typename T>
void setchnk(T* ptr, int range, T val){
    for (int a = 0; a < range; a++){
        ptr[a]=val;
    }
}

#endif // ULTI_TYPES
