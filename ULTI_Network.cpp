/*
 *  NetworkFunctions.cpp
 *  BasicUnixAdmin
 *
 *  Created by Stephen Cox on 3/21/11.
 *  Copyright 2011 ultifinitech. All rights reserved.
 *
 */

#ifndef UNICODE
#define UNICODE
#endif

#include "ULTI_Network.h"

#undef INVALID_SOCKET
#define INVALID_SOCKET -1

#include <iostream>

#undef _WIN_32
#define _WIN_32

#include "utf8.h"
#include "ULTI_Utility.h"
#include "ULTI_Thread.h"

namespace ulti{

int startnetworking(){
 #if defined(_WIN32)||defined(_WIN_32)
 WSADATA wsadata;
 if (WSAStartup(MAKEWORD(2,2),&wsadata) == -1)
    return -1;
 if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
    return -2;
 return 1;
 #endif
 return 0;
}
int stopnetworking(){
 #if defined(_WIN32)||defined(_WIN_32)
 return WSACleanup();
 #endif
 return 1;
}

datum::datum(){status=0;}
datum::datum(const datum& idat){*this=idat;}
datum& datum::operator=(const datum& idat){if (this!=&idat){this->status=idat.status;this->data=idat.data;}return *this;}

int set_nonblocking(int &sock){

    #if defined(_WIN32)||defined(_WIN_32)
    //-------------------------
// Set the socket I/O mode: In this case FIONBIO
// enables or disables the blocking mode for the
// socket based on the numerical value of iMode.
// If iMode = 0, blocking is enabled;
// If iMode != 0, non-blocking mode is enabled.

    u_long iMode = 1;
    int iResult = ioctlsocket(sock, FIONBIO, &iMode);
	if (iResult != NO_ERROR) {
		std::cout << "WTF, NONBLOCKINGFAIL";
		return -1;
	}



    #else

    int flags = fcntl(sock, F_GETFL, 0);
    return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    #endif
	return 1;
}
int set_blocking(int &sock){
    #if defined(_WIN32)||defined(_WIN_32)
    u_long iMode = 0;
    int iResult = ioctlsocket(sock, FIONBIO, &iMode);
    if (iResult != NO_ERROR)
        return -1;
      //cout << "WTF, BLOCKINGFAIL";

    #else
    int flags = fcntl(sock, F_GETFL, 0);
    return fcntl(sock,F_SETFL, ((flags | O_NONBLOCK) ^ O_NONBLOCK));
    #endif
    return 1;
}
std::string get_all_input(int sock){
    char buf[255] = {NULL};
    bool done = false;
    std::string input = "";
    while (!done){
        if (recv(sock, buf, 255, 0) == -1){
            break;
        }
        input += buf;
    }
    return input;
}
std::string all_input(int &sock){
    set_nonblocking(sock);
    char buf[512];
    buf[0] = '\0';
    bool done = false;
    std::string input = "";
    while (!done){
        int ret = recv(sock,buf,sizeof(buf),0);
        if (ret < 1){
            break;
        }
        input += buf;
    }
    return input;
}

int get_listen_socket(std::string port){
	int set = 1;
	int ListenSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, hints;

#if defined(_WIN32)||defined(_WIN_32)

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != 0) {
		//printf("WSAStartup failed: %d\n", iResult);
		return -1;
	}

#endif


	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;


	if (getaddrinfo(NULL, port.c_str(), &hints, &result) != 0){
		//perror("getaddrinfo failed!");
		return -1;
	}

	ListenSocket =  socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == -1){
		//perror("Listen Socket Fail!");
		freeaddrinfo(result);
		return -1;
	}
	//setsockopt(ListenSocket, SOL_SOCKET, 0, (char *)&set, sizeof(int));
	if (bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen) == -1){
		//perror("Bind Fail!");
		closesocket(ListenSocket);

		return -1;
	}
	freeaddrinfo(result);

	//Server Started!

	if (listen(ListenSocket, SOMAXCONN) == -1){
		//perror("Listen Failed!");
		//shutdown(ListenSocket, SHUT_RDWR);
		closesocket(ListenSocket);
		return -1;
	}
	return ListenSocket;

}

