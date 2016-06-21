#include "ULTI_Curl.h"
#ifdef CURL
namespace ulti{

httpret::httpret(){

}
httpret::httpret(const httpret& hret){
    (*this)=hret;
}
httpret& httpret::operator=(const httpret& hret){
    if(this==&hret)
        return *this;
    this->body=hret.body;
    this->error=hret.error;
    this->errorcode=hret.errorcode;
    this->status=hret.status;
    return *this;
}
httpret::~httpret(){

}
CURLcode lastmsg;
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
    ((std::string*)(userdata))->append(ptr,size*nmemb);
    return size*nmemb;
}
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream){

  ftpup* fup = (ftpup*)stream;
  if (fup->current<fup->size){
    long toread = fup->size-fup->current;
    if (toread>(nmemb*size)){
        toread=nmemb*size;
    }
    char* dat = (char*)fup->data;
    memcpy(ptr,(void*)&dat[fup->current],toread);
    fup->current+=toread;
    return toread;
  }
  //size_t retcode = fread(ptr, size, nmemb, (FILE*)stream);

  return 0;
}
int progress_callback( void *Bar,curl_off_t dltotal,curl_off_t dlnow,curl_off_t ultotal,curl_off_t ulnow){
    if (Bar!=NULL){
        if (dlnow>0){
            int x = 5;
        }
        progfunc* pf = (progfunc*)Bar;
        pf->func(dlnow,dltotal,pf->pass);
    }
    return 0;
}
int clearcurlopt(CURL* curl){
    curl_easy_reset(curl);
    return 1;
}
httpret postjson(CURL* curl, const std::string& url, const std::string& json, const std::string& username,const std::string& password, progfunc* passdat){
    httpret output;
    struct curl_slist *headers=NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    output = posthttp(curl,url,json,username,password,passdat);
    curl_slist_free_all(headers);
    return output;
}
httpret getjson(CURL* curl, const std::string& url, const std::string& username,const std::string& password, progfunc* passdat){
    httpret output;
    struct curl_slist *headers=NULL; ///TODO do this one time dammit also postjson
    curl_slist_append(headers, "Accept: application/json");
    curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    //curl_slist_append( headers, "charsets: utf-8");
    output = gethttp(curl,url,username,password,passdat);
    curl_slist_free_all(headers);
    return output;
}
httpret posthttp(CURL* curl, const std::string & url, const std::string& ipost, const std::string& username,const std::string& password, progfunc* passdat){
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,ipost.c_str());
    return gethttp(curl,url,username,password,passdat);
}
std::string geturl(CURL* curl, const std::string & url, const std::string& username,const std::string& password, progfunc* passdat){
    return gethttp(curl,url,username,password,passdat).body;
}
httpret gethttp(CURL* curl, const std::string & url, const std::string& username,const std::string& password, progfunc* passdat){
    httpret output;
    if (username.size()>0)
        curl_easy_setopt(curl,CURLOPT_USERNAME,username.c_str());
    if (password.size()>0)
        curl_easy_setopt(curl,CURLOPT_PASSWORD,password.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,&output.body);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

    if(passdat!=NULL){
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, passdat);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
    }

    output.errorcode = curl_easy_perform(curl);
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &output.status);
    return output;
}

httpret posthttpSSH(CURL* curl, const std::string& url, const std::string& ipost, const std::string& passwordSSH, const std::string& publicSSH, const std::string& privateSSH, const std::string& username, const std::string& password, progfunc* passdat){
    startSSH(curl,publicSSH,privateSSH,passwordSSH);
    return posthttp(curl,url,ipost,username,password,passdat);
}

httpret postjson(const std::string& url, const std::string& json, const std::string& username,const std::string& password, progfunc* passdat){
    CURL* tcurl = curl_easy_init();
    httpret ret = postjson(tcurl,url,json,username,password,passdat);
    curl_easy_cleanup(tcurl);
    return ret;
}
httpret getjson(const std::string& url, const std::string& username,const std::string& password, progfunc* passdat){
    CURL* tcurl = curl_easy_init();
    httpret ret = getjson(tcurl,url,username,password,passdat);
    curl_easy_cleanup(tcurl);
    return ret;
}
httpret posthttp(const std::string & url, const std::string& ipost, const std::string& username,const std::string& password, progfunc* passdat){
    CURL* tcurl = curl_easy_init();
    httpret ret = posthttp(tcurl,url,ipost,username,password,passdat);
    curl_easy_cleanup(tcurl);
    return ret;
}
std::string geturl(const std::string & url, const std::string& username,const std::string& password, progfunc* passdat){
    CURL* tcurl = curl_easy_init();
    std::string ret = geturl(tcurl,url,username,password,passdat);
    curl_easy_cleanup(tcurl);
    return ret;
}
httpret gethttp(const std::string & url, const std::string& username,const std::string& password, progfunc* passdat){
    CURL* tcurl = curl_easy_init();
    httpret ret = gethttp(tcurl,url,username,password,passdat);
    curl_easy_cleanup(tcurl);
    return ret;
}
httpret posthttpSSH(const std::string& url, const std::string& ipost, const std::string& passwordSSH, const std::string& publicSSH, const std::string& privateSSH, const std::string& username, const std::string& password, progfunc* passdat){
    CURL* tcurl = curl_easy_init();
    httpret ret = posthttpSSH(tcurl,url,ipost,passwordSSH,publicSSH,privateSSH,username,password,passdat);
    curl_easy_cleanup(tcurl);
    return ret;
}


