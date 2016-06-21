#ifndef ULTI_OBJECT_H
#define ULTI_OBJECT_H

#include <vector>
using std::vector;
#include <cstddef>


namespace ulti{

class eventSystem;
struct event;


/**
The base class of nearly every other class in the Ulti Library
Holds essential functions to be overridden by inherited classes.
**/
class object{
  protected:
  eventSystem* esys;
  int otype;
  public:
  unsigned long int tag; //4 bytes
  void set_event_manager(eventSystem* manager);
  virtual int submit_listens(eventSystem* manager);
  virtual int handle_event(event* evnt);
  virtual int class_type();
  int& type(); //I don't know wtf I was thinking here.
};


/**
Memory management class- so we can create new objects dynamically and
not worry about deleting them. Yay ram. Not effecient if you delete
objects frequently, but fine otherwise.
**/

class objectManager: public object{
    protected:
    vector<object*> objects;
    eventSystem* esys;

    vector<object*>::iterator it;

    unsigned long int curtag;

    public:

    vector<object*> get_vector();

    /**
    Function to create a new object, by default it will also pair it with the event system
    **/
    template<typename T>
    T& create_object(bool submitlistens = true){
        T* tmp = new(T);
        objects.push_back(tmp);
        if (esys!=NULL){
            objects.back()->set_event_manager(esys);
            objects.back()->submit_listens(esys);
            objects.back()->tag=curtag;
            curtag++;
        }
        return *tmp;
    }

    bool destroy_object(object* obj); //Exceptionally inefficient.
    bool clear();

    int size();

    object* get_object(int index);

    int handle_event(event* evnt);


    objectManager();
    ~objectManager();

};


}

#include "ULTI_Event.h"
#endif