int accept_on_listen_socket(int listenSock){
	int set = 1;
	int classSock = accept(listenSock, NULL, NULL);
	//setsockopt(classSock, SOL_SOCKET, SO_REUSEADDR, (char *)&set, sizeof(int));
	if (classSock == -1){
		//perror("Accept Failed!!");
		return -1;
	}
	return classSock;

}

int autoConnectOnPort (std::string port){
	int listen = get_listen_socket(port);
	int endsock = accept_on_listen_socket(listen);
	closesocket(listen);
	return endsock;
}

int connectTo(std::string ipAddress, std::string port){
	int classSock = -1;
	struct addrinfo *result = NULL, *ptr =NULL, hints;

#if defined(_WIN32)||defined(_WIN_32)

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(iResult != 0) {
		//printf("WSAStartup failed: %d\n", iResult);
		return -1;
	}

#endif

	memset (&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(ipAddress.c_str(), port.c_str(), &hints, &result) != 0){
		//perror("GetAddrInfo Failed!");
		return -1;
	}
	for (ptr = result; ptr != NULL; ptr=ptr->ai_next){
		classSock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (classSock == INVALID_SOCKET){
			//perror("Error at Socket!!");
			freeaddrinfo(result);
			return -1;
		}

		if (connect(classSock, ptr ->ai_addr, (int)ptr->ai_addrlen) == -1){
			closesocket(classSock);
			classSock = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);

	if(classSock == -1){
		//perror("Unable To Connect!!");
		return -1;
	}

	return classSock;
}

int sendString(std::string outString, int sock){
	if (sock != INVALID_SOCKET && outString.size() > 0){
		if (outString[outString.size() - 1] != (char)4){
			outString+=(char)4;
		}

		std::ostringstream oss("");
        oss << (char)7;
        oss << "{";
        oss << outString.size();
        oss << "}";
        oss << (char)8;
        oss << outString;

		if(send(sock, oss.str().c_str(), oss.str().size(), 0)  < 1){
			closesocket(sock);
			return -1;
		}
		return 1;
	}
	return 0;
}

std::string receiveString (int insock){

    char recvbuf[1] = {NULL};
    int iResult = 0;
    std::string output = "";
    int size = 0;

    iResult = recv(insock, recvbuf, 1, 0);
    if (recvbuf[0] == char(7)){ //it's a data trans.. blech should do std::vector<char> but i'm lazy
        std::stringstream oss("");
        while(recvbuf[0] != char(8) && iResult > 0){
            iResult = recv(insock, recvbuf, 1, 0);
            if(isdigit(recvbuf[0]))
                oss << recvbuf[0];
        }
        //cout << endl << oss.str();
        size = atoi(oss.str().c_str());
        for (int a = 0; a < size && iResult > 0; a++ ){
            iResult = recv(insock, recvbuf, 1, 0);
                output += recvbuf[0];
        }
    }
    else{
    if (recvbuf[0] != char(4))
        output += recvbuf[0];
    while(recvbuf[0] != char(4) && iResult > 0){
        iResult = recv(insock, recvbuf, 1, 0);
        if(recvbuf[0] != char(4))
            output += recvbuf[0];
    }
    }


    if(iResult > 0) {
        return output;
    }
    else if(iResult == 0) {
        //printf("Connection closing...\n");
        output = "";
        //output += char(24);
        return output;
    }
    else  {
        //closeSocket(insock);
        output = "";
        //output += char(24);
        return output;
    }

}



void closeSocket(int sock){
	closesocket(sock);
}

int ping(char toSend, int sock){
	std::string out = &toSend;
	return sendString(out,sock);
}

bool isConnected(int sock){
	if (sock == -1)
		return false;
	char out[1] = {NULL};
	int retval = send(sock,out,0,0);
	if (retval == 0) {
		return true;
	}
	return false;
}

sockaddr_in make_sockaddr(const std::string& addr, int port){
    sockaddr_in address;
    address.sin_family = AF_INET;
    if (addr == "")
        address.sin_addr.s_addr = INADDR_ANY;
    else
        address.sin_addr.s_addr = inet_addr(addr.c_str());
    address.sin_port = htons( (unsigned short) port );
    return address;
}

std::string get_sock_ip(int sock) {
	sockaddr_in addr;
	socklen_t len = sizeof(addr);
	getpeername(sock, (sockaddr*)&addr, &len);
	return std::string(inet_ntoa(addr.sin_addr));
}

std::string get_sock_ip(const sockaddr_in& addr){
    return std::string(inet_ntoa(addr.sin_addr));
}


int udpsock(int port, const char* addr){
    int handle = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if (handle < 1)
        return -1;

    sockaddr_in address;
    address.sin_family = AF_INET;
    if (addr == INADDR_ANY)
        address.sin_addr.s_addr = INADDR_ANY;
    else
        address.sin_addr.s_addr = inet_addr(addr);
    address.sin_port = htons( (unsigned short) port );

    if ( bind( handle, (const sockaddr*) &address, sizeof(sockaddr_in) ) < 0 )
        return -1;

    return handle;
}
std::string recvudp(int sock,const int size){
    sockaddr_in SenderAddr;
    int SenderAddrSize = sizeof (SenderAddr);
	std::string buf;
	buf.resize(size);

    int retsize = recvfrom(sock, &buf[0], sizeof(buf), 0, (SOCKADDR *) & SenderAddr, &SenderAddrSize);
    if (retsize == -1){
        if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == 0){
         return "";
        }
        return "\0";
    }
    else if (retsize < size){
        buf[retsize] = NULL;
    }
    return buf;
}
int sendudp(std::string str, std::string ip, unsigned short port, int sock){
    if (str.size()==0)
        return 0;
    sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr( ip.c_str() );
    dest.sin_port = htons( port );
    return sendto(sock,str.c_str(),str.size(),0, (sockaddr*)&dest,sizeof(dest));
}