httpret uploadFTP(CURL* curl, const std::string& url, const std::string& upfilename, const std::string& data, const std::string& username, const std::string& password, progfunc* passdat){
  httpret ret;
  struct curl_slist *headerlist=NULL;
  std::string tempfilename = upfilename + "_upload";
  std::string upurl = url + upfilename;
  std::string upcmd = "RNFR ";upcmd+=tempfilename;
  std::string rncmd = "RNTO ";rncmd+=upfilename;

  if(curl) {
    headerlist = curl_slist_append(headerlist, upcmd.c_str());
    headerlist = curl_slist_append(headerlist, rncmd.c_str());

    if (username.size()>0)
        curl_easy_setopt(curl,CURLOPT_USERNAME,username.c_str());
    if (password.size()>0)
        curl_easy_setopt(curl,CURLOPT_PASSWORD,password.c_str());

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    curl_easy_setopt(curl, CURLOPT_URL, upurl.c_str());

    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

    ftpup fup;
    fup.current=0;
    fup.size=data.size();
    fup.data=(void*)&data[0];

    curl_easy_setopt(curl, CURLOPT_READDATA, &fup);

    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     data.size());

    ret.errorcode = curl_easy_perform(curl);
    if (ret.errorcode==18){
        if (fup.current>=fup.size){
            ret.errorcode=0;
        }
    }

    curl_slist_free_all (headerlist);

  }
  return ret;
}

httpret uploadFTP(const std::string& url, const std::string& upfilename, const std::string& data, const std::string& username, const std::string& password, progfunc* passdat){
    CURL* tcurl = curl_easy_init();
    curl_easy_setopt(tcurl, CURLOPT_VERBOSE, 1L);
    httpret ret = uploadFTP(tcurl,url,upfilename,data,username,password,passdat);
    curl_easy_cleanup(tcurl);
    return ret;
}

httpret uploadFTPSSH(const std::string& url, const std::string& upfilename, const std::string& data, const std::string& SSHPrivateFile, const std::string& SSHPublicFile, const std::string& SSHPassword, const std::string& username, const std::string& password, progfunc* passdat){
    CURL* tcurl = curl_easy_init();
    //curl_easy_setopt(tcurl, CURLOPT_VERBOSE, 1L);
    startSSH(tcurl,SSHPublicFile,SSHPrivateFile,SSHPassword);
    httpret ret = uploadFTP(tcurl,url,upfilename,data,username,password,passdat);
    curl_easy_cleanup(tcurl);
    return ret;
}

httpret downloadFTP(const std::string& url, const std::string& username, const std::string& password, progfunc* passdat){
    return gethttp(url,username,password,passdat);
}

void startSSH(CURL* curl, const std::string& SSHPublicFile, const std::string& SSHPrivateFile, const std::string& SSHPassword){
    curl_easy_setopt(curl, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_PUBLICKEY);
    curl_easy_setopt(curl, CURLOPT_SSH_PUBLIC_KEYFILE, SSHPublicFile.c_str());
    curl_easy_setopt(curl, CURLOPT_SSH_PRIVATE_KEYFILE, SSHPrivateFile.c_str());
    //curl_easy_setopt(curlUp, CURLOPT_SSH_KNOWNHOSTS, "/home/squirem/.ssh/known_hosts");
    curl_easy_setopt(curl, CURLOPT_KEYPASSWD, SSHPassword.c_str());
}

std::string getErrorString(CURLcode code){
    std::string ret = curl_easy_strerror(code);
    return ret;
}

httpret ftpCMD(CURL* curl, const std::string& url, const std::string& cmd, const std::string& data, const std::string& username, const std::string& password, progfunc* passdat){
  httpret ret;
  struct curl_slist *headerlist=NULL;
  if(curl) {
    headerlist = curl_slist_append(headerlist, cmd.c_str());
    //headerlist = curl_slist_append(headerlist, rncmd.c_str());
    if (username.size()>0)
        curl_easy_setopt(curl,CURLOPT_USERNAME,username.c_str());
    if (password.size()>0)
        curl_easy_setopt(curl,CURLOPT_PASSWORD,password.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);


    ftpup fup;
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    if (data.size()>0){
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        fup.current=0;
        fup.size=data.size();
        fup.data=(void*)&data[0];

        curl_easy_setopt(curl, CURLOPT_READDATA, &fup);

        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                         data.size());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,&ret.body);
    }

    ret.errorcode = curl_easy_perform(curl);
    if (ret.errorcode==18){

        if (fup.current>=fup.size){
            ret.errorcode=0;
        }
    }
    curl_slist_free_all (headerlist);

  }
  return ret;
}

httpret ftpCMD(const std::string& url, const std::string& cmd, const std::string& data, const std::string& username, const std::string& password, progfunc* passdat){
    CURL* tcurl = curl_easy_init();
    curl_easy_setopt(tcurl, CURLOPT_VERBOSE, 1L);
    httpret ret = ftpCMD(tcurl,url,cmd,data,username,password,passdat);
    curl_easy_cleanup(tcurl);
    return ret;
}
}
#endif
