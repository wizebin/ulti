#include "ULTI_Thread.h"

namespace ulti{



#if defined(_WIN32)||defined(_WIN_32)
    thread newthread(void (*func)(void*), LPVOID param){
         //func=(LPTHREAD_START_ROUTINE)func;
         LPTHREAD_START_ROUTINE pfunc=(LPTHREAD_START_ROUTINE)func;
         thread tmp;

         tmp.thr = CreateThread(NULL,0,pfunc,param,0,&tmp.thrid); //returns thread id on success 0 on failure

         return tmp;
    }

    thread::thread(){
        thr=NULL;
        thrid=-1;
        func=NULL;
    }
    thread::thread(const thread& ithr){
        *this=ithr;
    }
    thread& thread::operator=(const thread& ithr){
        if (this==&ithr)
            return *this;

        this->func=ithr.func;
        this->thr=ithr.thr;
        this->thrid=ithr.thrid;

        return *this;
    }

    int thread::running(){
        return waitFor(0);
    }
    int thread::waitFor(DWORD timeout){
        DWORD ret = WaitForSingleObject(thr,timeout);
        if (ret==WAIT_TIMEOUT){
            return 1;
        }
        else if (ret==WAIT_OBJECT_0){
            return 0;
        }
        else{
            return -1;
        }
    }
    int thread::forceQuit(){
        bool res = TerminateThread(thr,1);
        if (res){
            thr=NULL;
        }
        return res;
    }
#endif // _WIN32

#ifdef __linux__
    thread newthread(void *(*func) (void *), void* param){
        thread tmp;

        int ret = pthread_create(&tmp.thr,NULL,func,param); //returns 0 on success error code on failure

        return tmp;
    }
    thread::thread(){
    }
    thread::thread(const thread& ithr){
        *this=ithr;
    }
    thread& thread::operator=(const thread& ithr){
        if (this==&thr)
            return *this;

        this->thr=ithr.thr;
        this->func=ithr.func;

        return *this;
    }
#endif // __linux__


}
