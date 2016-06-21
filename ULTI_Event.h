#ifndef ULTI_EVENT_H
#define ULTI_EVENT_H

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <deque>
#include <limits.h>
#include "ULTI_Structs.h"
#include "ULTI_Object.h"
namespace ulti{

typedef unsigned short EVENTINT;

class responder;
class eventStation;

/**
    This class allows many event manager classes to keep a consistent type/id map for events
    The int ID is especially important because of the CPU savings compared with a string ID,
    Using this Class you can have the convenience of naming your events with the speed of having IDs.
**/

class MessageTypes{
    protected:
    std::map<int,std::string> imap;
    std::map<std::string,int> smap;

    int nextID;
    public:
    int getType(const std::string& tname);
    int registerType(const std::string& tname);
    int registerType(const std::string& tname, int num);
    bool isNumberRegistered(int num);
    bool isNameRegistered(std::string name);
    std::string getTypeName(int num);

    MessageTypes();
};

//currently unused, not sure of the best way to implement this
const static int ANY_MESSAGE = -1;

//Not a singleton class, but it could be made into one, singletons don't really mesh well with this paradigm
extern MessageTypes messageTypes;


int getType(const std::string& tname);
int registerType(const std::string& tname);
int registerType(const std::string& tname, int num);
bool isNumberRegistered(int num);
std::string getTypeName(int num);


/**
    Generic event data object, this is the class used to pass event data to responders
    For windows based systems
        value1 = WPARAM
        value2 = LPARAM
        handle = HWND
        type   = MSG

    Linux not yet implemented
**/

struct message{
    ulti::element data;
    ulti::element ret;
    int type;
    int id;
    unsigned long value1;
    unsigned long value2;
    unsigned long handle;
    bool propagate;
    bool preventDefault;
    responder* parent;
    int respond(const element& response);//use this to send a message back to parent
    bool operator==(const message&);
    message& operator=(const message&);
    message();
    message(const message& in);
    message(const ulti::element& in,int type=0,responder* parent=NULL);
    message(const ulti::element& in,int type=0, int id=0, responder* parent=NULL);
};


/**
    This class is the workhorse of the event subsystem, responders must be subclasses of the responder class
    which uses a linked list for response priority. In the future this will be a member object instead of
    a required parent class
**/

class eventStation{
public:
    //EVENTINT nextMessageType;
    bool frameMessagesByDefault;
    std::vector<responder*> childrenToDelete;

    //std::map<std::string,EVENTINT> messageMap;
    vector<responder*> responders;
    //responder* responders[USHRT_MAX];
    std::vector<message> que;

    std::vector<int> previousFrameIndexes;//you can use this to handle events from the main thread, instead of from an object

    std::vector<std::deque<message> > previousFrame;//[USHRT_MAX];

    message* getMessage(int which);
    message getAndHandleMessage(int which);
    message getAndHandleMessage(const std::string& which);
    void handleNextMessage(int which);
    void handleMessage(message* msg);

    int setMessageType(const std::string& name, EVENTINT which);
    EVENTINT getMessageType(const std::string& which);

    int clearMessageQueue();
    int clearMessageFrame();

    int clearResponders(EVENTINT which);
    int clearResponders(const std::string& which);

    int sendDelayed(const message& msg);//-1 indicates whatever the default is
    int sendRefNow(message& msg);//Does not include in frame, must call send to accomplish that
    int sendNow(const message& msg);//Does not include in frame, must call send to accomplish that
    int send(const message& msg, int includeInFrame = -1);

    int send(const ulti::element& msg, EVENTINT which, int includeInFrame = -1);
    int send(const ulti::element& msg, const std::string& which, int includeInFrame = -1);
    int sendDelayed(const ulti::element& msg, EVENTINT which);
    int sendDelayed(const ulti::element& msg, const std::string& which);

    void process(bool doClearMessageFrame = true);//Should Be Called When You Want To Process Items Immediately
    //call process(false) if you want to process items immediately but leave items in the previousFrame for another
    //process to handle, process true is meant to clear the current frame

    //EVENTINT getMessageType(const std::string& which);

    //void clear();

    int listen(responder&,EVENTINT which);
    int listen(responder&,const std::string& which);
    int listen(responder*, EVENTINT which);
    int listen(responder*, const std::string& which);

