#include "ULTI_Event.h"

namespace ulti{

    eventStation defaultStation;

    MessageTypes messageTypes;
    int MessageTypes::getType(const std::string& tname){
        int* pret = ulti::mapget(smap,tname);
        int ret=0;
        if (pret!=NULL){
            ret=*pret;
            ret=smap[tname];
        }
        else{
            return registerType(tname);
        }
        return ret;
    }
    int MessageTypes::registerType(const std::string& tname){
        int* pret = ulti::mapget(smap,tname);
        if (pret==NULL){
            int ret=nextID;
            smap[tname]=ret;
            imap[ret]=tname;
            nextID++;
            return ret;
        }
        return -1;
    }
    int MessageTypes::registerType(const std::string& tname, int num){
        int* pret = ulti::mapget(smap,tname);
        std::string* sret = ulti::mapget(imap,num);
        if (pret==NULL&&sret==NULL){
            int ret=num;
            smap[tname]=ret;
            imap[ret]=tname;
            return ret;
        }
        return -1;
    }
    bool MessageTypes::isNumberRegistered(int num){
        std::string* pret = ulti::mapget(imap,num);
        return (pret!=NULL);
    }
    bool MessageTypes::isNameRegistered(std::string name){
        int* pret = ulti::mapget(smap,name);
        return (pret!=NULL);
    }
    std::string MessageTypes::getTypeName(int num){
        std::string* pret = ulti::mapget(imap,num);
        if (pret!=NULL){
            return *pret;
        }
        return "";
    }
    MessageTypes::MessageTypes(){
        nextID=2049;
    }

    int message::respond(const element& response){//use this to send a message back to parent
        if (parent!=NULL){
            ret=response;
            parent->receiveResponse(*this);
            return 1;
        }
        return 0;
    }
    bool message::operator==(const message& in){
        return (id=in.id&&type==in.type&&data==in.data&&parent==in.parent&&ret==in.ret);//NOTE: NOT CHECKING PROPAGATE
    }
    message& message::operator=(const message& in){
        if (this!=&in){
            id=in.id;
            type=in.type;
            data=in.data;
            value1=in.value1;
            value2=in.value2;
            handle=in.handle;
            parent=in.parent;
            propagate=in.propagate;
            preventDefault=in.preventDefault;
            ret=in.ret;
        }
        return *this;
    }

