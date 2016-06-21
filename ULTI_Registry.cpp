#include "ULTI_Registry.h"
#if (WINVER < 501)
WINADVAPI BOOL WINAPI ConvertSidToStringSidA(PSID Sid, LPSTR* StringSid);//REAAAAAAAALY STUPID, BUT ON THE DEFAULT MINGW WITH CODE::BLOCKS THE WINVER CANNOT BE SET TO 501
WINADVAPI BOOL WINAPI ConvertSidToStringSidW(PSID Sid, LPWSTR* StringSid);
WINADVAPI BOOL WINAPI ConvertStringSidToSidA(LPSTR StringSid, PSID *Sid);
WINADVAPI BOOL WINAPI ConvertStringSidToSidW(LPWSTR StringSid, PSID *Sid);
#endif
namespace ulti{
int gval = 0;
string sreadreg(const string& regfolder, const string& key, HKEY base){
    HKEY hkey;string str;DWORD siz = 0;
    int gval = RegOpenKeyExA(base,regfolder.c_str(),NULL,KEY_READ,&hkey);
    if (gval!=0){
        return str;
    }

    gval = RegQueryValueExA(hkey,key.c_str(),NULL,NULL,NULL,&siz); //SECOND PARAMETER MAY NOT BE NULL
    unsigned char* data = new unsigned char[siz];
    gval = RegQueryValueExA(hkey,key.c_str(),NULL,NULL,data,&siz);

    if (siz>0)
    str.append((char*)data,(int)siz-1);

    RegCloseKey(hkey);
    delete [] data;
    return str;
}
ulti::reg readreg(const string& regfolder, const string& key, HKEY base){
    HKEY hkey;string str;DWORD siz = 0;reg ret;ret.location=regfolder;ret.name=key;
    int gval = RegOpenKeyExA(base,regfolder.c_str(),NULL,KEY_READ,&hkey);
    if (gval!=0){
        //if (gval==ERROR_FILE_NOT_FOUND)
            ret.type=regtype::none;

        return ret;
    }

    gval = RegQueryValueExA(hkey,key.c_str(),NULL,NULL,NULL,&siz); //SECOND PARAMETER MAY NOT BE NULL
    if (gval!=ERROR_SUCCESS){
        //if (gval==ERROR_FILE_NOT_FOUND)
            ret.type=regtype::none;

        return ret;
    }
    unsigned char* data = new unsigned char[siz];
    gval = RegQueryValueExA(hkey,key.c_str(),NULL,NULL,data,&siz);

    if (siz>0)
    str.append((char*)data,(int)siz-1);

    RegCloseKey(hkey);
    delete [] data;

    ret.val=str;
    ret.type=regtype::key;
    return ret;
}

bool checkreg(const string& regfolder, const string& key, HKEY base){
    HKEY hkey;DWORD siz = 0;
    int gval = RegOpenKeyExA(base,regfolder.c_str(),NULL,KEY_READ,&hkey);
    RegCloseKey(hkey);
    return (gval==0);
}

vector<reg> dirreg(const string& regfolder, HKEY base){
    HKEY hkey; vector<reg> ret;
    gval = RegOpenKeyExA(base,regfolder.c_str(),NULL,KEY_ENUMERATE_SUB_KEYS|KEY_QUERY_VALUE|KEY_READ,&hkey);
    if (gval!=0){
        return ret;
    }
    unsigned long subkeys = 0, maxsubkeylength=0, subvalues= 0, maxvaluelen=0, maxvaluesize=0;

    gval = RegQueryInfoKey(hkey,NULL,NULL,NULL,&subkeys,&maxsubkeylength,NULL,&subvalues,&maxvaluelen,&maxvaluesize,NULL,NULL);
    if (gval!=0){
        return ret;
    }
    //cout << "\nSubkeys : " << subkeys << " mlen " << maxsubkeylength << " subvalues : " << subvalues << " mval " << maxvaluelen << " msiz " << maxvaluesize;
    int index=0; unsigned long siz=0; string buf;
    while(gval==0||gval==ERROR_MORE_DATA){
        siz=maxsubkeylength+1;
        buf.resize(siz);
        gval = RegEnumKeyExA(hkey,index,&buf[0],&siz,NULL,NULL,NULL,NULL);
        buf.resize(siz);
        if (gval==0){
            ret.push_back(reg());
            ret.back().name=buf;
            ret.back().type=regtype::rfolder;
            ret.back().location=regfolder;
            //cout << "\nAdding " << buf;
        }
        index++;
    }
    gval=0;
    index=0;
    while(gval==0||gval==ERROR_MORE_DATA){
        siz=maxvaluelen+1;
        buf.resize(siz);
        gval = RegEnumValueA(hkey,index,&buf[0],&siz,NULL,NULL,NULL,NULL);
        buf.resize(siz);
        if (gval==0){
            ret.push_back(reg());
            ret.back().name=buf;
            ret.back().type=regtype::key;
            ret.back().val=sreadreg(regfolder,buf);
            ret.back().location=regfolder;
        }
        index++;
    }
    if (gval!=0){
        //cout << gval;
    }
    return ret;

}
int swritereg(const string& regfolder, const string& key, const string& value, bool createifnotthere, HKEY base){
    HKEY hkey; int gval;
    if (createifnotthere){
        gval = RegCreateKeyA(base,regfolder.c_str(),&hkey);
    }
    else{
        gval = RegOpenKeyExA(base,regfolder.c_str(),NULL,KEY_WRITE,&hkey);
    }
    if (gval!=0){
        return gval;
    }

    gval = RegSetValueExA(hkey,key.c_str(),NULL,REG_SZ,(unsigned char*)value.c_str(),value.size());

    RegCloseKey(hkey);
    return gval;
}
int swriteregdword(const string& regfolder, const string& key, unsigned long value, bool createifnotthere, HKEY base){

    HKEY hkey;int gval;
    if (createifnotthere){
    LPDWORD lpdw;
    gval = RegCreateKeyExA(base,regfolder.c_str(),0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS|KEY_WOW64_64KEY ,NULL,&hkey,lpdw);
        if (gval!=0){
            return gval;
        }
    }
    gval = RegSetValueExA(hkey,key.c_str(),NULL,REG_DWORD,(unsigned char*)&value,sizeof(value));

    RegCloseKey(hkey);

    return gval;
}
typedef LONG (WINAPI * PFN_RegDeleteKeyEx)(HKEY hKey , LPCTSTR lpSubKey , REGSAM samDesired , DWORD Reserved );

PFN_RegDeleteKeyEx _RegDeleteKeyEx;

HINSTANCE hAdvAPI32;
void _Init_AdvAPI32()
{
   hAdvAPI32 = LoadLibraryA(("AdvAPI32.dll"));
   assert(hAdvAPI32 != NULL);

    _RegDeleteKeyEx = (PFN_RegDeleteKeyEx)GetProcAddress(hAdvAPI32 , "RegDeleteKeyExA");
    assert(_RegDeleteKeyEx != NULL);
}

void _Term_AdvAPI32()
{
   FreeLibrary(hAdvAPI32);
}
int sdelreg(const string& regfolder, const string& key, HKEY base){
    HKEY hkey; int gval;
    gval = RegOpenKeyExA(base,regfolder.c_str(),NULL,KEY_WRITE|KEY_READ,&hkey);

    if (gval!=0){
        return gval;
    }
    if (key.size()>0){
        _Init_AdvAPI32();
		//std::wstring wkey = std::wstring(key.begin(),key.end());
        gval = _RegDeleteKeyEx(hkey,(LPCTSTR)key.c_str(),KEY_WOW64_32KEY,NULL);
        _Term_AdvAPI32();
    }
    else{
        _Init_AdvAPI32();
        gval = _RegDeleteKeyEx(hkey,NULL,KEY_WOW64_32KEY,NULL);
        _Term_AdvAPI32();
    }
    //gval = RegSetValueEx(hkey,key.c_str(),NULL,REG_DWORD,(unsigned char*)&value,sizeof(value));

    RegCloseKey(hkey);
    return gval;
}
int sdelregval(const string& regfolder, const string& key, HKEY base){
    HKEY hkey; int gval;
    gval = RegOpenKeyExA(base,regfolder.c_str(),NULL,KEY_SET_VALUE,&hkey);

    if (gval!=0){
        return gval;
    }
    gval = RegDeleteValueA(hkey,key.c_str());
    //gval = RegSetValueEx(hkey,key.c_str(),NULL,REG_DWORD,(unsigned char*)&value,sizeof(value));

    RegCloseKey(hkey);
    return gval;
}

//pretty useless, but whatever.
int screatereg(const string& regfolder,const string& key, const string& value, HKEY base){
    HKEY hkey;
    int gval = RegCreateKeyA(base,regfolder.c_str(),&hkey);
    if (gval!=0)return gval;
    gval = RegSetValueExA(hkey,key.c_str(),NULL,REG_SZ,(unsigned char*)value.c_str(),value.size());
    RegCloseKey(hkey);
    return gval;
}

int savemap(const map<string,string>& tosave, const string& regfolder){
    for(auto& item: tosave){
        swritereg(regfolder,item.first,item.second,1);
    }
	return 1;
}
int loadmap(map<string,string>& tomake, const string& regfolder){
    vector<reg> regs = dirreg(regfolder);
    for(int a = 0; a < regs.size(); a++){
        tomake[regs[a].name]=regs[a].val;
    }
	return 1;
}


user::user(){enabled=false;}
user::user(const user& usr):user(){*this=usr;}
user& user::operator=(const user& usr){if (this!=&usr){name=usr.name;sidstring=usr.sidstring;enabled=usr.enabled;}return *this;}

vector<user> getLocalUsers() {
	vector<user> ret;
	LPUSER_INFO_0 pBuf = NULL;
	LPUSER_INFO_23 betterBuf = NULL;
	LPUSER_INFO_23 pBuf23 = NULL;
	LPUSER_INFO_0 pTmpBuf;
	DWORD dwLevel = 0;
	DWORD betterLevel = 23;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = NULL;

	do // begin do
	{
		nStatus = NetUserEnum((LPCWSTR)pszServerName,
			dwLevel,
			FILTER_NORMAL_ACCOUNT, // global users
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);
		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{
				for (i = 0; (i < dwEntriesRead); i++)
				{

					if (pTmpBuf == NULL)
					{
					    //access violation
						break;
					}

					DWORD nStatus2 = NetUserGetInfo((LPCWSTR)pszServerName,pTmpBuf->usri0_name,betterLevel,(LPBYTE*)&betterBuf);

                    if (nStatus2 == NERR_Success){

                        ret.resize(ret.size() + 1);
                        std::wstring wname=betterBuf->usri23_name;
                        ret.back().name.assign(wname.begin(),wname.end());
                        ret.back().enabled=!betterBuf->usri23_flags&UF_ACCOUNTDISABLE;
                        if (betterBuf->usri23_user_sid != NULL) {
                            LPSTR tid = NULL;
                            DWORD tres = ConvertSidToStringSidA(betterBuf->usri23_user_sid, &tid);
                            if (tid != NULL) {
                                ret.back().sidstring = tid;
                            }
                            LocalFree(tid);
                        }
                    }

					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}
		if (pBuf != NULL)
		{
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}
	while (nStatus == ERROR_MORE_DATA);
	if (pBuf != NULL)
		NetApiBufferFree(pBuf);

	return ret;
}

};
