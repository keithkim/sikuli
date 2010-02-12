#include <highgui.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <time.h>
#include <algorithm>

#include "finder.h"
#include "event-manager.h"

using namespace std;

void
SikuliEventManager::addObserver(int event_type, const char* param_image_filename, int handler_id, int x, int y, int w, int h){

  Observer* observer = new Observer(event_type,param_image_filename,handler_id,x,y,w,h);
  observers.push_back(observer);
};

SikuliEventManager::SikuliEventManager(){
  prev_screen_image = 0;
  debug_mode = false;
}

void
SikuliEventManager::setDebugMode(bool debug_mode){
  this->debug_mode = debug_mode ;
}

SikuliEventManager::~SikuliEventManager(){
  for (vector<Observer*>::iterator it = observers.begin(); it != observers.end(); it++){
    delete (*it);
  }
  if (prev_screen_image){
      cvReleaseImage(&prev_screen_image); 
  }    
}

vector<Event>
SikuliEventManager::update(const char* screen_image_filename){

  Match top_match;
  Finder f(screen_image_filename);
  f.debug(debug_mode);

  ChangeFinder cf(screen_image_filename);
  cf.debug(debug_mode);
  
  vector<Event> events;
  for (vector<Observer*>::iterator it = observers.begin(); it != observers.end(); it++){

    Observer& ob = *(*it);    
    
    switch (ob.event_type){

      case SIKULI_EVENT_APPEAR:

  
   
    f.setROI(ob.x,ob.y,ob.w,ob.h);
    f.find(ob.param_img);   

    top_match = f.next();


        if (top_match.score > 0.8){

          if (!ob.active){
            
            if (debug_mode)
              f.debug_show_image();

            Event e;
            e.type = ob.event_type;
            e.handler_id = ob.handler_id;
            e.x = top_match.x;
            e.y = top_match.y;
            e.h = top_match.h;
            e.w = top_match.w;
            events.push_back(e);
            ob.active = true;
          }

          
        }else{
          ob.active = false;
        } 
        break;

      case SIKULI_EVENT_VANISH:

  
      f.setROI(ob.x,ob.y,ob.w,ob.h);
      f.find(ob.param_img);   
    
      top_match = f.next();

        if (top_match.score < 0.8){

          if (!ob.active){
            
            if (debug_mode)
              f.debug_show_image();

            Event e;
            e.type = ob.event_type;
            e.handler_id = ob.handler_id;
            e.x = ob.x;
            e.y = ob.y;
            e.w = ob.w;
            e.h = ob.h;
            events.push_back(e);
            ob.active = true;
          }
        }else{
          ob.active = false;
        } 
        break;

      case SIKULI_EVENT_CHANGE:

  
        cf.setROI(ob.x,ob.y,ob.w,ob.h);
        if (prev_screen_image){
          cf.find(prev_screen_image);

          if (cf.hasNext()){


            if (debug_mode){
              while(cf.hasNext())
                cf.next();
              cf.debug_show_image();
            }

              Event e;
              e.type = ob.event_type;
              e.handler_id = ob.handler_id;
              e.x = ob.x;
              e.y = ob.y;
              e.w = ob.w;
              e.h = ob.h;
              events.push_back(e);
              ob.active = true;
          }

        }
        break;

    };    
  }

    if (prev_screen_image){
      cvReleaseImage(&prev_screen_image); 
      prev_screen_image = 0;
    }    
    prev_screen_image = cvLoadImage(screen_image_filename);
    

  return events;

};



Observer::Observer(int event_type_, const char* param_image_filename, int handler_id_, int x_, int y_, int w_, int h_){

  event_type = event_type_;

  param_img = 0;

  if (event_type_ != SIKULI_EVENT_CHANGE){
  // change events do not need image paramters
    param_img =  cvLoadImage(param_image_filename, CV_LOAD_IMAGE_COLOR );

    if (param_img==0){
      cerr << "can not load the image for the observer from:" << param_img << endl;
    }
  }

  x = x_;
  y = y_;
  w = w_;
  h = h_;
  handler_id = handler_id_;
  active = false;
}

Observer::~Observer(){
  if (param_img){
    cvReleaseImage(&param_img);
  }
}