    message::message(){type=0;id=0;value1=value2=0;handle=0;parent=NULL;propagate=true;preventDefault=false;}
    message::message(const message& in){*this=in;}
    message::message(const ulti::element& in,int itype,responder* iparent):message(){data=in;type=itype;parent=iparent;}
    message::message(const ulti::element& in,int itype, int iid, responder* iparent){data=in;type=itype;id=iid;parent=iparent;}
    int responder::receive(message& msg){
        if (callback!=NULL){
            callback(msg);
            return 1;
        }
        return 0;
    }
    int responder::send(const message& msg){if (station!=NULL){station->send(msg);return 1;}return -1;}
    int responder::breakChain(EVENTINT which){
        if (which>=0&&(rightNode.size()>which||leftNode.size()>which)){
            if (rightNode[which]!=NULL)
                rightNode[which]->linkLeft(which,leftNode[which]);
            if (leftNode[which]!=NULL){
                leftNode[which]->linkRight(which,rightNode[which]);
            }
            if (station!=NULL){
                if (station->getFirstResponder(which)==this){
                    station->setFirstResponder(which,rightNode[which]);
                }
                else{
                    void* tmp = (void*) station;
                }
            }
            rightNode[which]=NULL;
            leftNode[which]=NULL;
            return 1;
        }
        return -1;
    }
    int responder::breakChain(const std::string& which){
        if (station==NULL)
            return -2;
        return breakChain(station->getMessageType(which));
    }
    int responder::linkLeft(EVENTINT which, responder* in){
        if (which>=0){
            while (leftNode.size()<=which){
                leftNode.push_back(NULL);
                rightNode.push_back(NULL);
            }
            leftNode[which]=in;
            return 1;
        }
        return -1;
    }
    int responder::linkRight(EVENTINT which, responder* in){
        if (which>=0){
            while (rightNode.size()<=which){
                leftNode.push_back(NULL);
                rightNode.push_back(NULL);
            }
            rightNode[which]=in;
            return 1;
        }
        return -1;
    }
    int responder::becomeFirst(EVENTINT which){
        if (leftNode.size()<=which)
            return -1;
        responder* prev = getLeft(which);
        if (prev==NULL)
            return 0;
        breakChain(which);
        responder* cur=prev;
        while(cur!=NULL){//CYCLICAL LISTS WILL INIFINITE LOOP
            prev=cur;
            //POSSIBLY DO CHECK FOR THIS == prev
            cur=cur->getLeft(which);
        }
        prev->linkLeft(which,this);
        linkRight(which,prev);
        if (station!=NULL)//THIS DOES NOT VERIFY THAT THE OLD FIRST RESPONDER WAS PREV (IT SHOULD ALWAYS BE)
            station->setFirstResponder(which,this);
        return 1;
    }
    int responder::becomeFirst(const std::string& which){
        if (station==NULL)
            return -1;
        return becomeFirst(station->getMessageType(which));
    }
    int responder::listen(EVENTINT which){
        if (station==NULL)
            return -1;
        station->listen(*this,which);
    }
    int responder::listen(const std::string& which){
        if (station==NULL)
            return -1;
        return listen(station->getMessageType(which));
    }
    int responder::unlisten(EVENTINT which){
        if (breakChain(which)==1){//this fixes first responder
            return 1;
        }
        return -1;
    }
    int responder::unlisten(const std::string& which){
        if (station==NULL)
            return -1;
        return unlisten(station->getMessageType(which));
    }
    int responder::unlistenAll(){
        for(int a = 0; a < USHRT_MAX; a++){
            unlisten(a);
        }
		return 1;
    }
    int responder::receiveResponse(message& msg){
        return -1;
    }

    responder* responder::getLeft(EVENTINT which){if (leftNode.size()>which){return leftNode[which];}return NULL;}
    responder* responder::getRight(EVENTINT which){if (rightNode.size()>which){return rightNode[which];}return NULL;}

    responder::responder(){
        station=&defaultStation;
        eventStation** ttmp = &station;
        if ((int)station==0xbaadf00d)
            station=NULL;
    }
    responder::responder(std::function<void(ulti::message& msg)> func){
        callback=func;
    }
    responder::~responder(){//IMPORTANT!! CONNECT NEXT AND LAST!!
        for(EVENTINT a = 0; a < rightNode.size(); a++){
            unlisten(a);
        }
    }


    message* eventStation::getMessage(int which){if(previousFrame[which].size()>0){return &previousFrame[which].front();}return NULL;}
	message eventStation::getAndHandleMessage(int which) { message* pret = getMessage(which);if (pret==NULL){return message();} message ret = *pret; handleNextMessage(which); return ret; }
    message eventStation::getAndHandleMessage(const std::string& which){return getAndHandleMessage(getMessageType(which));}
    void eventStation::handleNextMessage(int which){if (previousFrame[which].size()>0){previousFrame[which].pop_front();}}
    void eventStation::handleMessage(message* msg){if (msg==NULL)return;
        if (previousFrame[msg->type].size()>0){
            for(int a = 0; a < previousFrame[msg->type].size();a++){
                if (&previousFrame[msg->type][a]==msg){
                    previousFrame[msg->type].erase(previousFrame[msg->type].begin()+a);
                    return;
                }
            }
        }
    }

    EVENTINT eventStation::getMessageType(const std::string& which){
        return messageTypes.getType(which);
    }

