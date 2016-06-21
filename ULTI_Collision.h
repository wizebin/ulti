#ifndef ULTI_COLLISION_H
#define ULTI_COLLISION_H

#include "ULTI_Object.h"
#include "ULTI_Graphics.h"
#include <algorithm>
#include <math.h>

namespace ulti{

/**
Currently includes a basic aabb collision test function and a "vecmap"
the vecmap is meant to have an extendable tile system. But it should be
changed into a chunk system so it doesn't take up gigabytes of unnecessary data.
**/

bool collide(sprite& spa,sprite& spb);
bool collide(sprite* spa,sprite* spb);

template<typename T>
class vecmap{
    protected:
    vector<vector<T*> > q1; //++
    vector<vector<T*> > q2; //-+
    vector<vector<T*> > q3; //--
    vector<vector<T*> > q4; //+-
    typedef typename std::vector<T*>::iterator titerator;
    titerator it;
    public:

    T* getthing(icoord pos){
        if (pos.x<0){
            pos.x*=-1;
            pos.x-=1;
         if (pos.y<0){
             pos.y*=-1;
             pos.y-=1;
            if (q3.size()>pos.x){
             if (q3[pos.x].size()>pos.y){
              return q3[pos.x][pos.y];
             }
            }
         }
         else{//pos.y>=0
            if (q2.size()>pos.x){
             if (q2[pos.x].size()>pos.y){
              return q2[pos.x][pos.y];
             }
            }
         }
        }
        else{//pos.x>=0
          if (pos.y<0){
            pos.y*=-1;
            pos.y-=1;
            if (q4.size()>pos.x){
             if (q4[pos.x].size()>pos.y){
              return q4[pos.x][pos.y];
             }
            }
          }
          else{//pos.y>=0
             if (q1.size()>pos.x){
             if (q1[pos.x].size()>pos.y){
              return q1[pos.x][pos.y];
             }
            }
          }
        }
        return NULL;
    }


    /*T& getrefthing(icoord pos){
        T* tmp = getthing(pos);
        if (tmp == NULL)
            return T;
        return *tmp;
    }*/
    void setthing(T& thing, icoord pos){
        setthing(&thing,pos);
    }

    void setthing(T* thing, icoord pos){
        //vector<T*>::iterator it;
        if (pos.x<0){
            pos.x*=-1;
            pos.x-=1;
         if (pos.y<0){
             pos.y*=-1;
             pos.y-=1;
            if (q3.size()>pos.x){
                if (q3[pos.x].size()>pos.y){
                    q3[pos.x][pos.y] = thing;
                }
                else{
                    q3[pos.x].resize(pos.y+1);
                    q3[pos.x][pos.y] = thing;
                }
            }
            else{
             q3.resize(pos.x+1);
             q3[pos.x].resize(pos.y+1);
             q3[pos.x][pos.y] = thing;

            }
         }
         else{//pos.y>=0
            if (q2.size()>pos.x){
                if (q2[pos.x].size()>pos.y){
                    q2[pos.x][pos.y] = thing;
                }
                else{
                    q2[pos.x].resize(pos.y+1);
                    q2[pos.x][pos.y] = thing;
                }
            }
            else{
             q2.resize(pos.x+1);
             q2[pos.x].resize(pos.y+1);
             q2[pos.x][pos.y] = thing;

            }
         }
        }
        else{//pos.x>=0
          if (pos.y<0){
            pos.y*=-1;
            pos.y-=1;
            if (q4.size()>pos.x){
                if (q4[pos.x].size()>pos.y){
                    q4[pos.x][pos.y] = thing;
                }
                else{
                    q4[pos.x].resize(pos.y+1);
                    q4[pos.x][pos.y] = thing;
                }
            }
            else{
             q4.resize(pos.x+1);
             q4[pos.x].resize(pos.y+1);
             q4[pos.x][pos.y] = thing;

            }
          }
          else{//pos.y>=0
             if (q1.size()>pos.x){
                if (q1[pos.x].size()>pos.y){
                    q1[pos.x][pos.y] = thing;
                }
                else{
                    q1[pos.x].resize(pos.y+1);
                    q1[pos.x][pos.y] = thing;
                }
            }
            else{
             q1.resize(pos.x+1);
             q1[pos.x].resize(pos.y+1);
             q1[pos.x][pos.y] = thing;

            }
          }
        }
    }

    vecmap(){

    }
    vecmap(int r1, int r2, int r3, int r4){
        q1.resize(r1);
        for (int a = 0; a < r1; a++){
         q1[a].resize(r1);
        }
        q2.resize(r2);
        for (int a = 0; a < r2; a++){
         q2[a].resize(r2);
        }
        q3.resize(r3);
        for (int a = 0; a < r3; a++){
         q3[a].resize(r3);
        }
        q4.resize(r4);
        for (int a = 0; a < r4; a++){
         q4[a].resize(r4);
        }
    }



};


/*class collisionsystem: public object{
  protected:
  vecmap<vector<sprite*> > vmap;
  vector<sprite*>::iterator sit;
  //vector<sprite*> toupdate; //will send messages to all objects in this queue as well as objects it collides with
  int tilesize;
  vector<sprite*>* vpt;

  vector<sprite*> stoupdate;

  vector<icoord> toupdate;
  vector<icoord>::iterator cit;

  vector<vector<sprite*>* > todelete;

  public:
  icoord getcolpos(sprite* spr);
  icoord getcolpos(coord cor);
  icoord getcolpos(icoord cor);

  void remobject(sprite* spr);
  void updobject(sprite* spr);

  void fastadd(sprite* spr);
  void fastupd(sprite* spr);

  void professionaladd(sprite* spr); //Be extremely careful with this method

  void remfromfastupd(sprite* spr);

  vector<sprite*> collideswith(coord pos, coord size);
  vector<sprite*> roughcollide(coord pos, coord size);

  vector<sprite*>* getvec(icoord pos);

  int update();
  void updatefromsort();
  int updatefromsprite();

  int handle_event(event* evnt);

  ~collisionsystem();
  collisionsystem();
  collisionsystem(int tsize);

};

*/
/*
struct tosort{
int index;
int length;
};

struct stosort{
sprite* spr;
int length;
};

bool sorter(tosort* first, tosort* second);
int distanceBetweenObjects(sprite* first, sprite* second);
int distanceBetweenObjects(sprite& first, sprite& second);

struct ssorter{
    sprite *host;
    ssorter(sprite *host) : host(host) { }
    bool operator()(sprite* first, sprite* second);
};
*/


}
#endif
