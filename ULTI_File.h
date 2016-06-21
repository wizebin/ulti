#ifndef ULTI_FILE_H
#define ULTI_FILE_H

#include <vector>
#include <string>
#include <fstream>
#include <direct.h>
#include <map>
#include <algorithm>
#include <cstdio>
#include <sstream>

#include "ULTI_Utility.h"
#include "ULTI_Types.h"

#if defined(_WIN32)||defined(_WIN_32)
///If you're on windows, you need to link to several libs:
//(advapi32) && (mswsock) && (winmm) && (ws2_32) && (wsock32)

//***Windows
#define WIN_32_LEAN_AND_MEAN
#include <windows.h>
#include <lm.h>
#include <accctrl.h>
#include <aclapi.h>

#include <Shlobj.h>
#include <objidl.h>
//end Windows***

#else

//***Posix compliant
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
//end Posix***

#endif

#include "utf8.h"

namespace ulti{

extern long lastFileError;

enum settingType{
    tstring,
    tlong,
    tbool,
    tdouble
};

struct setting{
    std::string name;
    std::string sval;
    long ival;
    bool bval;
    double dval;

    settingType type;
};

enum filetype{
    file,
    folder,
    shortcut,
    unknown
};

struct ufile{
    filetype type;
    std::string name;
    ufile parent();
    std::vector<ufile> children();

    ufile(const std::string& str);
    ufile();
    ufile(const std::string& str, filetype itype);
    ufile(const ufile&);
};

struct flock{
    long id;
    long permissions;
    long locks;
    std::wstring path;
    std::wstring username;

    flock(const _FILE_INFO_3& finfo);
    flock(const flock& finfo);
    flock();
};

std::vector<ufile> dir(const std::string& folder);
std::vector<ufile> dir(const ufile& folder);
int mkdir(const std::string& folder, bool recursive = true);
ufile parentFolder(const std::string& folder);
ufile parentFolder(const ufile& ifile);

std::string absolutePath(const std::string& path);


bool fileExists(const std::string& folder);
bool fileExists(const ufile& ifile);

size_t getfsize(std::fstream & file);
size_t getfsize(const std::string & filename);
std::string cwd();

std::string loadfile(const std::string& filename);
int loadfile(const std::string&filename, std::vector<unsigned char>& vec);
int loadfile(const std::string&filename, std::string& str);
std::string readfilepart(const std::string& filename, long pos, long size);
int savefile(const std::string& filename,const std::string& content, bool createDir = true);
int savefile(const std::string& filename,const char* content, int size, bool createDir = true);
int appendfile(const std::string& filename, const std::string& content, bool createDir = true);
std::wstring loadufile(const std::string& filename);
int saveufile(const std::string& filename, const std::wstring& content);

std::map<std::string,setting> loadSettings(const std::string& filename);
int saveSettings(const std::string& filename, const std::map<std::string,setting>& settings);

//This function should really be in Utility, but would break compatibility

template<typename T,typename R>
bool keyInMap(const std::map<T,R> mp, const T& key){
    return (mp.find(key) != mp.end());
}


//returns a file name and extension from a path
std::string getFileName(const std::string& path);

//returns just an extension from a path
std::string getFileExt(const std::string& path);

//iterates through the file to find the line in question, SLOW
std::string pullLine(const std::string str, int line);

std::vector<std::string> getLines(const std::string& str);
std::vector<std::wstring> getuLines(const std::wstring& str);

//searches recursively to find a file, no infinite recursion available, MAX_INT current equivalent
std::vector<ufile> findFile(const std::string& folder, const std::string& tofind, int searchLevels = 2, int currentRecursiveLevel = 0);

//finds pat in str, if equal result is 1, if str contains pat the index of the return is returned, poor choice
//looking back
int contains(const std::string& pat, const std::string& str, bool nocase = true);

int rename(const std::string& name, const std::string& ren);
int copyFile(const std::string& name, const std::string& ren);
int copyFilePortion(const std::string& original, const std::string& destination, long pos, long size = 0, long bufsize = 1024);
int moveFile(const std::string& name, const std::string& ren);
int deleteFile(const std::string& name);

//make shortcut
int createLink(const std::string& name, const std::string& linkname, const std::string& linkdesc);


int setFileTimes(const std::string& filename, unsigned long modified, unsigned long created, unsigned long accessed);
time_t getFileModified(const std::string& filename);
time_t getFileCreated(const std::string& filename);
time_t getFileAccessed(const std::string& filename);
int setFileModified(const std::string& filename, time_t modified);
int setFileCreated(const std::string& filename, time_t created);
int setFileAccessed(const std::string& filename, time_t accessed);

//cwd = current working directory
std::string getcwd();

//this requires the netapi32 library
std::vector<flock> getSharedFiles(const std::wstring& path);

int closeSharedFiles(const std::wstring& path);
int closeFileLock(const flock& lock);
int closeFilesMatching(const std::wstring& name, const std::wstring& path);
std::vector<flock> findFilesMatching(const std::wstring& name,const std::wstring& path);

//run a program
int start(const std::string& filename, const std::string& param = "", const std::string& directory = "");

//run a program as a dependent child, it will close when the parent closes
HANDLE startChild(const std::string& filename, const std::string& param = "", const std::string& directory = "");

int backupfile(const std::string& filename, const std::string& backupname, int renamelevels = 1);

//returns the absolute path to the current exe
std::string getExePath();

std::string getDriveLetter(const std::string& location, bool defaultToCWD = true);

std::string getCSIDLPath(long csidl);
std::string getRootPath(const std::string& path);
std::string getSystemPath();
std::string getWindowsPath();
std::string getAppDataPath();
std::string getDesktopPath();
std::string getUsername();
std::string getComputer();

//bitmasked & (1 << x) WON'T RETURN DISCONNECTED NETWORK DRIVES OR DRIVES MAPPED IN OPPOSITE ADMIN PROFILE (elevated vs not)
ui32 getDrives();

//checks if that drive letter is in use
bool usingDrive(char letter);

//Can be any of the following:
//DRIVE_UNKNOWN DRIVE_NO_ROOT_DIR DRIVE_REMOVABLE DRIVE_FIXED DRIVE_REMOTE DRIVE_CDROM DRIVE_RAMDISK
unsigned int getDriveType(char letter);

//returns the above type but a string value instead of the index
std::string getDriveTypeName(char letter);

//load and save the generic settings type, this is going to be replaced by ulti::element found in structs.h
template<typename T, typename R>
std::map<T,R> loadSettings(const std::string& filename){
    std::string content = loadfile(filename);
    std::map<T,R> ret = stringToMap<T,R>(content);
    return ret;
}
template<typename T, typename R>
int saveSettings(const std::string& filename, const std::map<T,R>& settings){
    std::fstream file(filename,std::ios::out|std::ios::binary);
    if (!file.good())
        return -1;
    file << mapToString<T,R>(settings);
    return 1;
}
long findInFile(const std::string& filename, const std::string& tofind, long startpos = 0, long sectionsize = 1024);

};


#endif
