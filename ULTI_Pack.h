#ifndef ULTI_PACKER_H
#define ULTI_PACKER_H

#include <fstream>
#include <string>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <vector>
#include "zlib.h"
#include "ULTI_Types.h"

using std::vector;
using std::fstream;
using std::ios;
using std::string;

/**
    Pack format :

    Original File, BEGINPACKSTR<version unsigned byte> , instructionlist , unsigned int beginning ENDPACKSTR

**/

namespace ulti{

template <typename T>
T nnum(string& in, int pos){
    return *(T*)(&in[pos]);
}
template <typename T>
T nnum(char* str, int pos){
    return *(T*)(&str[pos]);
}

/**
    Return a new string from a random type. Requires the sizeof function to work.
**/

template <typename T>
string anum(T num){
    string tmp;
    for (int a = 0; a < sizeof(T); a++){
        tmp += ((unsigned char*)(&num))[a];
    }
    return tmp;
}

int readzip(const string& filename, string& filedata);
int writezip(const string& data, const string& filename);

int zipdata(const string& data, string& zipdata);
int unzipdata(const string& zipdata, string& data, unsigned long int sizehint = 0);

int writefile(const string& filename, string& data);
int readfile(const string& filename, vector<char>& data);
int readfile(const string& filename, string& data);

unsigned long long filesize(const string& filename);
unsigned long long filesize(fstream & file);
string nextxbytes(fstream& file, int bytenum);
string getsys32();


int searchfile(fstream& file, const string& tofind);
int searchfilefromback(fstream& file, const string& tofind);//return position of first char on full match or -1


#define INFO_BUFFER_SIZE 32767


//1 is the current pack version, for future flexibility
const string BEGINPACKSTR = "ULTIPACK" + anum<ui8>(1);
const string PREPACKCHR = "##_-{"; ///NECESSARY TO FLUFF BEGIN AND END PACK TO ELIMINATE DUPLICATES IN BYTECODE
const string POSTPACKCHR = "}-_##";
const string ENDPACKSTR = "ENDPACK";
const int BEGINPACKSTRSIZE = BEGINPACKSTR.size() + BEGINPACKSTR.size()*(PREPACKCHR.size()+POSTPACKCHR.size());
const int ENDPACKSTRSIZE = ENDPACKSTR.size() + ENDPACKSTR.size()*(PREPACKCHR.size()+POSTPACKCHR.size());
const int FOOTERSIZE = ENDPACKSTRSIZE+sizeof(unsigned int);
const int PACKHEADERSIZE = sizeof(unsigned int);
const int MAXPACK = 50000; //Max number of packed files? I've forgotten what this does 3 months later..

string createbeginpackstr();
string createendpackstr();

/**

    This class is designed to pack and unpack data from a file, useful for installers

    Int methods return -1 on failure.

**/

class packer{
protected:

    fstream file;

    string filename;

    unsigned int filebegin;
    unsigned int initsize;

    bool packing;

public:

    /*
        Open file, set filename, get filesize, forpack==true(ios::out) else ios::in
    */

    int openfile(const string& infilename, bool forpack);

    //verify in correct state
    bool isopenreadable();
    bool isopenwritable();

    //Write BEGINPACKSTR to file, 8*'-' + version (unsigned byte)
    int beginpack();

    //Write new data segment to the end of the file
    int addpack(const string& data);

    //Stream data, much easier on RAM
    int streampack(const string& filename);

    //Write ENDPACKSTR to file
    int endpack();

    //Remove Pack CLOSES FILE STREAM
    int clearpack();

    //Check pack for integrity, verify there's a begin and end and data *slow
    bool detectpack();

    //Get the number of packed segments
    int unpackcount();

    //Get start of pack (packed at end-16 to end-8)
    unsigned int unpackheaderpos();

    //Get start of packed data at packindex (root of unpacksize and unpack)
    unsigned int unpackbegin(int packindex);

    //Get size of packed data at packindex
    unsigned int unpacksize(int packindex);

    //Get packed data packed at packindex
    string unpack(int packindex);

    //Unpack data to file
    int unpackstream(int packindex, const string& filename);

    // :*
    int closefile();

    //Helper Functions
    unsigned intsize();

    packer();
};

//SetFileTime(filename, &thefiletime, (LPFILETIME) NULL,(LPFILETIME) NULL);//WINAPI to change file dates
/*
BOOL WINAPI SetFileTime(
  __in      HANDLE hFile,
  __in_opt  const FILETIME *lpCreationTime,
  __in_opt  const FILETIME *lpLastAccessTime,
  __in_opt  const FILETIME *lpLastWriteTime
);
*/
}


#endif