std::string recvudp(int sock, const int size, sockaddr_in& SenderAddr, int& SenderAddrSize)
{
        // TODO: use std::vector<char> here instead of char array
    char* buf = 0;
    buf = new char[size];

    int retsize = recvfrom(sock, buf, size, 0, (sockaddr*) &SenderAddr, &SenderAddrSize);

    if(retsize == -1)
    {
        //cout << "\nRecv Error : " << WSAGetLastError();

        if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == 0)
        {
            return "";
        }

        return "\0";
    }
    else if (retsize < size)
    {
        buf[retsize] = NULL;
    }

    std::string str(buf);
    delete[] buf;

    return str;
}

// On the client side, prepare dest like this:
//  sockaddr_in dest;
//  dest.sin_family = AF_INET;
//  dest.sin_addr.s_addr = inet_addr(ip.c_str());
//  dest.sin_port = htons(port);
int sendudp(std::string str, const sockaddr_in& dest, int sock)
{
    if (str.size()==0)
        return 0;

    int ret = sendto(sock,str.c_str(),str.size(),0, (sockaddr*)&dest,sizeof(dest));

    //if (ret == -1)
    //{
        //cout << "\nSend Error Code : " <<  WSAGetLastError();

    //}

    return ret;
}



datum drecv(int socket, char startchar, char endchar){
    datum fret;fret.status=0;
    char cbuf[1];
    int ret = recv(socket,cbuf,sizeof(cbuf),0);
    while(cbuf[0] != startchar && ret > 0){
        ret = recv(socket,cbuf,sizeof(cbuf),0);
    }
    //if (ret == 0) //client closed connection
    if (ret <= 0){
        fret.status=-1;
        return fret;
    }

    ret = recv(socket,cbuf,sizeof(cbuf),0);
    std::stringstream ss("");
    while(cbuf[0] != endchar && ret > 0){
        ss << cbuf[0];
        ret = recv(socket,cbuf,sizeof(cbuf),0);
    }

    int size = 0;
    ss >> size;

    if (size <= 0)
        return fret;

    if (ret <= 0){
        fret.status=-1;
        return fret;
    }

    fret.status=1;

    char* buf = new char[size]; // this may have to change to 1, if we're getting garbage
    //memset(buf,sizeof(buf),NULL);//this may be unnecessary

    while(size > 0 && ret > 0){
        ret = recv(socket,buf,size,0);
        if (ret <= 0){
            fret.status=-1;
            break;
        }
        size -= ret;
        if (fret.data.size()==0){
            fret.data.assign(buf,buf+ret);
        }
        else{
          for (int a = 0; a < ret; a++){
            fret.data.push_back(buf[a]);
          }
        }
    }
    delete[] buf;
    return fret;
}