    int eventStation::setMessageType(const std::string& name, EVENTINT which){
        return messageTypes.registerType(name,which);
    }

    int eventStation::clearMessageQueue(){
        que.clear();
		return 1;
    }
    int eventStation::clearMessageFrame(){
        for(int a = 0;a < previousFrameIndexes.size(); a++){
            previousFrame[previousFrameIndexes[a]].clear();
        }
        previousFrameIndexes.clear();
		return 1;
    }

    int eventStation::clearResponders(EVENTINT which){
        responder* first = getFirstResponder(which);
        while(first!=NULL){
            responder* next = first->getRight(which);
            first->breakChain(which);
            first=next;
        }
        return 1;
    }
    int eventStation::clearResponders(const std::string& which){
        return clearResponders(getMessageType(which));
    }

    int eventStation::sendDelayed(const message& msg){
        que.push_back(msg);
        return 1;
    }
    int eventStation::sendRefNow(message& msg){
        responder* nxt = getFirstResponder(msg.type);
        while(nxt!=NULL){
            nxt->receive(msg);
            if (msg.propagate){
                nxt=nxt->getRight(msg.type);
            }
            else{
                break;
            }
        }
        return 1;
    }
    int eventStation::sendNow(const message& msg){
        message tmsg = msg;
        return sendRefNow(tmsg);
    }
    int eventStation::send(const message& msg, int includeInFrame){
        sendNow(msg);

        if (includeInFrame==1||(includeInFrame==-1&&frameMessagesByDefault)){
            if (find(previousFrameIndexes.begin(),previousFrameIndexes.end(),msg.type)==previousFrameIndexes.end())
                previousFrameIndexes.push_back(msg.type);
            previousFrame[msg.type].push_back(msg);
        }
        return 1;
    }

    int eventStation::send(const ulti::element& msg, EVENTINT which, int includeInFrame){
        return send(message(msg,which,NULL),includeInFrame);
    }
    int eventStation::send(const ulti::element& msg, const std::string& which, int includeInFrame){
        return send(msg,getMessageType(which),includeInFrame);
    }
    int eventStation::sendDelayed(const ulti::element& msg, EVENTINT which){
        return sendDelayed(message(msg,which,NULL));
    }
    int eventStation::sendDelayed(const ulti::element& msg, const std::string& which){
        return sendDelayed(msg,getMessageType(which));
    }

    void eventStation::process(bool doClearMessageFrame){
        for(unsigned int a = 0; a < que.size(); a++){
            send(que[a]);
        }
        if (doClearMessageFrame){
            clearMessageFrame();
        }
    }

    int eventStation::listen(responder& in,EVENTINT which){
        return listen(&in,which);
    }
    int eventStation::listen(responder& in,const std::string& which){
        return listen(in,getMessageType(which));
    }
    int eventStation::listen(responder* in, EVENTINT which){
        if (responders.size()<=which)
            responders.resize(which+1,NULL);
        responder* first = responders[which];
        if (first==NULL){
            setFirstResponder(which,in);
            return 1;
        }
        while(first->getRight(which)!=NULL){
            first = first->getRight(which);
        }
        first->linkRight(which,in);
        in->linkLeft(which,first);
        return 1;
    }
    int eventStation::listen(responder* in, const std::string& which){
        return listen(in,getMessageType(which));
    }
    responder* eventStation::listenFunc(std::function<void(ulti::message& msg)> func, EVENTINT which){
        responder* spond = new responder(func);
        childrenToDelete.push_back(spond);
        eventStation::listen(spond,which);
        return spond;
    }
    responder* eventStation::listenFunc(std::function<void(ulti::message& msg)> func, const std::string& which){
        return eventStation::listenFunc(func,getMessageType(which));
    }
    int eventStation::unlistenFunc(responder* resp){
        for(int a = 0; a < childrenToDelete.size(); a++){
            if (childrenToDelete[a]==resp){
                childrenToDelete[a]->unlistenAll();
                delete(childrenToDelete[a]);
                childrenToDelete.erase(childrenToDelete.begin() + a);
                return 1;
            }
        }
        return -1;
    }

