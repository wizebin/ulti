#include "ULTI_File.h"

#undef _WIN32_WINNT
#define _WIN32_WINNT 501

namespace ulti{

long lastFileError=0;


std::vector<ufile> dir(const std::string& ifile){
    std::vector<ufile> ret;
    std::string folder = ifile;
    if (folder == ""){
        char cCurrentPath[MAX_PATH];
        _getcwd(cCurrentPath,sizeof(cCurrentPath));
        folder = cCurrentPath;
    }

    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind;
    DWORD Attributes;
    std::string fld;
    char tmp = folder.back();
    if (folder.back()!='*'){
        if (folder.back()!='\\' && folder.back()!='/'){
            folder += '\\';
        }
        fld += folder + "*.*";
    }
    else
        fld=folder;

    hFind = FindFirstFileA(fld.c_str(), &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE){
        lastFileError = (long int)hFind;
        return ret;
    }
    do{
		std::string tmpName = FindFileData.cFileName;

        if (tmpName != "." && tmpName != "..")
        {

            ufile tfile;
            tfile.name=folder;
            tfile.name += tmpName;

            Attributes = GetFileAttributesA(tfile.name.c_str());
            if (Attributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                tfile.type=filetype::folder;
            }
            else
            {
                tfile.type=filetype::file;
            }

            ret.push_back(tfile);
        }
    }while(FindNextFileA(hFind, &FindFileData));
    FindClose(hFind);

    return ret;
}
int mkdir(const std::string& folder, bool recursive){
#if defined(_WIN32)||defined(_WIN_32)

    int res = CreateDirectoryA(folder.c_str(),NULL);
    if (res==0){
        if (GetLastError()==ERROR_PATH_NOT_FOUND && recursive){

            std::string parfold = parentFolder(folder).name;
            if (parfold.size()<folder.size()){
                mkdir(parfold,recursive);
                return mkdir(folder,false);//kinda silly, but should work just fine.
            }
        }
        else if (GetLastError()==ERROR_ALREADY_EXISTS){
            return 0;
        }

        return -1;
    }

    HANDLE hDir = CreateFileA(folder.c_str(),READ_CONTROL|WRITE_DAC,0,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
    if(hDir == INVALID_HANDLE_VALUE)
    return FALSE;

    ACL* pOldDACL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    GetSecurityInfo(hDir, SE_FILE_OBJECT , DACL_SECURITY_INFORMATION,NULL, NULL, &pOldDACL, NULL, &pSD);

    PSID pSid = NULL;
    SID_IDENTIFIER_AUTHORITY authNt = SECURITY_NT_AUTHORITY;
    AllocateAndInitializeSid(&authNt,2,SECURITY_BUILTIN_DOMAIN_RID,DOMAIN_ALIAS_RID_USERS,0,0,0,0,0,0,&pSid);

    EXPLICIT_ACCESS ea={0};
    ea.grfAccessMode = GRANT_ACCESS;
    ea.grfAccessPermissions = GENERIC_ALL;
    ea.grfInheritance = CONTAINER_INHERIT_ACE|OBJECT_INHERIT_ACE;
    ea.Trustee.TrusteeType = TRUSTEE_IS_GROUP;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.ptstrName = (LPTSTR)pSid;

    ACL* pNewDACL = 0;
    DWORD err = SetEntriesInAcl(1,&ea,pOldDACL,&pNewDACL);

    if(pNewDACL)
    SetSecurityInfo(hDir,SE_FILE_OBJECT,DACL_SECURITY_INFORMATION,NULL, NULL, pNewDACL, NULL);

    FreeSid(pSid);
    LocalFree(pNewDACL);
    LocalFree(pSD);
    LocalFree(pOldDACL);
    CloseHandle(hDir);

    return 1;

#else
    return mkdir(folder.c_str());
#endif // _WIN32
}
ufile parentFolder(const std::string& folder){
    ufile ret;
    for(int a = folder.size()-2; a>=0; a--){ //-2 because the last character could be a /
        if (folder[a]=='/'||folder[a]=='\\'){
            ret.name=folder.substr(0,a);
            break;
        }
    }
    //if (ret.name==""){
    //    ret.name=folder;
    //}
    return ret;
}
bool fileExists(const std::string& folder){
    #if defined(_WIN32)||defined(_WIN_32)
    DWORD dwAttrib = GetFileAttributesA(folder.c_str());
    return (dwAttrib != INVALID_FILE_ATTRIBUTES);
    #else
    std::fstream file(folder,std::ios::in);
    if (file.good()){
        file.close();
        return true;
    }
    return false;
    #endif // _WIN32
}

ui32 getDrives(){
    #if defined(_WIN32)||defined(_WIN_32)

    return GetLogicalDrives();

    #endif // _WIN_32
}

bool usingDrive(char letter){
    ui32 drives = getDrives();
    int tnum = toupper(letter)-'A';
    return drives & (1 << tnum);
}

unsigned int getDriveType(char letter){
    char ray[3];ray[0]=letter;ray[1]=':';ray[2]=NULL;
    return ::GetDriveTypeA(ray);
}
std::string getDriveTypeName(char letter){
    unsigned int res = getDriveType(letter);
    static std::string ray[] = {"Unknown","Invalid","Removable","Fixed","Remote","CD","Ramdisk"};
    if (res < 7){
        return ray[res];
    }
    return "Invalid";
}

std::vector<ufile> dir(const ufile& folder){
    return dir(folder.name);
}
ufile parentFolder(const ufile& folder){
    return parentFolder(folder.name);
}
bool fileExists(const ufile& folder){
    return fileExists(folder.name);
}


ufile ufile::parent(){
    return parentFolder(*this);
}
std::vector<ufile> ufile::children(){
    return dir(*this);
}

ufile::ufile(const std::string& str){
    this->name=str;
    this->type=filetype::unknown;
}
ufile::ufile(){
    this->type=filetype::unknown;
}
ufile::ufile(const std::string& str, filetype itype){
    this->name=str;
    this->type=itype;
}
ufile::ufile(const ufile& ifile){
    this->name=ifile.name;
    this->type=ifile.type;
}

flock::flock(const _FILE_INFO_3& finfo):path((wchar_t*)finfo.fi3_pathname),username((wchar_t*)finfo.fi3_username){
    id=finfo.fi3_id;
    permissions=finfo.fi3_permissions;
    locks=finfo.fi3_num_locks;
}
flock::flock(const flock& finfo){
    this->path=finfo.path;
    this->username=finfo.username;
    this->id=finfo.id;
    this->permissions=finfo.permissions;
    this->locks=finfo.locks;
}
flock::flock(){
    this->id=0;
    this->permissions=0;
    this->locks=0;
}

size_t getfsize(std::fstream & file){
    file.seekg(0, std::ios::end);
    size_t size = (size_t)file.tellg();
    file.seekg(0);
    return size;
}
size_t getfsize(const std::string & filename){
    std::fstream file(filename.c_str(),std::ios::in);
    if (file.bad())
        return 0;
    size_t ret = getfsize(file);
    file.close();
    return ret;
}

std::string loadfile(const std::string& filename){
    std::string ret;
    loadfile(filename,ret);
	return ret;
}
int loadfile(const std::string& filename, std::vector<unsigned char>& vec){
    std::fstream file(filename.c_str(),std::ios::in|std::ios::binary);
    if (file.is_open()){
        int sz = getfsize(file);
        vec.resize(sz);
        file.read((char*)&vec[0],sz);
        file.close();
        return 1;
    }
    return 0;
}
int loadfile(const std::string&filename, std::string& str){
    std::fstream file(filename.c_str(),std::ios::in|std::ios::binary);
    if (file.is_open()){
        int sz = getfsize(file);
        str.resize(sz);
        file.read(&str[0],sz);
        file.close();
        return 1;
    }
    return 0;
}

int savefile(const std::string& filename,const std::string& content, bool createDir){
    if (createDir){
        ufile tfile = parentFolder(filename);
        mkdir(tfile.name);
    }
    std::fstream file(filename.c_str(),std::ios::out|std::ios::binary);
    if (!file.good())
        return -1;
    file << content;
    file.close();
    return 1;
}
int savefile(const std::string& filename,const char* content, int size, bool createDir){
    if (createDir){
        ufile tfile = parentFolder(filename);
        mkdir(tfile.name);
    }
    std::fstream file(filename.c_str(),std::ios::out|std::ios::binary);
    if (!file.good())
        return -1;
    file.write(content,size);
    int ret = file.good();
    file.close();
    return ret;
}
int appendfile(const std::string& filename, const std::string& content, bool createDir){
    if (createDir){
        ufile tfile = parentFolder(filename);
        mkdir(tfile.name);
    }
    std::fstream file(filename.c_str(),std::ios::out|std::ios::binary|std::ios::app);
    if (!file.good())
        return -1;
    file << content;
    file.close();
    return 1;
}

#if defined(IID_IPersistFile)
#if (defined(_WIN32)||defined(_WIN_32))

HRESULT CreateLink(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc, LPCSTR wrkdir)
{
    HRESULT hres;
    IShellLinkA* psl;

    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres))
    {
        IPersistFile* ppf;

        // Set the path to the shortcut target and add the description.
        psl->SetPath(lpszPathObj);
        psl->SetDescription(lpszDesc);
        psl->SetWorkingDirectory(wrkdir);

        // Query IShellLink for the IPersistFile interface, used for saving the
        // shortcut in persistent storage.
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

        if (SUCCEEDED(hres))
        {
            WCHAR wsz[MAX_PATH];

            // Ensure that the string is Unicode.
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

            // Add code here to check return value from MultiByteWideChar
            // for success.

            // Save the link by calling IPersistFile::Save.
            hres = ppf->Save(wsz, TRUE);
            ppf->Release();
        }
        psl->Release();
    }
    return hres;
}
#endif //_WIN_32


