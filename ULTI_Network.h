/**
    This component needs attention, it was written in 2011- and much of the code could use a good refactor,
    as you should be able to clearly see from the style problems that are rampant in this code
**/

 /**
 This networking file *SHOULD* be cross platform.
 It is meant for basic sock operations.

 Start up by calling startnetworking();

 Close with stopnetworking();

 Use bsend and brecv for most things.

 Possible TODO:
 1:change from stringstream to a more effecient container.
 2:if not 1, create semipermanent stringstream so it isn't created
 every new socket operation.
 **/

#ifndef NETWORKFUNCTIONS_H
#define NETWORKFUNCTIONS_H


#if defined(_WIN32) || defined(_WIN_32)


//If you're on windows, you need to link to several libs:
//(advapi32) && (mswsock) && (winmm) && (ws2_32) && (wsock32)

//***Windows
#undef FD_SETSIZE
#define FD_SETSIZE 1024
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x501
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
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

//***Cross platform
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "utf8.h"

//end xPlat***
namespace ulti{


struct datum{
    int status;
    std::string data;
    datum();
    datum(const datum& idat);
    datum& operator=(const datum& idat);
};

int startnetworking();
int stopnetworking();

int set_nonblocking(int &sock);
int set_blocking(int &sock);

std::string get_all_input(int nonblocksock);
std::string all_input(int &blocksock);

int get_listen_socket(std::string port);
int accept_on_listen_socket(int listenSock);

int autoConnectOnPort(std::string port);
int connectTo(std::string ipAddress, std::string port);

int sendstring(std::string outstring, int sock);

std::string receiveString(int insock);

void closeSocket(int sock);

int ping(char toSend, int sock);

bool isConnected(int sock); // not accurate

sockaddr_in make_sockaddr(const std::string& addr, int port);

std::string get_sock_ip(int sock);
std::string get_sock_ip(const sockaddr_in& addr);

std::string getmyip();
std::string getmyip3();

int udpsock(int port = 0, const char* addr = INADDR_ANY);
std::string recvudp(int sock,const int size);
int sendudp(std::string str, std::string ip, unsigned short port, int sock);
int sendudp(std::string str, const sockaddr_in& dest, int sock);
std::string recvudp(int sock, const int size, sockaddr_in& SenderAddr, int& SenderAddrSize);


//Send/Receive data using the format
//[Character Ascii Value startchar]Formatted Size Of Package[Character Ascii Value endchar]Package
std::string brecv(         int socket,          char startchar = '\x2', char endchar = '\x3');
std::vector<char> vrecv(   int socket,          char startchar = '\x2', char endchar = '\x3');

int bsend(int socket, const std::string& sends,        char startchar = '\x2', char endchar = '\x3');
int bsend(int socket, std::vector<char>  sends, char startchar = '\x2', char endchar = '\x3');
int bsend(int socket, std::vector<char>* sends, char startchar = '\x2', char endchar = '\x3');
int bsend(int socket, std::fstream file,        char startchar = '\x2', char endchar = '\x3');

//Slightly better receive function, status is -1 if the connection was closed 0 if no data received
//and 1 if data received
datum drecv(int socket, char startchar = '\x2', char endchar='\x3');

//Send and receive base 16 windows wide character strings
int wsend(int socket, const std::wstring& sends,       char startchar = '\x2', char endchar = '\x3');
std::wstring wrecv(int socket, char startchar = '\x2', char endchar = '\x3');

//Send a filename
int fsend(int socket, std::string fname,        char startchar = '\x2', char endchar = '\x3');

//Save data directly to a file
int recvandsave(int socket, std::string fname,  char startchar = '\x2', char endchar = '\x3');


//Simply check a socket for read status, 0 = nothing, 1 = something available
int easyselectread(SOCKET socket);

//string getnamefromip(const string& ip);
//string getipfromname(const string& name);

#ifdef _WIN32
//Requires mpr library
DWORD MapNetworkDrive(LPTSTR lpLocalName, LPTSTR lpRemoteName,
                      LPTSTR lpPassword, LPTSTR lpUsername);
DWORD DisconnectNetworkDrive(LPTSTR lpLocalName);
LPTSTR GetUNC(LPTSTR lpLocalName);
void EnumConnectedDrives();



struct spair{
    std::string key,val;
};

struct fshare{
    std::string filename;
    std::string user;
    int handle;
};

//int disconnectshares(const string& filename);
//int disconnectshare(int handle);
//int disconnectshare(fshare share);
//std::vector<fshare> getfileshares();

std::vector<spair> getnetdrives();
int mapnetdrive(const std::string& localname, const std::string& remotename,
                const std::string& username, const std::string& password, DWORD eflags = 0x00001000|0x00000800|CONNECT_INTERACTIVE|CONNECT_UPDATE_PROFILE);
int unmapnetdrive(const std::string& localname);
std::string getunc(const std::string& localname);
std::string getunc4(const std::string& ip);
int getNetworkComputers(std::vector<spair>& inpair, const std::string& baseip3, int timeout = 500, int sectionsize = 64);
std::vector<spair> getNetworkComputers(const std::string& baseip3, int timeout = 500, int sectionsize = 64);
DWORD getNetworkComputers2(LPNETRESOURCEW lpnr, std::vector<std::string>& outvec);
int getNetworkShares(std::vector<std::string>& invec, const std::string& computername);
std::vector<std::string> getNetworkShares(const std::string& computername);
int shareFolder(const std::string& folder, const std::string& name);

#endif


struct connection{
  int sock;
  sockaddr_in sin;
  const connection& operator=(const connection& conin);
};

}

#endif
