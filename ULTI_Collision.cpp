#include "ULTI_Collision.h"

namespace ulti{

bool collide (sprite* o1, sprite* o2){
    /*int px=o1->getpos().x;
    int py=o1->getpos().y;
    int sx=o1->getsize().x;
    int sy=o1->getsize().y;

    int px1=o2->getpos().x;
    int py1=o2->getpos().y;
    int sx1=o2->getsize().x;
    int sy1=o2->getsize().y;

    return collide(o1->getpos().x,o1->getpos().y,o1->getsize().x,o1->getsize().y,o2->getpos().x,o2->getpos().y,o2->getsize().x,o2->getsize().y);*/
    return (o1->getpos().x,o1->getpos().y,o1->width(),o1->height(),o2->getpos().x,o2->getpos().y,o2->width(),o2->height());
}
bool collide(sprite& spa,sprite& spb){
    return (spa.getpos().x,spa.getpos().y,spa.width(),spa.height(),spb.getpos().x,spb.getpos().y,spb.width(),spb.height());
}

int distanceBetweenObjects(sprite* first, sprite* second){
 return(((first->getpos().x+first->width()/2) - (second->getpos().x+second->width()/2))*((first->getpos().x+first->width()/2) - (second->getpos().x+second->width()/2))  +
        ((first->getpos().y+first->height()/2) - (second->getpos().y+second->height()/2))*((first->getpos().y+first->height()/2) - (second->getpos().y+second->height()/2)));
}
int distanceBetweenObjects(sprite& first, sprite& second){
 return distanceBetweenObjects(&first,&second);
}

/*
 collisionsystem::collisionsystem(int tsize){
  tilesize=tsize;
 }
 collisionsystem::collisionsystem(){
  tilesize=33;
  }
  collisionsystem::~collisionsystem(){
   for (int a = 0; a < todelete.size(); a++){
    delete(todelete[a]);
   }
  }

  icoord collisionsystem::getcolpos(sprite* spr){
      return icoord(spr->getpos().x/tilesize,spr->getpos().y/tilesize);
  }
  icoord collisionsystem::getcolpos(coord cor){
      return icoord(cor.x/tilesize,cor.y/tilesize);
  }
  icoord collisionsystem::getcolpos(icoord cor){
      return icoord(cor.x/tilesize,cor.y/tilesize);
  }

  int collisionsystem::handle_event(event* evnt){
        if (evnt->type=="delete"){
            remobject((sprite*)evnt->extra);
            remfromfastupd((sprite*)evnt->extra);
        }
        else if (evnt->type=="movement"){
            if (evnt->caller!=NULL)
                updobject((sprite*)evnt->caller);
        }
    }

  void collisionsystem::professionaladd(sprite* spr){
     if (spr==NULL)
        return;
      icoord pos=getcolpos(spr->getpos());
      icoord epos=getcolpos(icoord(spr->getpos().x+spr->getsize().x,spr->getpos().y+spr->getsize().y));


      for (int a = pos.x; a <= epos.x; a++){
          for (int b = pos.y; b <= epos.y; b++){
              vpt = vmap.getthing(icoord(a,b));
              if (vpt==NULL){
                vpt = new vector<sprite*>;
                vmap.setthing(vpt,icoord(a,b));
                todelete.push_back(vpt);
              }
              ///REMOVING CHECK FOR SPEED's SAKE
              //sit = find(vpt->begin(),vpt->end(), spr);
              //if (sit == vpt->end()){
               vpt->push_back(spr);
              //}
              if (vpt->size()>1){

               sit=find(stoupdate.begin(),stoupdate.end(),spr);
                if (sit==stoupdate.end())
                    stoupdate.push_back(spr);

              }
          }
      }
      spr->collisionprev(true);//updates prev pos values
  }

  void collisionsystem::fastadd(sprite* spr){

    if (spr==NULL)
        return;
      icoord pos=getcolpos(spr->getpos()); //6 calls
      icoord epos=getcolpos(icoord(spr->getpos().x+spr->getsize().x,spr->getpos().y+spr->getsize().y));
      //6calls

      for (int a = pos.x; a <= epos.x; a++){
          for (int b = pos.y; b <= epos.y; b++){
              vpt = vmap.getthing(icoord(a,b));//10 calls
              if (vpt==NULL){
                vpt = new vector<sprite*>;
                vmap.setthing(vpt,icoord(a,b));
                todelete.push_back(vpt);
              }
              ///CHECK TIME START HERE
              sit = find(vpt->begin(),vpt->end(), spr); //25 calls
              if (sit == vpt->end()){
               vpt->push_back(spr);
              }
              ///END CHECK TIME
              if (vpt->size()>1){

               sit=find(stoupdate.begin(),stoupdate.end(),spr);
                if (sit==stoupdate.end())
                    stoupdate.push_back(spr);

              }
          }
      }
      spr->collisionprev(true);//updates prev pos values
  }
  void collisionsystem::remobject(sprite* spr){
      if (spr==NULL)
        return;
      icoord pos=getcolpos(spr->collisionprev());
      icoord epos=getcolpos(icoord(spr->collisionprev().x+spr->getsize().x,spr->collisionprev().y+spr->getsize().y));

      for (int a = pos.x; a <= epos.x; a++){
          for (int b = pos.y; b <= epos.y; b++){
              vpt = vmap.getthing(icoord(a,b));
              if (vpt!=NULL){
                  sit = find(vpt->begin(),vpt->end(), spr);
                  if (sit != vpt->end()){
                   //std::swap(*sit,vpt->back());
                   //vpt->pop_back();
                   vpt->erase(sit); ///Change to swap back and pop?
                  }

              }
          }}

  }
  void collisionsystem::updobject(sprite* spr){ //yikes, change this please
      //if (spr->collisionprev()!=spr->getpos()){
        remobject(spr);
        //addobject(spr);
        fastadd(spr);
      //}
  }
  void collisionsystem::fastupd(sprite* spr){
      remobject(spr);
      fastadd(spr);
  }

  vector<sprite*>* collisionsystem::getvec(icoord pos){
      return vmap.getthing(pos);
  }

  vector<sprite*> collisionsystem::collideswith(coord ipos, coord size){
    vector<sprite*> ret;

      icoord pos=getcolpos(ipos);
      icoord epos=getcolpos(icoord(ipos.x+size.x,ipos.y+size.y));

      for (int a = pos.x; a <= epos.x; a++){
          for (int b = pos.y; b <= epos.y; b++){
              vpt = vmap.getthing(icoord(a,b));
              if (vpt!=NULL){
                  if (vpt->size()>0){
                   for (int c = 0; c < vpt->size(); c++){
                    if (collide(ipos.x,ipos.y,size.x,size.y,(*vpt)[c]->getpos().x,(*vpt)[c]->getpos().y,(*vpt)[c]->getsize().x,(*vpt)[c]->getsize().y)){
                         sit=find(ret.begin(),ret.end(),(*vpt)[c]);
                         if (sit==ret.end()){
                          ret.push_back((*vpt)[c]);
                         }
                    }
                   }
                  }
              }
          }
      }

    return ret;
  }

  vector<sprite*> collisionsystem::roughcollide(coord ipos, coord size){
      vector<sprite*> ret;

      icoord pos=getcolpos(ipos);
      icoord epos=getcolpos(icoord(ipos.x+size.x,ipos.y+size.y));

      for (int a = pos.x; a <= epos.x; a++){
          for (int b = pos.y; b <= epos.y; b++){
              vpt = vmap.getthing(icoord(a,b));
              if (vpt!=NULL){
                  if (vpt->size()>0){
                   for (int c = 0; c < vpt->size(); c++){
                    //if (collide(ipos.x,ipos.y,size.x,size.y,(*vpt)[c]->getpos().x,(*vpt)[c]->getpos().y,(*vpt)[c]->getsize().x,(*vpt)[c]->getsize().y)){
                         sit=find(ret.begin(),ret.end(),(*vpt)[c]);
                         if (sit==ret.end()){
                          ret.push_back((*vpt)[c]);
                         }
                    //}
                   }
                  }
              }
          }
      }

    return ret;
  }

  void collisionsystem::remfromfastupd(sprite* spr){
      sit=find(stoupdate.begin(),stoupdate.end(),spr);
      if (sit!=stoupdate.end()){

            //std::swap(*sit,stoupdate.back());
            //stoupdate.pop_back();
            stoupdate.erase(sit);

      }
  }

  int collisionsystem::update(){
      event tmp;
      tmp.command="update";
      tmp.type="collision";
      tmp.caller=this;
      for (int a = 0; a < stoupdate.size(); a++){
       stoupdate[a]->handle_event(&tmp); ///This object's responsibility to notify other objects
      }
      int ret = stoupdate.size();
      stoupdate.clear();
      return ret;
  }


  void collisionsystem::updatefromsort(){
      event tmp;
      tmp.command="update";
      tmp.type="collision2";
      tmp.caller=this;

      ///I would really like to gather all objects together before calling handle_event...
      ///I just can't think of a good way to do it.

      for (int a = 0; a < toupdate.size(); a++){
        vpt = vmap.getthing(toupdate[a]);
        for (int b = 0; b < vpt->size(); b++){

            sort(vpt->begin(), vpt->end(),ssorter((*vpt)[b]));

            tmp.extra=vpt;
            sprite* stmp = (*vpt)[b];
            stmp->handle_event(&tmp);


        }
      }
      toupdate.clear();

      ///We could send an update event, and send a collision event before, then have the objects
      ///store a vector of things to check?, this way we can recursively call this update function until it returns no collisions.


  }



*/
/*
bool sorter(tosort* first, tosort* second){
 return first->length<second->length;
}


    bool ssorter::operator()(sprite* first, sprite* second){
      if (first==NULL||second==NULL||first->getdead()||second->getdead())
        return false;
      return distanceBetweenObjects (host,first) < distanceBetweenObjects (host,second);
    }*/



}