int createLink(const std::string& name, const std::string& linkname, const std::string& linkdesc){
    CoInitialize(NULL);
    //std::wstring wfn = s2ws(filename);
    //std::wstring wsn = s2ws(shortcutname);
    //std::wstring wsc = s2ws(shortcutdescr);
    //return (int) CreateLink(wfn.c_str(),wsn.c_str(),wsc.c_str());
    std::string wd = parentFolder(name).name;
    if (wd==name || wd.size()==1){
        wd = "";
    }
    int ret = CreateLink(name.c_str(),linkname.c_str(),linkdesc.c_str(),wd.c_str());
    CoUninitialize();
    return ret;
}
#endif // defined(IID_IPersistFile)
//format
//(")key("):(")value(")

std::map<std::string,setting> loadSettings(const std::string& filename){
    std::string content = loadfile(filename);
    bool inquote=false;
    bool ignorenext = false;
    std::string key,val,curstr;
    std::map<std::string,setting> ret;
    for(unsigned int a = 0; a < content.size(); a++){
        if (content[a]=='\\' && !ignorenext){//ignore next char
            ignorenext=true;
        }
        else{
            if (ignorenext){
                curstr += content[a];
                ignorenext=false;
            }
            else{

                if (content[a]=='"'){
                    inquote=!inquote;
                }
                else if (inquote){
                    curstr += content[a];
                }
                else if (isspace(content[a])){

                }
                else if (content[a]==':'){
                    key=curstr;
                    curstr="";
                }
                else if (content[a]==';'){
                    val=curstr;
                    curstr="";
                    ret[key].sval=val;
                }

            }
        }
    }
    return ret;
}
int saveSettings(const std::string& filename, const std::map<std::string,setting>& settings){
    std::fstream file(filename,std::ios::out|std::ios::binary);
    if (!file.good())
        return -1;
    for (auto it=settings.begin(); it!=settings.end(); ++it)
        file << "\"" <<  quickEscape(it->first) << "\":\"" << quickEscape(it->second.sval) << "\";\n";
    return 1;
}