    int eventStation::unlisten(responder& in,EVENTINT which){
        return unlisten(&in,which);
    }
    int eventStation::unlisten(responder& in,const std::string& which){
        return unlisten(in,getMessageType(which));
    }
    int eventStation::unlisten(responder* in, EVENTINT which){
        return in->unlisten(which);
    }
    int eventStation::unlisten(responder* in, const std::string& which){
        return unlisten(in,getMessageType(which));
    }

    responder* eventStation::getFirstResponder(EVENTINT which){
        if (which>=responders.size())
            return NULL;
        return responders[which];
    }
    void eventStation::setFirstResponder(EVENTINT which, responder* resp){
        responders[which]=resp;
    }

    eventStation::eventStation(){
        //nextMessageType=0;
        frameMessagesByDefault=false;
        //for(int a = 0; a < responders.size(); a++){
        //    responders[a]=NULL;
        //}
    }
    eventStation::~eventStation(){
        responders.clear();
        for(int a = childrenToDelete.size()-1; a >=0; a--){
            childrenToDelete[a]->station=NULL;
            delete(childrenToDelete[a]);
        }
        childrenToDelete.clear();
    }




  int  eventSystem::type_index(const std::string& type){
      if (types.count(type))
        return (*types.find(type)).second;
      types[type]=listeners.size();
      listeners.push_back(std::vector<object*>());
      first.push_back(NULL);
      return listeners.size()-1;
  }
  int eventSystem::listen_for_event(object& listener, const std::string& type){
      int itype = type_index(type);
      std::vector<object*>::iterator it = std::find(listeners[itype].begin(),listeners[itype].end(),&listener);
      if (it==listeners[itype].end()){
        listeners[itype].push_back(&listener);
        return 1;
      }
      else{
        std::swap(listeners[itype].back(),*it);
      }
      return 0;
  }
  void eventSystem::set_first(const std::string& type, object* obj){
        int itype=type_index(type);
        first[itype]=obj;
  }
  int eventSystem::listen_for_every_event(object& listener){
      std::vector<object*>::iterator it = std::find(listenalls.begin(),listenalls.end(),&listener);
      if (it==listenalls.end()){
        listenalls.push_back(&listener);
        return 1;
      }
      return 0;
  }
  void eventSystem::send_event(event& evnt){
      int itype = type_index(evnt.type);
      if (first[itype]!=NULL){
            if (first[itype]->handle_event(&evnt)<=0){
                return;
            }
      }
      for (uint32_t a = 0; a < listeners[itype].size(); a++){
          if (listeners[itype][a]->handle_event(&evnt)<=0){
            return;
          }
        }
      for (uint32_t a = 0; a < listenalls.size(); a++){
          if (listenalls[a]->handle_event(&evnt)<=0){
            return;
          }
      }
  }
  void eventSystem::send_event(const std::string& evnt){
       event tnt;
       tnt.type=evnt;
       send_event(tnt);
  }
  void eventSystem::send_delayed_event(event& evnt){
        equeue.push_back(evnt);
  }

  void eventSystem::run_delayed_events(){
      for (uint32_t a = 0; a < equeue.size(); a++){
            send_event(equeue[a]);
      }
      equeue.clear();
  }
  int eventSystem::add_event_type(const std::string& type){
      return type_index(type);
  }
  void eventSystem::unlisten(object& listener){
      std::vector<object*>::iterator it;
      for (uint32_t a = 0; a < listeners.size(); a++){
         it = std::find(listeners[a].begin(),listeners[a].end(),&listener);
         if (it!=listeners[a].end()){
            listeners[a].erase(it);
         }
      }
      it = std::find(listenalls.begin(),listenalls.end(),&listener);
      if (it!=listenalls.end())
        listenalls.erase(it);
  }
  void eventSystem::unlisten(object& listener, const std::string& type){
      int itype = type_index(type);
      std::vector<object*>::iterator it = std::find(listeners[itype].begin(),listeners[itype].end(),&listener);
      if ((it!=listeners[itype].end()))
            listeners[itype].erase(it); //Not sure if this needs to be begin()+it
  }
  int eventSystem::handle_event(event* evnt){
        if (evnt->type=="delete"){
            unlisten(*(object*)evnt->extra);
            return 1;
        }
        return 0;
    }