    int unlisten(responder&,EVENTINT which);
    int unlisten(responder&,const std::string& which);
    int unlisten(responder*, EVENTINT which);
    int unlisten(responder*, const std::string& which);

    responder* listenFunc(std::function<void(ulti::message& msg)> func, EVENTINT which);
    responder* listenFunc(std::function<void(ulti::message& msg)> func, const std::string& which);
    int unlistenFunc(responder* resp);

    responder* getFirstResponder(EVENTINT which);
    void setFirstResponder(EVENTINT which, responder* resp);

    eventStation();
    virtual ~eventStation();
};

//Required Parent For Any Objects To Receive Events
class responder{
public:
    std::function<void(ulti::message& msg)> callback;
    virtual int receive(message& msg);
    virtual int send(const message& msg);
    virtual int breakChain(EVENTINT which);
    virtual int breakChain(const std::string& which);
    virtual int linkLeft(EVENTINT which, responder* in);
    virtual int linkRight(EVENTINT which, responder* in);
    responder* getLeft(EVENTINT which);
    responder* getRight(EVENTINT which);
    virtual int becomeFirst(EVENTINT which);
    virtual int becomeFirst(const std::string& which);
    virtual int listen(EVENTINT which);
    virtual int listen(const std::string& which);
    virtual int unlisten(EVENTINT which);
    virtual int unlisten(const std::string& which);
    virtual int unlistenAll();
    virtual int receiveResponse(message& msg);//this is used by events to respond to events
    //in other words, if you request information through an event, this will respond with the data
    //you requested

    std::vector<EVENTINT> myRegistrations;
    eventStation* station;
    eventStation soloStation;
    //Here we choose between a static vector(more memory consumption) or a map(slower, but vastly better memory)
    //we could also choose a vector and dynamically resize it as needed (this may be the better option)
    std::vector<responder*> rightNode;
    std::vector<responder*> leftNode;
    responder();
    responder(std::function<void(ulti::message& msg)> func);
    virtual ~responder();//IMPORTANT!! CONNECT NEXT AND LAST!!
};

extern eventStation defaultStation;

class object; //see object.h

//DEPRACATED
struct event{
    float x,y,sx,sy;
    std::string type;
    std::string command;
    int value;
    unsigned int ID;
    void* extra;
    object* caller;
    const event&operator=(const event& invent);
    event();
    event(std::string intype);
};

//DEPRACATED
std::string eventToString(const event& evnt);
event stringToEvent(const std::string& str);

//DEPRACATED
class eventHandler: public object{
  protected:
      std::vector<event*> events;
      int curventnum;
      std::vector<event*>::iterator it;
  public:
    bool get_and_handle_event(event& evnt,std::string type);
    bool get_and_handle_event(event* evnt,std::string type);
    int handle_event(event* evnt);
    event* check_for_event(std::string type);
    bool finish_event(event* evnt);
    void framedone();
    eventHandler();
    ~eventHandler();
};

//unsigned int MAXIMUM_OBJECT_NUMBER=0;


/**
Basic event system for nearly anything. This was my first
real attempt. So some things may need to be adjusted for
efficiency's sake.

Used to send events between objects. Not very useful for
events from objects to main loop. But the event handler works
just fine for that purpose.
**/
//DEPRACATED
class eventSystem: public object{
  protected:
  std::map<std::string,int> types;
  std::vector<std::vector<object*> > listeners;
  std::vector<object*> first; //The first responder for an event, this allows fine tuned control over the event sys
  std::vector<object*> listenalls;
  std::vector<event> equeue;
  //object* tags[MAXIMUM_OBJECT_NUMBER]; //filled with void, use this to index objects extremely effeciently
  public:
  int listen_for_event(object& listener, const std::string& type);
  int listen_for_every_event(object& listener);
  void set_first(const std::string& type, object* obj);//different than other methods, because it can be null
  void send_event(event& evnt);
  void send_event(const std::string& evnt);
  void send_delayed_event(event& evnt);//Add a de-duped method
  void run_delayed_events();
  int  add_event_type(const std::string& type);
  void unlisten(object& listener);  //SLOW!!
  void unlisten(object& listener, const std::string& type);
  int  type_index(const std::string& type);
  int  handle_event(event* evnt);


};




}

//#include "ULTI_Object.h"

#endif
