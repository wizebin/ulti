#ifndef ULTI_REGISTRY_H
#define ULTI_REGISTRY_H

#include <cassert>

#ifndef KEY_WOW64_64KEY
#define KEY_WOW64_64KEY 0x0100
#endif // KEY_WOW64_64KEY
#ifndef KEY_WOW64_32KEY
#define KEY_WOW64_32KEY 0x0200
#endif // KEY_WOW64_32KEY

#include <windows.h>

#include <sddl.h>
#include <lm.h>

#include <string>
#include <vector>
#include <map>
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif


using std::map;
using std::string;
using std::vector;

extern int gval;
/*
LONG WINAPI RegOpenKeyEx(
  _In_      HKEY hKey,
  _In_opt_  LPCTSTR lpSubKey,
  _In_      DWORD ulOptions,
  _In_      REGSAM samDesired,
  _Out_     PHKEY phkResult
);

LONG WINAPI RegQueryValueEx(
  _In_         HKEY hKey,
  _In_opt_     LPCTSTR lpValueName,
  _Reserved_   LPDWORD lpReserved,
  _Out_opt_    LPDWORD lpType,
  _Out_opt_    LPBYTE lpData,
  _Inout_opt_  LPDWORD lpcbData
);

HKEY_CLASSES_ROOT
HKEY_CURRENT_CONFIG
HKEY_CURRENT_USER
HKEY_LOCAL_MACHINE
HKEY_USERS
potential values for hkey


KEY_ALL_ACCESS (0xF003F)
Combines the STANDARD_RIGHTS_REQUIRED, KEY_QUERY_VALUE, KEY_SET_VALUE, KEY_CREATE_SUB_KEY, KEY_ENUMERATE_SUB_KEYS, KEY_NOTIFY, and KEY_CREATE_LINK access rights.
KEY_CREATE_LINK (0x0020)
Reserved for system use.
KEY_CREATE_SUB_KEY (0x0004)
Required to create a subkey of a registry key.
KEY_ENUMERATE_SUB_KEYS (0x0008)
Required to enumerate the subkeys of a registry key.
KEY_EXECUTE (0x20019)
Equivalent to KEY_READ.
KEY_NOTIFY (0x0010)
Required to request change notifications for a registry key or for subkeys of a registry key.
KEY_QUERY_VALUE (0x0001)
Required to query the values of a registry key.
KEY_READ (0x20019)
Combines the STANDARD_RIGHTS_READ, KEY_QUERY_VALUE, KEY_ENUMERATE_SUB_KEYS, and KEY_NOTIFY values.
KEY_SET_VALUE (0x0002)
Required to create, delete, or set a registry value.
KEY_WOW64_32KEY (0x0200)
Indicates that an application on 64-bit Windows should operate on the 32-bit registry view. This flag is ignored by 32-bit Windows. For more information, see Accessing an Alternate Registry View.
This flag must be combined using the OR operator with the other flags in this table that either query or access registry values.
Windows 2000:  This flag is not supported.
KEY_WOW64_64KEY (0x0100)
Indicates that an application on 64-bit Windows should operate on the 64-bit registry view. This flag is ignored by 32-bit Windows. For more information, see Accessing an Alternate Registry View.
This flag must be combined using the OR operator with the other flags in this table that either query or access registry values.
Windows 2000:  This flag is not supported.
KEY_WRITE (0x20006)
Combines the STANDARD_RIGHTS_WRITE, KEY_SET_VALUE, and KEY_CREATE_SUB_KEY access rights.



REG_BINARY
Binary data in any form.
REG_DWORD
A 32-bit number.
REG_DWORD_LITTLE_ENDIAN
A 32-bit number in little-endian format.
Windows is designed to run on little-endian computer architectures. Therefore, this value is defined as REG_DWORD in the Windows header files.
REG_DWORD_BIG_ENDIAN
A 32-bit number in big-endian format.
Some UNIX systems support big-endian architectures.
REG_EXPAND_SZ
A null-terminated string that contains unexpanded references to environment variables (for example, "%PATH%"). It will be a Unicode or ANSI string depending on whether you use the Unicode or ANSI functions. To expand the environment variable references, use the ExpandEnvironmentStrings function.
REG_LINK
A null-terminated Unicode string that contains the target path of a symbolic link that was created by calling the RegCreateKeyEx function with REG_OPTION_CREATE_LINK.
REG_MULTI_SZ
A sequence of null-terminated strings, terminated by an empty string (\0).
The following is an example:
String1\0String2\0String3\0LastString\0\0
The first \0 terminates the first string, the second to the last \0 terminates the last string, and the final \0 terminates the sequence. Note that the final terminator must be factored into the length of the string.
REG_NONE
No defined value type.
REG_QWORD
A 64-bit number.
REG_QWORD_LITTLE_ENDIAN
A 64-bit number in little-endian format.
Windows is designed to run on little-endian computer architectures. Therefore, this value is defined as REG_QWORD in the Windows header files.
REG_SZ
A null-terminated string. This will be either a Unicode or an ANSI string, depending on whether you use the Unicode or ANSI functions.
*/
namespace ulti{

#define sdregpath "Software\\VB and VBA Program Settings\\ServiceDesk\\"
#define trustregpath "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Domains\\"
#define trustregpathip "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Ranges\\"

enum regtype{
    rfolder,
    key,
    none
};

struct reg{
    string name;
    string val;
    string location;
    regtype type; // 0=folder
};
//Read a registry key string value
string sreadreg(const string& regfolder, const string& key, HKEY base = HKEY_CURRENT_USER);

//Write a registry key string value
int swritereg(const string& regfolder, const string& key, const string& value, bool createifnotthere = true, HKEY base = HKEY_CURRENT_USER);

//Write a registry key double word value
int swriteregdword(const string& regfolder, const string& key, unsigned long value, bool createifnotthere = true, HKEY base = HKEY_CURRENT_USER);

//Return a list of the contents in a registry folder
vector<reg> dirreg(const string& regfolder, HKEY base = HKEY_CURRENT_USER);

//Delete a registry key
int sdelreg(const string& regfolder, const string& key = "", HKEY base = HKEY_CURRENT_USER);

//Delete a registry key value
int sdelregval(const string& regfolder, const string& key, HKEY base = HKEY_CURRENT_USER);

//Does this registry key exist?
bool checkreg(const string& regfolder, const string& key, HKEY base = HKEY_CURRENT_USER);

//Read a registry in to a generic registry type
ulti::reg readreg(const string& regfolder, const string& key, HKEY base = HKEY_CURRENT_USER);

//Create a registry key, handled by writereg normally, unusual to use this
int screatereg(const string& regfolder,const string& key="", const string& value = "", HKEY base = HKEY_CURRENT_USER);


//DEPRACATED
int savemap(const std::map<string,string>& tosave, const string& regfolder);

//DEPRACTED
int loadmap(std::map<string,string>& tomake, const string& regfolder);

//Simple user structure to hold the SID which will allow you to perform many essential functions in windows
//Specifically to do with account previledges
struct user{
    std::string name;//usri4_name
    std::string sidstring;
    bool enabled; // !usri4_flags&UF_ACCOUNTDISABLE
    user();
    user(const user& usr);
    user& operator=(const user& usr);
};

//Returns a list of users and SIDs
vector<user> getLocalUsers();

};


#endif