    int eventHandler::handle_event(event* evnt){
        if (evnt==NULL)
            return -1;
        if (curventnum>=events.size()){
            events.push_back(new event);
        }

        (*events[curventnum])= (*evnt);
        curventnum++;


        /*if (curventnum+1<events.size()){
             (*events[curventnum])= (*evnt);
        }
        else{
            events.push_back(new event);
            (*events.back())= (*evnt);
        }
        curventnum++;*/
        return 1;
    }
    event* eventHandler::check_for_event(std::string type){
        if (type==""){
         if (curventnum>0){
              return events[0];
         }
         else
              return NULL;
        }
        for (int a = 0; a < curventnum; a++){
                if (events[a]->type==type)
                    return events[a];
        }
        return NULL;
    }
    bool eventHandler::finish_event(event* evnt){
        if (evnt==NULL)
            return -1;
        it=find(events.begin(),events.end(),evnt);
        if (it!=events.end()){
         std::swap(*it,events[curventnum-1]);
         curventnum--;
         return true;
        }
        return false;
    }
    void eventHandler::framedone(){
        curventnum=0;
    }
    eventHandler::eventHandler(){
        curventnum=0;
    }
    eventHandler::~eventHandler(){
        for(int a = 0; a < events.size(); a++){
         delete(events[a]);
        }
    }

    bool eventHandler::get_and_handle_event(event& evnt, std::string type){
        event* tvnt = check_for_event(type);
        if (tvnt==NULL)
            return false;
        evnt=*tvnt;
        finish_event(tvnt);
        return true;
    }

    bool eventHandler::get_and_handle_event(event* evnt, std::string type){
        event* tvnt = check_for_event(type);
        if (tvnt==NULL)
            return false;
        *evnt=*tvnt;
        finish_event(tvnt);
        return true;
    }

  const event& event::operator=(const event& invent){
    if (this != &invent){
    x=invent.x;
    y=invent.y;
    sx=invent.sx;
    sy=invent.sy;
    command=invent.command;
    type=invent.type;
    extra=invent.extra;
    caller=invent.caller;
    value=invent.value;
    ID=invent.ID;
    }
    return *this;
  }
  event::event(std::string intype){
      type=intype;
  }
  event::event(){}

  std::string eventToString(const event& evnt){
      std::stringstream ss;
      ss << evnt.x << ',' << evnt.y << ',' << evnt.sx << ',' << evnt.sy << ',' << evnt.command
         << ',' << evnt.type << ',' << evnt.value << ',' << evnt.ID;
      return ss.str();
  }
  event stringToEvent(const std::string& str){
      event ret;
      int cur = 0;
      for (int a = 0, b = 0; a < str.size()+1; a++){
          if (a == str.size() || str[a]==','){
                std::stringstream ss;
                ss << str.substr(cur,a-cur);

                if (b==0)
                    ss >> ret.x;
                else if (b==1)
                    ss >> ret.y;
                else if (b==2)
                    ss >> ret.sx;
                else if (b==3)
                    ss >> ret.sy;
                else if (b==4)
                    ret.command=ss.str();
                else if (b==5)
                    ret.type = ss.str();
                else if (b==6)
                    ss >> ret.value;
                else if (b==7)
                    ss >> ret.ID;

                b++;
                cur=a+1;
          }

      }
      return ret;
  }

}