std::string brecv(int socket, char startchar, char endchar){
 char cbuf[1];
 int ret = recv(socket,cbuf,sizeof(cbuf),0);
 while(cbuf[0] != startchar && ret > 0){
    ret = recv(socket,cbuf,sizeof(cbuf),0);
 }
 //if (ret == 0) //client closed connection
 if (ret <= 0)
    return "";

 ret = recv(socket,cbuf,sizeof(cbuf),0);
 std::stringstream ss("");
 while(cbuf[0] != endchar && ret > 0){
    ss << cbuf[0];
    ret = recv(socket,cbuf,sizeof(cbuf),0);
 }

 int size = 0;
 ss >> size;

 if (size <= 0)
    return "";

 if (ret <= 0)
    return "";

 std::string rets = "";

 char* buf = new char[size]; // this may have to change to 1, if we're getting garbage
 //memset(buf,sizeof(buf),NULL);//this may be unnecessary

 while(size > 0 && ret > 0){
  ret = recv(socket,buf,size,0);
  if (ret <= 0)
    break;
  size -= ret;
  if (rets.size()==0){
   rets.assign(buf,buf+ret);
  }
  else{
      for (int a = 0; a < ret; a++){
        rets += buf[a];
      }
  }
 }

 delete[] buf;

 return rets;
}

std::vector<char> vrecv(int socket, char startchar, char endchar){
 char cbuf[1];
 int ret = recv(socket,cbuf,sizeof(cbuf),0);
 while(cbuf[0] != startchar && ret > 0){
    ret = recv(socket,cbuf,sizeof(cbuf),0);
 }
 //if (ret == 0) //client closed connection
 if (ret <= 0)
    return std::vector<char>();

 ret = recv(socket,cbuf,sizeof(cbuf),0);
 std::stringstream ss("");
 while(cbuf[0] != endchar && ret > 0){
    ss << cbuf[0];
    ret = recv(socket,cbuf,sizeof(cbuf),0);
 }

 int size = 0;
 ss >> size;

 if (size <= 0)
    return std::vector<char>();

 if (ret <= 0)
    return std::vector<char>();

 std::vector<char> invec(size);
 //invec.resize(size);

 int curchar = 0;

 while(size > 0 && ret > 0){
  ret = recv(socket,&invec[curchar],size,0);
  if (ret <= 0)
    break;
  size -= ret;
  curchar+=ret;
 }

 return invec;
}

int bsend(int socket, const std::string& sends, char startchar, char endchar){
    if (sends.size() == 0)
        return 0;
    std::stringstream ss("");
    ss << startchar;
    ss << sends.size();
    ss << endchar;

    int ret = send(socket,ss.str().c_str(),ss.str().size(),0);

    if (ret < ss.str().size())
        return -1;

    ret = send(socket,sends.c_str(),sends.size(),0);
    if (ret < sends.size())
        return -1;

    return sends.size();
}
int bsend(int socket, std::vector<char> sends, char startchar, char endchar){
    std::stringstream ss("");
    ss << startchar;
    ss << sends.size();
    ss << endchar;

    if (send(socket,ss.str().c_str(),ss.str().size(),0) < ss.str().size())
        return -1;

    if (send(socket,&sends[0],sends.size(),0) < sends.size())
        return -1;

    return sends.size();
}
int bsend(int socket, std::vector<char>* sends, char startchar, char endchar){
    std::stringstream ss("");
    ss << startchar;
    ss << sends->size();
    ss << endchar;

    int endsize = sends->size();

    if (send(socket,ss.str().c_str(),ss.str().size(),0) < ss.str().size())
        return -1;

    if (send(socket,&(*sends)[0],sends->size(),0) < sends->size())
        return -1;

    return sends->size();
}

int bsend(int socket, std::fstream file, char startchar, char endchar){
    //std::ifstream ifs("foobar.txt", std::ios::binary);

    file.seekg(0, std::ios::end);
    std::ifstream::pos_type filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> bytes(filesize);

    file.read(&bytes[0], filesize);

    return bsend(socket,&bytes);
}