std::string getFileName(const std::string& path){
    for(int a = path.size(); a>=0; a--){
        if (path[a]=='/' || path[a]=='\\'){
            return path.substr(a+1,path.size()-a-1);
        }
    }
    return path;
}
std::string getFileExt(const std::string& path){
    for(int a = path.size(); a>=0; a--){
        if (path[a]=='.'){
            return path.substr(a+1,path.size()-a-1);
        }
    }
    return "";
}

std::string pullLine(const std::string str, int line){
    bool curnew = false;int cnstart=-1;int curline = 0;int startpos=0;
    char lastchar = NULL; //may want to initialize to a different value using a different container size
    for(unsigned int a = 0; a < str.size(); a++){
        if (str[a]=='\r' || str[a]=='\n'){
            if (lastchar == '\r' && str[a]=='\n'){
                //ignore this one
            }
            else{
                if (curline == line){
                    return str.substr(startpos,a-startpos);
                }
                curline++;
            }
            startpos = a + 1;
        }
        else if (a==str.size()-1){
            if (curline == line){
                return str.substr(startpos,str.size()-startpos+1);
            }
        }
        lastchar = str[a];
    }

    return "";
}
std::vector<std::string> getLines(const std::string& str){
    std::vector<std::string> ret;
    bool curnew = false;int cnstart=-1;int curline = 0;int startpos=0;
    char lastchar = NULL; //may want to initialize to a different value using a different container size
    for(unsigned int a = 0; a < str.size(); a++){
        if (str[a]=='\r' || str[a]=='\n'){
            if (lastchar == '\r' && str[a]=='\n'){
                //ignore this one
            }
            else{
                ret.push_back(str.substr(startpos,a-startpos));
                curline++;
            }
            startpos = a + 1;
        }
        else if (a==str.size()-1){

            ret.push_back(str.substr(startpos,str.size()-startpos+1));

        }
        lastchar = str[a];
    }

    return ret;
}
std::vector<std::wstring> getuLines(const std::wstring& str){
    std::vector<std::wstring> ret;
    bool curnew = false;int cnstart=-1;int curline = 0;int startpos=0;
    wchar_t lastchar = NULL; //may want to initialize to a different value using a different container size
    for(unsigned int a = 0; a < str.size(); a++){
        if (str[a]=='\r' || str[a]=='\n'){
            if (lastchar == '\r' && str[a]=='\n'){
                //ignore this one
            }
            else{
                ret.push_back(str.substr(startpos,a-startpos));
                curline++;
            }
            startpos = a + 1;
        }
        else if (a==str.size()-1){
            ret.push_back(str.substr(startpos,str.size()-startpos+1));
        }
        lastchar = str[a];
    }

    return ret;
}
int contains(const std::string& pat, const std::string& str, bool nocase){
    if (pat == str){
        return 1;
    }
    if (nocase){
        for(unsigned int a = 0,cur=0;a < str.size(); a++){
            if (toupper(pat[cur])==toupper(str[a])){
                cur++;
                if (cur == pat.size()){
                    return a;
                }
            }
            else{
                cur=0;
            }
        }
    }
    else{
        for(unsigned int a = 0,cur=0;a < str.size(); a++){
            if (pat[cur]==str[a]){
                cur++;
                if (cur == pat.size()){
                    return a;
                }
            }
            else{
                cur=0;
            }
        }
    }
    return 0;
}
std::vector<ufile> findFile(const std::string& folder, const std::string& tofind, int searchLevels, int currentRecursiveLevel){
    std::vector<ufile> curlist = dir(folder);
    std::vector<ufile> ret;
    for(unsigned int a = 0; a < curlist.size(); a++){
        std::string tttttmp = curlist[a].name;
        if (curlist[a].type==filetype::folder){
            if (currentRecursiveLevel<searchLevels){
                std::vector<ufile> recursed = findFile(curlist[a].name,tofind,searchLevels,currentRecursiveLevel+1);
                ret.insert(ret.end(),recursed.begin(),recursed.end());
            }
        }
        else{
            if (contains(strtolower(tofind),strtolower(curlist[a].name))>0){
                ret.push_back(curlist[a]);
            }
        }
    }
    return ret;
}

