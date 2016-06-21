/**
    usage:

    void func(void* param){
        std::string name = *(string*)param;
        std::cout << name;
    }
    std::string name = "bobby";
    void* param = (void*)&name;
    ulti::thread a = ulti::newthread(func,param);
**/

#ifndef ULTITHREAD_H
#define ULTITHREAD_H



/**
WINDOWS:
HANDLE WINAPI CreateThread(
  _In_opt_   LPSECURITY_ATTRIBUTES lpThreadAttributes,
  _In_       SIZE_T dwStackSize,
  _In_       LPTHREAD_START_ROUTINE lpStartAddress,
  _In_opt_   LPVOID lpParameter,
  _In_       DWORD dwCreationFlags,
  _Out_opt_  LPDWORD lpThreadId);

  Returns NULL if broken

  **example**

  DWORD WINAPI MyThreadFunction( LPVOID lpParam ){
            cout << "totes anew thread";
  }

  HANDLE thread = CreateThread(
            NULL,                   // default security attributes
            0,                      // use default stack size
            MyThreadFunction,       // thread function name
            pDataArray[i],          // argument to thread function
            0,                      // use default creation flags
            &dwThreadIdArray[i]);   // returns the thread identifier
);
**/

#include <thread> //C++11 makes it easy, but isn't working yet //Does work on mingw 4.8.x
#include <functional>

#if defined(_WIN32)||defined(_WIN_32)
#define WIN32_LEAN_AND_MEAN //NECESSARY TO ALLOW NETWORKING, OTHERWISE WINDOWS.H INCLUDES WINSOCK & NOT WINSOCK2
#include <windows.h>

#ifndef threadret
#define threadret DWORD WINAPI
#endif
#endif // _WIN32

#ifdef __linux__
#include <pthread>
    #ifndef threadret
    #define threadret void*
    #endif
#endif // __linux__

namespace ulti{

struct thread{
#if defined(_WIN32)||defined(_WIN_32)
    HANDLE thr;//thread pointer
    DWORD thrid;//identifier of the thread
    LPTHREAD_START_ROUTINE func; //function pointer

    int running();
    int waitFor(DWORD timeout = INFINITE);
    int forceQuit();
#endif // _WIN32

#ifdef __linux__
    pthread_t thr;
    void *(*func) (void *); //function pointer

#endif // __linux__
thread();
thread(const thread& ithr);
thread& operator=(const thread& ithr);


};



#if defined(_WIN32)||defined(_WIN_32)
    thread newthread(void (*func)(void*), LPVOID param);
    template <typename T>
    thread newthread2(T func, LPVOID param){
         //func=(LPTHREAD_START_ROUTINE)func;
         LPTHREAD_START_ROUTINE pfunc=(LPTHREAD_START_ROUTINE)func;
         thread tmp;

         tmp.thr = CreateThread(NULL,0,pfunc,param,0,&tmp.thrid); //returns thread id on success 0 on failure

         return tmp;
    }
#endif // _WIN32

#ifdef __linux__
    thread newthread(void *(*func) (void *), void* param);
#endif // __linux__

}

#endif