int wsend(int socket, const std::wstring& str, char startchar, char endchar){
    std::string utf8line;
    utf8::utf16to8(str.begin(), str.end(), back_inserter(utf8line));
    return bsend(socket,utf8line,startchar,endchar);
}
std::wstring wrecv(int socket, char startchar, char endchar){
    std::string utf8line = brecv(socket,startchar,endchar);
    std::wstring utf16line;

    std::string::iterator end_it = utf8::find_invalid(utf8line.begin(), utf8line.end());
    if (end_it != utf8line.end()) {
        //WE HAVE AN ENCODING ISSUE
    }

    utf8::utf8to16(utf8line.begin(), end_it, back_inserter(utf16line));
    return utf16line;
}

int fsend(int socket,std::string fname, char startchar, char endchar){
    std::fstream file(fname.c_str(), std::ios::in|std::ios::binary);
    if (!file)
        return -1;


    file.seekg(0, std::ios::end);
    std::ifstream::pos_type filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> bytes(filesize);

    file.read(&bytes[0], filesize);

    file.close();

    int endsize = bytes.size();

    return bsend(socket,&bytes);
}

int recvandsave(int socket, std::string fname, char startchar, char endchar){
    char cbuf[1];
     int ret = recv(socket,cbuf,sizeof(cbuf),0);
     while(cbuf[0] != startchar && ret > 0){
        ret = recv(socket,cbuf,sizeof(cbuf),0);
     }
     if (ret <= 0)
        return -1;

     ret = recv(socket,cbuf,sizeof(cbuf),0);
     std::stringstream ss("");
     while(cbuf[0] != endchar && ret > 0){
        ss << cbuf[0];
        ret = recv(socket,cbuf,sizeof(cbuf),0);
     }

     int size = 0;
     ss >> size;

     if (size==0)
        return 0;


    std::fstream file(fname.c_str(), std::ios::out|std::ios::binary);
    if (!file)
        return -1;

    char buf[255] = {NULL};

    int curchar = 0;
    while(curchar<size){
     ret = recv(socket,&buf[0],sizeof(buf),0);
     if (ret==-1){
      file.close();
      return -1;
     }
     file.write(&buf[0],ret);
     curchar+=ret;
    }
    file.close();
    return size;
}

const connection& connection::operator=(const connection& conin){
 sock = conin.sock;
 sin=conin.sin;
 return *this;
}

