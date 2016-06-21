#include "ULTI_Object.h"

namespace ulti{

  void object::set_event_manager(eventSystem* manager){
    esys = manager;
  }
  int object::submit_listens(eventSystem* manager){
    return 0;
  }
  int object::handle_event(event* evnt){
    return 0;
  }
  int& object::type(){
    return otype;
  }
  int object::class_type(){
   return 0;
  }


    vector<object*> objectManager::get_vector(){
        return objects;
    }

    bool objectManager::destroy_object(object* obj){
        it=find(objects.begin(),objects.end(),obj);
        if (it!=objects.end()){
          std::swap(*it,objects.back());
          delete(objects.back());
          objects.pop_back();
          return true;
        }
        return false;
    }

    bool objectManager::clear(){
        for (int a = 0; a < objects.size(); a++){
         delete(objects[a]);
        }
        objects.clear();
		return true;
    }

    int objectManager::handle_event(event* evnt){
        if (evnt->type=="delete"){
            destroy_object((object*)evnt->extra);
        }
        return 1;
    }

    int objectManager::size(){
        return objects.size();
    }

    object* objectManager::get_object(int index){
        if (index<0||index>=objects.size())
            return NULL;
        return objects[index];
    }
    objectManager::objectManager(){
        esys=NULL;
        curtag=0;
    }
    objectManager::~objectManager(){
        for (uint16_t a = 0; a < objects.size(); a++){
         delete(objects[a]);
         objects[a]=NULL;
        }
    }

}
