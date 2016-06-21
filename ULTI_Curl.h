
#ifndef ULTI_CURL_H
#define ULTI_CURL_H

#ifdef CURL
#include <curl/curl.h>

#include <string>
#include <functional>

namespace ulti{

int clearcurlopt(CURL* curl);

struct httpret{
    std::string body;
    long status;
    std::string error;
    long errorcode;
    httpret();
    httpret(const httpret& hret);
    httpret& operator=(const httpret& hret);
    virtual ~httpret();
};

struct ftpup{
    void* data;
    long current;
    long size;
};

//This is a simple functor object to simplify download/upload callbacks, params=(current, maximum, extra data)
struct progfunc{
    std::function<void(long long,long long,void*)> func;
    void* pass;
};

extern CURLcode lastmsg;
//Two required CURL callbacks for writing and notifying of progress
size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
int progress_callback( void *Bar,curl_off_t dltotal,curl_off_t dlnow,curl_off_t ultotal,curl_off_t ulnow);

//post JSON vs HTTP just changes the http headers to request the correct data interaction types
httpret postjson(CURL* curl, const std::string& url, const std::string& json, const std::string& username = "",const std::string& password = "", progfunc* passdat = NULL);
httpret getjson(CURL* curl, const std::string& url, const std::string& username = "",const std::string& password = "", progfunc* passdat = NULL);
httpret posthttp(CURL* curl, const std::string & url, const std::string& ipost, const std::string& username = "",const std::string& password = "", progfunc* passdat = NULL);
std::string geturl(CURL* curl, const std::string & url, const std::string& username = "",const std::string& password = "", progfunc* passdat = NULL);
httpret gethttp(CURL* curl, const std::string & url, const std::string& username = "",const std::string& password = "", progfunc* passdat = NULL);
httpret posthttpSSH(CURL* curl, const std::string& url, const std::string& ipost, const std::string& passwordSSH = "", const std::string& publicSSH = "", const std::string& privateSSH = "", const std::string& username = "", const std::string& password = "", progfunc* passdat = NULL);

httpret uploadFTP(CURL* curl, const std::string& url, const std::string& filename, const std::string& data, const std::string& username = "", const std::string& password = "", progfunc* passdat = NULL);

httpret postjson(const std::string& url, const std::string& json, const std::string& username = "",const std::string& password = "", progfunc* passdat = NULL);
httpret getjson(const std::string& url, const std::string& username = "",const std::string& password = "", progfunc* passdat = NULL);
httpret posthttp(const std::string & url, const std::string& ipost, const std::string& username = "",const std::string& password = "", progfunc* passdat = NULL);
std::string geturl(const std::string & url, const std::string& username = "",const std::string& password = "", progfunc* passdat = NULL);
httpret gethttp(const std::string & url, const std::string& username = "",const std::string& password = "", progfunc* passdat = NULL);
httpret posthttpSSH(const std::string& url, const std::string& ipost, const std::string& passwordSSH = "", const std::string& publicSSH = "", const std::string& privateSSH = "", const std::string& username = "", const std::string& password = "", progfunc* passdat = NULL);

httpret uploadFTP(const std::string& url, const std::string& upfilename, const std::string& data, const std::string& username = "", const std::string& password = "", progfunc* passdat = NULL);
httpret uploadFTPSSH(const std::string& url, const std::string& upfilename, const std::string& data, const std::string& SSHPrivateFile, const std::string& SSHPublicFile, const std::string& SSHPassword, const std::string& username = "", const std::string& password = "", progfunc* passdat = NULL);
httpret downloadFTP(const std::string& url, const std::string& username = "", const std::string& password = "", progfunc* passdat = NULL);
httpret dirFTP();
httpret ftpCMD(CURL* curl, const std::string& url, const std::string& cmd, const std::string& data, const std::string& username = "", const std::string& password = "", progfunc* passdat = NULL);
httpret ftpCMD(const std::string& url, const std::string& cmd, const std::string& data, const std::string& username = "", const std::string& password = "", progfunc* passdat = NULL);

void startSSH(CURL* curl,const std::string& SSHPublicFile, const std::string& SSHPrivateFile, const std::string& SSHPassword);

std::string getErrorString(CURLcode code);
}
#endif
#endif