std::string getmyip(){
    std::stringstream ret;
    char szBuffer[1024];
    if (gethostname(szBuffer,sizeof(szBuffer))!=SOCKET_ERROR){
        struct hostent* host = gethostbyname(szBuffer);
        if (host!=NULL){
            ret << (int)((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b1 << ".";
            ret << (int)((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b2 << ".";
            ret << (int)((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b3 << ".";
            ret << (int)((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b4;
        }
    }
    return ret.str();
}
std::string getmyip3(){ //returns your subnet... kinda (actually we cheat and assume your net mast is 255.255.255.0
    std::stringstream ret;
    char szBuffer[1024];
    if (gethostname(szBuffer,sizeof(szBuffer))!=SOCKET_ERROR){
        struct hostent* host = gethostbyname(szBuffer);
        if (host!=NULL){
            ret << (int)((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b1 << ".";
            ret << (int)((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b2 << ".";
            ret << (int)((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b3;
        }
    }
    return ret.str();
}

int easyselectread(SOCKET socket){
    static FD_SET tset;
    tset.fd_array[0]=socket;
    tset.fd_count=1;
    static timeval tv = {0,1};
    return select(1,&tset,NULL,NULL,&tv);
}



#if defined(_WIN32)||defined(_WIN_32)
std::vector<spair> getnetdrives(){
    HANDLE hEnum;
    LPNETRESOURCEW lpNr;
    DWORD dwResult, dwCount=-1, dwIndex;
    DWORD dwBufsize=16384; //no error check
    std::vector<spair> ret;

    dwResult = WNetOpenEnum(RESOURCE_CONNECTED,
        RESOURCETYPE_DISK, 0, NULL, &hEnum);

    if (dwResult!=NO_ERROR) return ret;

    lpNr = (LPNETRESOURCEW) GlobalAlloc(GPTR, dwBufsize);
    ZeroMemory(lpNr, dwBufsize);

    dwResult = WNetEnumResource(hEnum, &dwCount, lpNr, &dwBufsize);

    for (dwIndex=0; dwIndex < dwCount; dwIndex++)
    {
        NETRESOURCEW ns = (NETRESOURCEW)lpNr[dwIndex];
        ret.push_back(spair());
		std::wstring local = ns.lpLocalName;
		std::wstring remote = ns.lpRemoteName;
        ret.back().key=std::string(local.begin(),local.end());
        ret.back().val=std::string(remote.begin(),remote.end());
    }

    GlobalFree((HGLOBAL)lpNr);
    WNetCloseEnum(hEnum);
    return ret;
}
//static int CONNECT_CMD_SAVECRED=0x00001000;
int mapnetdrive(const std::string& localname, const std::string& remotename,
                const std::string& username, const std::string& password, DWORD eflags){
    NETRESOURCEA nr;
    nr.dwType=RESOURCETYPE_DISK;
    nr.dwScope=RESOURCE_GLOBALNET;

    //DWORD dwFlags= CONNECT_UPDATE_PROFILE;
    nr.lpLocalName=(LPSTR)localname.c_str();
    nr.lpRemoteName=(LPSTR)remotename.c_str();
    nr.lpProvider=(LPSTR) NULL;

    //ERROR_ALREADY_ASSIGNED
    //ERROR_INVALID_ADDRESS
    //ERROR_BAD_DEVICE
    //ERROR_PASSWORD_RESTRICTION
    //ERROR_BAD_USERNAME
    if (username.size()>0){
        return WNetAddConnection2A(&nr, password.c_str(),username.c_str(), eflags);
    }
    else{
        return WNetAddConnection2A(&nr, NULL,NULL, eflags);
    }
}
int unmapnetdrive(const std::string& localname){
    return WNetCancelConnection2A(localname.c_str(), 0, FALSE);
}
std::string getunc(const std::string& localname){
    CHAR szDeviceName[1024];
    DWORD bufsize = sizeof(szDeviceName);

    DWORD dwResult = WNetGetConnectionA(localname.c_str(),
        (LPSTR) szDeviceName, &bufsize);

    //ERROR_NOT_CONNECTED
    std::string ret;
    if (dwResult==NO_ERROR){ret = szDeviceName;}
    return ret;
}
std::string getunc4(const std::string& ip){
    #if defined(_WIN32)||defined(_WIN_32)
        struct addrinfo    hints;
        struct addrinfo   *res=0;
        int status=getaddrinfo(ip.c_str(),0,0,&res);
        char host[512],port[128];
        status=getnameinfo(res->ai_addr,res->ai_addrlen,host,512,0,0,0);
        std::string ret = host;
        freeaddrinfo(res);
        return ret;
    #else
        hostent* het = gethostbyaddr(ip.c_str(),ip.size(),AF_INET);
        if (het==NULL)
            return "";
        return std::string(het->h_name);
    #endif // _WIN_32
}
struct ipUnc{
    std::string ip;
    std::string unc;
    int tst;
};

void ipUncFunc(void* tstruct){
    ipUnc* ts = (ipUnc*)tstruct;
    std::string ip = ts->ip;
    std::string unc = ulti::getunc4(ip);
    ts->unc=unc;
}
DWORD getNetworkComputers2(LPNETRESOURCEW lpnr, std::vector<std::string>& outvec)
{


    DWORD dwResult, dwResultEnum;
    HANDLE hEnum;
    DWORD cbBuffer = 16384;     // 16K is a good size
    DWORD cEntries = -1;        // enumerate all possible entries
    LPNETRESOURCEW lpnrLocal;    // pointer to enumerated structures
    DWORD i;

    dwResult = WNetOpenEnumW(RESOURCE_GLOBALNET, // all network resources
                            RESOURCETYPE_ANY,   // all resources
                            0,  // enumerate all resources
                            lpnr,       // NULL first time the function is called
                            &hEnum);    // handle to the resource

    if (dwResult != NO_ERROR) {
        //printf("WnetOpenEnum1 failed with error %d %s\n", dwResult, ulti::getWindowsErrorString(dwResult).c_str());
        return dwResult;
    }
    lpnrLocal = (LPNETRESOURCEW) GlobalAlloc(GPTR, cbBuffer);
    if (lpnrLocal == NULL) {
        //printf("WnetOpenEnum2 failed with error %d %s\n", dwResult, ulti::getWindowsErrorString(dwResult).c_str());
        return dwResult;
    }

    do {
        ZeroMemory(lpnrLocal, cbBuffer);
        dwResultEnum = WNetEnumResourceW(hEnum,  // resource handle
                                        &cEntries,      // defined locally as -1
                                        lpnrLocal,      // LPNETRESOURCE
                                        &cbBuffer);     // buffer size
        if (dwResultEnum == NO_ERROR) {
            for (i = 0; i < cEntries; i++) {



                if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER)){
                    //if(!CString(lpnrLocal[i].lpRemoteName).IsEmpty())//Second Hand https://msdn.microsoft.com/en-us/library/windows/desktop/aa385341(v=vs.85).aspx
                        if (!getNetworkComputers2(&lpnrLocal[i],outvec)){
                            //printf("EnumerateFunc returned FALSE\n");
                        }
                }
                else{
                    std::wstring tmp = lpnrLocal[i].lpRemoteName;
                    std::string tmp2(tmp.begin(),tmp.end());
                    outvec.push_back(tmp2);
                }
            }
        }

        else if (dwResultEnum != ERROR_NO_MORE_ITEMS) {
            //printf("WNetEnumResource failed with error %d %s\n", dwResultEnum, ulti::getWindowsErrorString(dwResultEnum).c_str());

            break;
        }
    }
    while (dwResultEnum != ERROR_NO_MORE_ITEMS);
    GlobalFree((HGLOBAL) lpnrLocal);
    dwResult = WNetCloseEnum(hEnum);

    if (dwResult != NO_ERROR) {
        //printf("WNetCloseEnum failed with error %d %s\n", dwResult, ulti::getWindowsErrorString(dwResult).c_str());
        return dwResult;
    }

    return ERROR_SUCCESS;
}
int getNetworkComputers(std::vector<spair>& inpair, const std::string& baseip3, int timeout, int sectionsize){
    int ret=0;
    std::vector<thread> thr;
    thr.resize(256);
    ipUnc structors[256];
    for(int a = 0; a < 256; a++){
        std::string tmp = (ulti::stringBuilder() << baseip3 << "." << a).str();
        structors[a].ip=tmp;
    }

    for(int a = 0; a < 256;a+=sectionsize){
        //Callback a/256
        std::cout << a << " / " << 256 << "\n";
        for(int c = a; c < a+sectionsize && c < 256; c++){
            thr[c]=ulti::newthread(ipUncFunc,(void*)&structors[c]);
            if (thr[c].thr==NULL){
                std::cout << a << " ERR " << GetLastError() << " : " << ulti::getWindowsErrorString(GetLastError()) << "\n";
            }
        }

        std::vector<HANDLE> hands;

        for(int c = a; c < a+sectionsize && c < 256; c++){
            if (thr[c].thr!=NULL && thr[c].running()){
                hands.push_back(thr[c].thr);
            }
        }


        int res = WaitForMultipleObjects(hands.size(),&hands[0],true,timeout);
        if (res==WAIT_FAILED){
            std::cout << "ERR " << GetLastError() << " : " << ulti::getWindowsErrorString(GetLastError()) << "\n";
        for(int a = 0; a < hands.size(); a++){
            std::cout << a << " : " << hands[a] << "\n";
        }
            return -1;
        }

        for(int c = a; c < a+sectionsize && c < 256; c++){
            if (structors[c].unc.size()>0){
                inpair.resize(inpair.size()+1);
                inpair.back().key=structors[c].ip;
                inpair.back().val=structors[c].unc;
                ret++;
            }
        }
    }
    for(int c = 0; c < 256; c++){
        if (thr[c].running()){
            thr[c].forceQuit();
        }
    }
    return ret;
}
std::vector<spair> getNetworkComputers(const std::string& baseip3, int timeout, int sectionsize){
    std::vector<spair> ret;
    getNetworkComputers(ret,baseip3,timeout,sectionsize);
    return ret;

}

int getNetworkShares(std::vector<std::string>& invec, const std::string& computername){


    PSHARE_INFO_1 BufPtr,p;
    NET_API_STATUS res;
    std::wstring wname(computername.begin(),computername.end());
    LPWSTR   lpszServer = NULL;
    if (computername.size()>0){
        lpszServer = &wname[0];
    }
    DWORD er=0,tr=0,resume=0, i;

    do
    {
      res = NetShareEnum (lpszServer, 1, (LPBYTE *) &BufPtr, MAX_PREFERRED_LENGTH, &er, &tr, &resume);

      if(res == ERROR_SUCCESS || res == ERROR_MORE_DATA)
      {
         p=BufPtr;

         for(i=1;i<=er;i++)
         {
            #ifdef UNICODE
                if (p->shi1_type==STYPE_DISKTREE){

                    std::wstring str = p->shi1_netname;
                    std::string res(str.begin(),str.end());
                    invec.push_back(res);
                }
            #else
                if (p->shi1_type==STYPE_DISKTREE){
                    invec.push_back(std::string(p->shi1_netname));
                }
            #endif

            p++;
         }
         NetApiBufferFree(BufPtr);
      }
      //else
      //   printf("Error: [%ld] %s\n",res, getWindowsErrorString(res).c_str());
    }
    while (res==ERROR_MORE_DATA);
    if (res==0)
        return 1;
    return res;
}
std::vector<std::string> getNetworkShares(const std::string& computername){
    std::vector<std::string> ret;
    getNetworkShares(ret,computername);
    return ret;
}
int shareFolder(const std::string& folder, const std::string& name){

    NET_API_STATUS res;
    SHARE_INFO_502 p;
    DWORD parm_err = 0;

    //
    // Fill in the SHARE_INFO_2 structure.
    //

    SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
    PSID everyone_sid = NULL;
    AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID,
    0, 0, 0, 0, 0, 0, 0, &everyone_sid);

    EXPLICIT_ACCESS ea;
    ZeroMemory(&ea, sizeof(EXPLICIT_ACCESS));
    ea.grfAccessPermissions = SPECIFIC_RIGHTS_ALL | STANDARD_RIGHTS_ALL;
    ea.grfAccessMode = SET_ACCESS;
    ea.grfInheritance = NO_INHERITANCE;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
#ifdef UNICODE
    ea.Trustee.ptstrName  = (LPWSTR)everyone_sid;
    #else
    ea.Trustee.ptstrName  = (LPSTR)everyone_sid;
    #endif // _UNICODE

    PACL acl = NULL;
    SetEntriesInAcl(1, &ea, NULL, &acl);

    PSECURITY_DESCRIPTOR sd = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,
                                   SECURITY_DESCRIPTOR_MIN_LENGTH);
    InitializeSecurityDescriptor(sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(sd, TRUE, acl, FALSE);

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = sd;
    sa.bInheritHandle = FALSE;




    #ifdef _UNICODE
    std::wstring wname(name.begin(),name.end());
    p.shi502_netname = &wname[0];
    #else
    p.shi502_netname = (LPTSTR)&name[0];
    #endif // _UNICODE
    p.shi502_type = STYPE_DISKTREE; // disk drive
    p.shi502_remark = TEXT("Automated ULTI-Share");
    p.shi502_permissions = 0;
    p.shi502_max_uses = 4;
    p.shi502_current_uses = 0;
    #ifdef _UNICODE
    std::wstring wstr(folder.begin(),folder.end());
    p.shi502_path = &wstr[0];
    #else
    p.shi502_path = (LPTSTR)&folder[0];
    #endif // _UNICODE
    p.shi502_passwd = NULL; // no password
    p.shi502_security_descriptor= NULL;//sd;
    //
    // Call the NetShareAdd function,
    //  specifying level 2.
    //
    res=NetShareAdd(NULL, 502, (LPBYTE) &p, &parm_err);

    std::cout << "PARM " << parm_err << "\n";

    FreeSid(everyone_sid);
    LocalFree(sd);
    LocalFree(acl);
    //
    // If the call succeeds, inform the user.
    //
    if(res==0)
        return res;

    // Otherwise, print an error,
    //  and identify the parameter in error.
    //
    else
        return res;



}
#endif
}