std::wstring loadufile(const std::string& filename){
    std::string utf8line = loadfile(filename);
    std::wstring utf16line;

    std::string::iterator end_it = utf8::find_invalid(utf8line.begin(), utf8line.end());
    if (end_it != utf8line.end()) {
        //WE HAVE AN ENCODING ISSUE
    }

    utf8::utf8to16(utf8line.begin(), end_it, back_inserter(utf16line));
    return utf16line;
}
int saveufile(const std::string& filename, const std::wstring& content){
    std::string utf8line;
    utf8::utf16to8(content.begin(), content.end(), back_inserter(utf8line));
    return savefile(filename,utf8line);
}
int copyFile(const std::string& name, const std::string& ren){
    #if defined(_WIN32)||defined(_WIN_32)
        ulti::mkdir(ulti::parentFolder(ren).name,true);
        return CopyFileA(name.c_str(),ren.c_str(),false);
    #endif
    std::ifstream  src(name, std::ios::binary);
    std::ofstream  dst(ren,   std::ios::binary);
    int ret = -1;
    if (src.is_open()&&dst.is_open()){
        dst << src.rdbuf();
        ret = src.good() && dst.good();
    }
    src.close();
    dst.close();
    return ret;
}
int copyFilePortion(const std::string& original, const std::string& destination, long pos, long size, long bufsize){
    ulti::mkdir(ulti::parentFolder(destination).name,true);
    std::ofstream dst(destination.c_str(),std::ios::out|std::ios::binary);
    if (!dst.good())
        return -1;
    int part = 0;
    std::string buf = ulti::readfilepart(original,pos+((part)*bufsize),bufsize);
    while(buf!=""){
        dst << buf;
        buf = ulti::readfilepart(original,pos+((++part)*bufsize),bufsize);
    }
    return 1;
}
int rename(const std::string& name, const std::string& ren){
    #if defined(_WIN32)||defined(_WIN_32)
        return MoveFileExA(name.c_str(),ren.c_str(),MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
    #endif
    return std::rename(name.c_str(),ren.c_str());
}
int moveFile(const std::string& name, const std::string& ren){
    ulti::mkdir(ulti::parentFolder(ren).name,true);
    return rename(name,ren);
}
std::string cwd(){
    char cCurrentPath[MAX_PATH];
    _getcwd(cCurrentPath,sizeof(cCurrentPath));
    std::string ret = cCurrentPath;
    return ret;
}
std::string getcwd(){
    char* buffer;
    if((buffer = _getcwd( NULL, 0 )) != NULL){
        std::string ret = buffer;
        free(buffer);
        return ret;
    }
    return "";

}

#include <time.h>

void UnixTimeToFileTime(time_t t, LPFILETIME pft){
    LONGLONG ll;

    ll = Int32x32To64(t, 10000000) + 116444736000000000;
    pft->dwLowDateTime = (DWORD)ll;
    pft->dwHighDateTime = ll >> 32;
}

void UnixTimeToSystemTime(time_t t, LPSYSTEMTIME pst){
    FILETIME ft;

    UnixTimeToFileTime(t, &ft);
    FileTimeToSystemTime(&ft, pst);
}

long long SystemTimeToUnixTime(FILETIME ft)
{
    // takes the last modified date
    LARGE_INTEGER date, adjust;
    date.HighPart = ft.dwHighDateTime;
    date.LowPart = ft.dwLowDateTime;
    // 100-nanoseconds = milliseconds * 10000
    adjust.QuadPart = 11644473600000 * 10000;
    // removes the diff between 1970 and 1601
    date.QuadPart -= adjust.QuadPart;
    // converts back from 100-nanoseconds to seconds
    return date.QuadPart / 10000000;

}

int setFileTimes(const std::string& filename, unsigned long modified, unsigned long created, unsigned long accessed){
    // Create a FILETIME struct and convert our new SYSTEMTIME
    // over to the FILETIME struct for use in SetFileTime below
    FILETIME modtime,createtime,accesstime;

    UnixTimeToFileTime(created,&createtime);
    UnixTimeToFileTime(accessed,&accesstime);

    // Get a handle to our file and with file_write_attributes access
    HANDLE fhnd = CreateFileA(filename.c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    // Set the file time on the file
    int ret = SetFileTime(fhnd,&createtime,&accesstime,&modtime);

    // Close our handle.
    CloseHandle(fhnd);
	return ret;
}
int setFileModified(const std::string& filename, time_t modified){
    FILETIME modtime;
    UnixTimeToFileTime(modified,&modtime);
    HANDLE fhnd = CreateFileA(filename.c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    int ret = SetFileTime(fhnd,NULL,NULL,&modtime);
    CloseHandle(fhnd);
	return ret;
}
int setFileCreated(const std::string& filename, time_t created){
    FILETIME createtime;
    UnixTimeToFileTime(created,&createtime);
    HANDLE fhnd = CreateFileA(filename.c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    int ret = SetFileTime(fhnd,&createtime,NULL,NULL);
    CloseHandle(fhnd);
	return ret;
}
int setFileAccessed(const std::string& filename, time_t accessed){
    FILETIME accesstime;
    UnixTimeToFileTime(accessed,&accesstime);
    HANDLE fhnd = CreateFileA(filename.c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    int ret = SetFileTime(fhnd,NULL,&accesstime,NULL);
    CloseHandle(fhnd);
	return ret;
}
time_t getFileModified(const std::string& filename){
    FILETIME modtime;
    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if(hFile != INVALID_HANDLE_VALUE){GetFileTime(hFile,NULL,NULL,&modtime);}
    else{return 0;}
    CloseHandle(hFile);
    return SystemTimeToUnixTime(modtime);
}
time_t getFileCreated(const std::string& filename){
    FILETIME createtime;
    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if(hFile != INVALID_HANDLE_VALUE){GetFileTime(hFile,&createtime,NULL,NULL);}
    else{return 0;}
    CloseHandle(hFile);
    return SystemTimeToUnixTime(createtime);
}
time_t getFileAccessed(const std::string& filename){
    FILETIME accesstime;
    HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if(hFile != INVALID_HANDLE_VALUE){GetFileTime(hFile,NULL,&accesstime,NULL);}
    else{return 0;}
    CloseHandle(hFile);
    return SystemTimeToUnixTime(accesstime);
}

int deleteFile(const std::string& name){
#if defined(_WIN32)||defined(_WIN_32)
    return DeleteFileA(name.c_str());
#endif // _WIN32
}

/*std::string DisplayErrorText(DWORD dwLastError){
    std::string ret;
    HMODULE hModule = NULL; // default to system source
    LPSTR MessageBuffer;
    DWORD dwBufferLength;
    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM ;

    if(dwLastError >= 2100 && dwLastError <= 2100+899) {
        hModule = LoadLibraryEx(
            TEXT("netmsg.dll"),
            NULL,
            LOAD_LIBRARY_AS_DATAFILE
            );

        if(hModule != NULL)
            dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
    }

    if(dwBufferLength = FormatMessageA(
        dwFormatFlags,
        hModule, // module to get message from (NULL == system)
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPSTR) &MessageBuffer,
        0,
        NULL
        )){

        DWORD dwBytesWritten;
        ret.append(MessageBuffer,dwBufferLength);
        LocalFree(MessageBuffer);
    }
    if(hModule != NULL)
        FreeLibrary(hModule);
    return ret;
}*/

#if defined(ULTI_DONETWORK) && defined(ULTI_DOSHARED)

std::vector<flock> getSharedFiles(const std::wstring& path){
    FILE_INFO_3* pFiles = NULL;
    DWORD nRead = 0, nTotal = 0;

    std::vector<flock> ret;
    std::vector<long> ids;

    int res = ERROR_MORE_DATA;

    std::vector<wchar_t> writable(path.begin(), path.end());
    writable.push_back('\0');

    int totalDone = 0;

    while(res==ERROR_MORE_DATA){

        try{
        res = NetFileEnum(
          NULL, // servername, NULL means localhost
          &writable[0], // basepath, directory where VB6 program is
          NULL, // username, searches for all users
          3, // level, we just need resource ID
          (BYTE**)&pFiles, // bufptr,
          MAX_PREFERRED_LENGTH, // prefmaxlen, collect as much as possible
          &nRead, // entriesread, number of entries stored in pFiles
          &nTotal, // totalentries, ignore this
          NULL //resume_handle, ignore this
        );

        }
        catch(int exx){
            printf("%i",exx);
            break;
        }

        for (unsigned int i=0; i < nRead; ++i){
            //int nret = NetFileClose(NULL, pFiles[i].fi2_id);
            flock tmp(pFiles[i]);
            ret.push_back(tmp);

        }

        NetApiBufferFree(pFiles);
    }

    return ret;
}

int closeFileLock(const flock& lock){
    return NetFileClose(NULL, lock.id);
}

int closeSharedFiles(const std::wstring& path){
    std::vector<flock> shares = getSharedFiles(path);
    int ret=0;
    for(unsigned int a = 0;a < shares.size(); a++){
        int res = closeFileLock(shares[a]);
        if (res==0)ret++;
    }
    return ret;
}
int closeFilesMatching(const std::wstring& name, const std::wstring& path){
    std::vector<flock> locks = getSharedFiles(path);
    int close=0;
    for(unsigned int a = 0; a < locks.size(); a++){
        if (locks[a].path.find(name)!=std::wstring::npos){
            closeFileLock(locks[a]);
            close++;
        }
    }
    return close;
}
std::vector<flock> findFilesMatching(const std::wstring& name,const std::wstring& path){
    std::vector<flock> locks = getSharedFiles(path);
    std::vector<flock> ret;
    int close=0;
    for(unsigned int a = 0; a < locks.size(); a++){
        if (locks[a].path.find(name)!=std::wstring::npos){
            ret.push_back(locks[a]);
        }
    }
    return ret;
}
#endif // defined(ULTI_DONETWORK) && defined(ULTI_DOSHARED)

int start(const std::string& filename, const std::string& param, const std::string& directory){
#if defined(_WIN32)||defined(_WIN_32)
    if (directory.size()>0){
        return (ShellExecuteA(NULL, NULL, filename.c_str(), param.c_str(), directory.c_str(), SW_SHOWDEFAULT)!=NULL);
    }
    else{
        return (ShellExecuteA(NULL, NULL, filename.c_str(), param.c_str(), NULL, SW_SHOWDEFAULT)!=NULL);
    }

#endif // _WIN32
	return 0;
}
HANDLE startChild(const std::string& filename, const std::string& param, const std::string& directory){
    SHELLEXECUTEINFOA info;
    info.cbSize=sizeof(info);
    info.fMask        = SEE_MASK_NOCLOSEPROCESS;
    info.hwnd         = 0;
    info.lpFile       = filename.c_str();  // Application name
    info.lpVerb       = "open";
    info.lpParameters = param.c_str();           // Additional parameters
    if (directory.size()>0){
        info.lpDirectory  = directory.c_str();                           // Default directory
    }
    info.nShow        = SW_SHOWDEFAULT;
    info.hInstApp     = 0;
    bool res = ShellExecuteExA(&info);
    if (res){
        return info.hProcess;
    }
    return NULL;
}
int backupfile(const std::string& filename, const std::string& backupname, int renamelevels){
    std::fstream file(filename.c_str(),std::ios::in|std::ios::binary);
    if (!file.is_open()){
        return -1;
    }
    ulti::mkdir(ulti::parentFolder(backupname).name,true);
    std::fstream fback;
    bool g2g = 0;
    int tries = 0;
    if (renamelevels>1){
        for(int a = 0; a < renamelevels; a++){
            std::stringstream ss(backupname);
            ss << backupname << ".back(" << a << ")";
            fback.open(ss.str().c_str(),std::ios::in|std::ios::binary);
            if (!fback.is_open()){
                fback.close();
                fback.open(ss.str().c_str(),std::ios::out|std::ios::binary);
                if (!fback.is_open()){
                    continue;
                }
                fback << file.rdbuf();
                std::stringstream ss2(backupname);
                ss2 << backupname << ".back(" << (a+1)%renamelevels << ")";
                remove(ss2.str().c_str());
                g2g=1;
                break;
            }
            else{
                fback.close();
                if (a == renamelevels-1 && tries == 0){
                    std::stringstream ss2(backupname);
                    ss2 << backupname << ".back(" << 0 << ")";
                    remove(ss2.str().c_str());
                    tries++;
                    a=-1;
                    continue;
                }
            }
        }
    }
    else{
        std::stringstream ss(backupname);
        ss << backupname << ".backup";
        fback.open(ss.str().c_str(),std::ios::out|std::ios::binary);
        if (fback.is_open()){
            fback << file.rdbuf();
            g2g=1;
        }
    }
    file.close();
    fback.close();
    if (!g2g){
        return -2;
    }
    return 1;
}

std::string getExePath(){
    #if defined(_WIN32)||defined(_WIN_32)
    char tpath[MAX_PATH];
    GetModuleFileNameA(NULL,tpath,MAX_PATH);
    std::string ret = tpath;
    return ret;
    #else
    return "";
    #endif // _WIN32
}

std::string getDriveLetter(const std::string& location, bool defaultToCWD){
    std::string tloc = location;
    long coloc = tloc.find(":");
    if (coloc==std::string::npos&&defaultToCWD){
        tloc = getcwd();
        long coloc = tloc.find(":");
    }
    if (coloc!=std::string::npos){
        for(int a = coloc; a >= 0; a--){
            if (isalpha(tloc[a])){
                return tloc.substr(a,1);
            }
        }
    }
    return "";
}
#define INFO_BUFFER_SIZE 32767
char  infoBuf[INFO_BUFFER_SIZE];
DWORD inoutsize;
std::string getSystemPath(){
    CHAR pth[MAX_PATH];
    HRESULT result = SHGetFolderPathA(NULL, CSIDL_SYSTEMX86, NULL, 0, pth);

    if (result != S_OK)
        return "";
    std::string ret = pth;
    ret+='\\';
    return ret;
}
std::string getRootPath(const std::string& path){
    CHAR pth[MAX_PATH];
    long len  = 0;
    BOOL res = 0;//GetVolumePathNameA(path.c_str(),pth,len);
    if (!res)
        return "";
    std::string ret(pth,len);
    ret.push_back('\\');
    return ret;
}
std::string getWindowsPath(){
    if( !GetWindowsDirectoryA( infoBuf, INFO_BUFFER_SIZE ) ){
        return "";
    }
    std::string ret = infoBuf;
    ret += "\\";
    return ret;
}
std::string getAppDataPath(){
    CHAR pth[MAX_PATH];
    HRESULT result = SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, pth);

    if (result != S_OK)
        return "";
    std::string ret = pth;
    ret+='\\';
    return ret;
}
std::string getDocumentsPath(){
    CHAR pth[MAX_PATH];
    HRESULT result = SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, 0, pth);

    if (result != S_OK)
        return "";
    std::string ret = pth;
    ret+='\\';
    return ret;
}
std::string getCSIDLPath(long csidl){
    CHAR pth[MAX_PATH];
    HRESULT result = SHGetFolderPathA(NULL, csidl, NULL, 0, pth);

    if (result != S_OK)
        return "";
    std::string ret = pth;
    ret+='\\';
    return ret;
}
std::string getDesktopPath(){
    CHAR pth[MAX_PATH];
    HRESULT result = SHGetFolderPathA(NULL, CSIDL_DESKTOP, NULL, 0, pth);

    if (result != S_OK)
        return "";
    std::string ret = pth;
    ret+='\\';
    return ret;
}
std::string getUsername(){
    inoutsize=INFO_BUFFER_SIZE;
    if( !GetUserNameA( infoBuf, &inoutsize ) ){
        return "";
    }
    std::string ret = infoBuf;
    return ret;
}
std::string getComputer(){
    inoutsize=INFO_BUFFER_SIZE;
    if( !GetComputerNameA( infoBuf, &inoutsize ) ){
        return "";
    }
    std::string ret = infoBuf;
    return ret;
}
std::string readfilepart(const std::string& filename, long pos, long size){
    std::fstream file(filename.c_str(),std::ios::in|std::ios::binary);
    long dif = getfsize(filename) - pos;

    if (dif<0)
        return "";
    if (dif<size)
        size=dif;

    file.seekg(pos);
    file.seekp(pos);
    std::string buff;buff.resize(size);
    file.read(&buff[0],size);
    file.close();
    return buff;
}

std::string absolutePath(const std::string& path){
    #if defined(_WIN32)||defined(_WIN_32)
        std::string buf;buf.resize(MAX_PATH);
        int res = GetFullPathNameA(path.c_str(),MAX_PATH,&buf[0],NULL); //SHOULD NOT BE USED IN A MULTITHREADED APP...
        if (res==0)
            return "";//error
        buf.resize(res);
        return buf;
    #else
        char buf[MAX_PATH];
        realpath(path.c_str(),buf);
        string ret = buf;
        return ret;
    #endif // _WIN_32
}

long findInFile(const std::string& filename, const std::string& tofind, long startpos, long sectionsize){
    int part = 0;
    long fpos = startpos;
    std::string dat = ulti::readfilepart(filename,startpos+(part*sectionsize),sectionsize);
    int cur = 0;
    bool found = false;
    long ret = -1;
    while(dat!=""){
        for(int a = 0; a < dat.size(); a++){
            if (dat[a]==tofind[cur]){
                cur++;
                if (cur==tofind.size()){//tofind.size()-1?
                    found = true;
                    ret = startpos+((part)*sectionsize)+a-(tofind.size()-1);
                }
            }
            else{
                cur=0;
                startpos=-1;
            }
        }
        if (ret==-1){
            dat = ulti::readfilepart(filename,startpos+((++part)*sectionsize),sectionsize);
        }
        else
            break;
    }
    return ret;
}

};
