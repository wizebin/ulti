#include "ULTI_Pack.h"

namespace ulti{

packer::packer(){
    filebegin=0;
    packing=0;
    initsize=0;
}

int packer::openfile(const string& infilename, bool forpack){
    if (!file.is_open()){
        packing = forpack;
        file.open(infilename.c_str(), (forpack? (ios::out|ios::app) : ios::in )|ios::binary);
        if (file.is_open()){
            //If we're unpacking, verify there is actually data packed
            initsize = filesize(file);
            if (!forpack){
                if (!detectpack()){
                    file.close();
                    return -2;
                }
                filebegin = unpackheaderpos();
            }
            return 1;
        }
        return -1;
    }
    return 0;
}
bool packer::isopenreadable(){
    return (file.is_open() && (!packing));
}
bool packer::isopenwritable(){
    return (file.is_open() && (packing));
}
int packer::beginpack(){
    if (!isopenwritable())return -1;

    file.seekp(0,ios::end);
    file << createbeginpackstr();

    return 1;
}
int packer::addpack(const string& data){
    if (!isopenwritable())return -1;
    file << anum(data.size()) << data;
}
int packer::streampack(const string& ifilename){
    if (!isopenwritable())return -1;
    fstream ifile(ifilename.c_str(),ios::in|ios::binary);
    if (!ifile.is_open()){
        return -1;
    }
    ui32 ifilesize = filesize(ifile);
    if (ifilesize > 0){
        file << anum(ifilesize);
        ///FOR NOW SLOW BYTE BY BYTE, LATER CHUNKS
        char buf = ifile.get();
        while(!ifile.eof()){
                file.put(buf);
                buf=ifile.get();
        }
    }
    ifile.close();
    return 1;
}
int packer::endpack(){
    if (!isopenwritable())return -1;
    file << anum(initsize) << createendpackstr();
    return 1;
}
int packer::clearpack(){
    //WINDOWS : setendoffile
    //LINUX : truncate
	return -1;
}
bool packer::detectpack(){
    if (!isopenreadable())
        return false;
    if (initsize < BEGINPACKSTRSIZE + FOOTERSIZE)
        return false;

    ///Search for beginstring
    int endpos = searchfile(file,createendpackstr());
    if (endpos==-1)
        return false;
    file.seekg(endpos-sizeof(unsigned int));
    char dt[sizeof(unsigned int)];
    file.read(dt,sizeof(unsigned int));
    unsigned int begpos = nnum<unsigned int>(dt,0);
    file.seekg(begpos);
    return (nextxbytes(file,BEGINPACKSTRSIZE) == createbeginpackstr());
}
int packer::unpackcount(){
    if (!isopenreadable())return -1;
    unsigned int cur=0,old=0;
    int ret = 0;
    for (int a = 0; a < MAXPACK; a++){
        cur = unpackbegin(a);
        if (cur > old)
            ret++;
        else break;
        old = cur;
    }
    return ret-1;

}
unsigned int packer::unpackheaderpos(){
    if (!isopenreadable())return -1;

    ///Search For Beginpack

    return searchfile(file,createbeginpackstr()); //This will search for the beginning string, returning -1 on fail

    file.seekg(0,ios::end);
	ui32 cpos = file.tellg();
    file.seekg(cpos-FOOTERSIZE); //the 8 dashes and a ui32
    char dt[4];
    file.read(dt,4);
    return nnum<ui64>(dt,0);
}
unsigned int packer::unpackbegin(int packindex){
    if (!isopenreadable())return -1;
    if (packindex == -1)
        return filebegin;

    unsigned int cur = (unsigned int)filebegin; ///MAY OVERFLOW ON BIGGER THAN 4 GB U HAV RAM?!
    cur += BEGINPACKSTRSIZE;
    char sz[4];
    string ret;
    for (int a = 0; a < packindex; a++){
        file.seekg(cur);
        if (cur+4+PACKHEADERSIZE+FOOTERSIZE > initsize)
            return 0; ///I DON'T LIKE THIS, Better way of return error with unsigned int without throw?
        file.read(sz,4);
        cur +=  nnum<ui32>(sz,0)+sizeof(ui32);
    }
    return cur;
}
unsigned int packer::unpacksize(int packindex){
    if (!isopenreadable())return -1;
    unsigned int start = unpackbegin(packindex);
    if (start == 0)
        return 0;

    file.seekg(start);
    char sz[sizeof(ui32)];
    file.read(sz,sizeof(ui32));
    return nnum<ui32>(sz,0);
}
string packer::unpack(int packindex){
    if (!isopenreadable())return "";

    unsigned int cur = unpackbegin(packindex);
    if (cur == 0) //Can't be 0 with a header.
        return "";
    char sz[4];
    string ret;
    file.seekg(cur);
    file.read(sz,4);
    unsigned int fieldsize= nnum<ui32>(sz,0);
    for (int a = 0; a < fieldsize; a++){ ///Currently not checking for EOF, assuming things were detected before
        ret += file.get();
    }

    return ret;

}
int packer::unpackstream(int packindex, const string& filename){
    if (!isopenreadable())return -1;

    unsigned int cur = unpackbegin(packindex);
    if (cur == 0) //Can't be 0 with a header.
        return -2;
    fstream ofile(filename.c_str(),ios::out|ios::binary);
    if (!ofile.is_open())
        return -1;
    char sz[4];
    string ret;
    file.seekg(cur);
    file.read(sz,4);
    unsigned int fieldsize= nnum<ui32>(sz,0);
    for (int a = 0; a < fieldsize; a++){ ///Currently not checking for EOF, assuming things were detected before
        ofile.put( file.get());
    }
    ofile.close();
    return 1;

}
int packer::closefile(){
    file.close();
    filename="";
    filebegin=initsize=0;
    return 1;
}

string getsys32(){

    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    int ret = GetSystemDirectory( infoBuf, INFO_BUFFER_SIZE );
    std::string thing;
    for (int a = 0; a < ret; a++){
        thing+=infoBuf[a];
    }
    return thing;
}


unsigned long long filesize(const string& filename){
    fstream file(filename.c_str(),ios::in|ios::binary);
    file.seekg(0,ios::end);
    unsigned long long ret = file.tellg();
    file.close();
    return ret;
}
unsigned long long filesize(fstream & file){
    std::streampos cur = file.tellg();
    file.seekg(0,ios::end);
    unsigned long long ret = file.tellg();
    file.seekg(cur);
    return ret;
}
string nextxbytes(fstream& file, int bytenum){

    string ret = "";
    for (int a = 0; a < bytenum&&!file.eof(); a++){
        ret+=file.get();
    }
    return ret;
}

int readzip(const string& filename, string& filedata){
    gzFile infile = gzopen(filename.c_str(), "rb");
    if (!infile) return -1;
    char buffer[128];
    int num_read=0;
    while((num_read = gzread(infile,buffer,sizeof(buffer))) > 0){
        filedata.append(buffer,num_read);
    }
    gzclose(infile);
    return 1;
}
int writezip(const string& data, const string& filename){
    gzFile outfile = gzopen(filename.c_str(), "wb");
    if (!outfile) return -1;
    int ret = gzwrite(outfile, data.c_str(), data.size());
    gzclose(outfile);
    return ret;

}

int zipdata(const string& data, string& zipdata){
    unsigned long int destsize = data.size();
    zipdata.resize(destsize);
    int ret = compress2((Bytef*)&zipdata[0],&destsize,(Bytef*)&data[0],data.size(),9);
    zipdata.resize((unsigned long int)destsize);
	return ret;
}
int unzipdata(const string& zipdata, string& data, unsigned long int sizehint){
    data.resize(sizehint); unsigned long int destsize = zipdata.size()*4; //*4 = 75% compression
    if (sizehint>zipdata.size())
        destsize=sizehint;
    data.resize(destsize);
    int ret=Z_BUF_ERROR;
    while(ret==Z_BUF_ERROR){
        ret = uncompress((Bytef*)&data[0],&destsize,(Bytef*)&zipdata[0],zipdata.size());
        if (ret==Z_BUF_ERROR){
            destsize+=zipdata.size();
            data.resize(destsize);
        }
    }
    data.resize(destsize);
    if (ret!=Z_OK)
        return ret;
    return 1;
}
int writefile(const string& filename, string& data){
    fstream file(filename.c_str(),ios::out|ios::binary);
    if (!file.is_open())
        return -1;
    file.write(data.c_str(),data.size());
    return 1;
}
int readfile(const string& filename, vector<char>& data){
    fstream file(filename.c_str(),ios::in|ios::binary);
    if (!file.is_open())
        return -1;
    char buf = file.get();
    while(!file.eof()){
        data.push_back(buf);
        buf=file.get();
    }
    return 1;
}
int readfile(const string& filename, string& str){
    fstream file(filename.c_str(),ios::in|ios::binary);
    if (!file.is_open())
        return -1;
    char buf = file.get();
    while(!file.eof()){
        str+=buf;
        buf=file.get();
    }
    return 1;
}

string createbeginpackstr(){
    string ret;
    for(int a = 0; a < BEGINPACKSTR.size(); a++){
        ret += PREPACKCHR;
        ret += BEGINPACKSTR[a];
        ret += POSTPACKCHR;
    }
    return ret;
}
string createendpackstr(){
    string ret;
    for(int a = 0; a < ENDPACKSTR.size(); a++){
        ret += PREPACKCHR;
        ret += ENDPACKSTR[a];
        ret += POSTPACKCHR;
    }
    return ret;
}

int searchfile(fstream& file, const string& tofind){
    if (tofind.size()==0)
        return -1; ///Should return something else, but what? Can "" match anything in a file??
    file.seekp(0,std::ios::beg);
    int fsize = filesize(file);
    char buf=file.get();int curmatch = 0;
    while(!file.eof()){
        if (tofind[curmatch]==buf){
            curmatch++;
            if (curmatch>2)
            if (curmatch==tofind.size()){
				unsigned long long curpos = file.tellp();
                return curpos-tofind.size();
            }
        }
        else
            curmatch=0;
        buf = file.get();
    }
    return -1;
}

}
