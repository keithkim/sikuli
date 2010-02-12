#include <cv.h>
#include <cxcore.h>
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

string screen_image_path(int screen_i){
  stringstream ss;
  ss << "testdata/images/"  << screen_i << "-screen.png";
  return ss.str();
}

string target_image_path(int screen_i, int target_i){
  stringstream ss;
  ss << "testdata/images/"  << screen_i << "-target-" << target_i<< ".png";
  return ss.str();
}

string result_image_path(int screen_i, int target_i, string testname){
  stringstream ss;
  ss << "testdata/results/" << screen_i << "-[" << testname << "]-target-" << target_i<< ".png";
  return ss.str();
}

void test_face_finder(){


  FaceFinder f("testdata/faces.jpg");
  f.debug(true);


  f.setROI(0,0,f.get_screen_width()/2,f.get_screen_height());
  f.find();
  while (f.hasNext()){
    Match m = f.next();
    cout << "Face: " << m.x << "," << m.y << "," << m.w << "," << m.h << endl;
  }
  f.debug_show_image();

}

void test_sem(){

  SikuliEventManager sem;
  sem.setDebugMode(true);
  sem.addObserver(SIKULI_EVENT_APPEAR, "testdata/advanced.png", 1, 450, 300, 400, 400);
  sem.addObserver(SIKULI_EVENT_VANISH, "testdata/advanced.png", 2, 450, 300, 400, 400);
  sem.addObserver(SIKULI_EVENT_CHANGE, "", 3, 450, 300, 400, 400);
  sem.addObserver(SIKULI_EVENT_APPEAR, "testdata/firewire.png", 4, 100, 150, 250, 400);
  sem.addObserver(SIKULI_EVENT_VANISH, "testdata/firewire.png", 4, 100, 150, 250, 400);


  vector<Event> events;

  char buf[50];
  for (int i=1;i<=20;++i){

    cout << "frame " << i << " =======================" << endl;
    sprintf(buf,"testdata/frames/%d.png",i);


    events = sem.update(buf);  

    for (vector<Event>::iterator it = events.begin(); it != events.end(); it++){


      if (it->type == SIKULI_EVENT_APPEAR){
        cout << "APPEAR";
      }
      else if (it->type == SIKULI_EVENT_VANISH){
        cout << "VANISH";
      }else if (it->type == SIKULI_EVENT_CHANGE){
        cout << "CHANGE";
      }

      cout << " @ " << "(" << it->x << " , " << it->y <<  " , " << it->w << " , " << it->h << " ) ";
      cout << " --> handler " << it->handler_id << endl;
    }


  }
}

void test_finder(){

  for (int screen_i=1; screen_i<=1; screen_i++){

    Finder f(screen_image_path(screen_i).c_str());
    f.debug(true);

    string testcase;
    /*
    testcase = "top";
    for (int target_i=1;target_i<=4;target_i++){

    Match m;

    f.find(target_image_path(screen_i,target_i).c_str());

    m = f.next();

    cout << "saving test result to " << result_image_path(screen_i,target_i,testcase) << endl;
    f.debug_save_image(result_image_path(screen_i,target_i,testcase).c_str());
    //f.debug_show_image();
    }
    */

    testcase = "above-0.8";
    for (int target_i=1;target_i<=4;target_i++){

      f.find(target_image_path(screen_i,target_i).c_str(),0.8);

      while (f.hasNext()){
        Match m;
        m = f.next();

      }

      cout << "saving test result to " << result_image_path(screen_i,target_i,testcase) << endl;
      f.debug_save_image(result_image_path(screen_i,target_i,testcase).c_str());
      f.debug_show_image();
    }

    //testcase = "top-5";
    //for (int target_i=1;target_i<=4;target_i++){

    //  f.find(target_image_path(screen_i,target_i).c_str());

    //  for (int i=0;i<5;++i){      
    //    Match m;
    //    m = f.next();
    //  }

    //  cout << "saving test result to " << result_image_path(screen_i,target_i,testcase) << endl;
    //  f.debug_save_image(result_image_path(screen_i,target_i,testcase).c_str());
    //  f.debug_show_image();
    //}//



    testcase = "roi-left-above-0.8";
    for (int target_i=1;target_i<=4;target_i++){

      Match m;

      f.setROI(0,0,f.get_screen_width()/2,f.get_screen_height());
      f.find(target_image_path(screen_i,target_i).c_str(),0.8);             

      while (f.hasNext()){
        f.next();   
      }

      cout << "saving test result to " << result_image_path(screen_i,target_i,testcase) << endl;
      f.debug_save_image(result_image_path(screen_i,target_i,testcase).c_str());
      f.debug_show_image();
    }
    /*
    testcase = "roi-lower-right";
    for (int target_i=1;target_i<=4;target_i++){

    Match m;

    f.setROI(f.get_screen_width()/2,f.get_screen_height()/2,f.get_screen_width()/2,f.get_screen_height()/2);
    f.find(target_image_path(screen_i,target_i).c_str());             

    for (int i=0; i < 5; i++)
    m = f.next();   

    cout << "saving test result to " << result_image_path(screen_i,target_i,testcase) << endl;
    f.debug_save_image(result_image_path(screen_i,target_i,testcase).c_str());
    //f.debug_show_image();
    }
    */

  }
}


void  test_change_finder(){

  ChangeFinder cf = ChangeFinder("testdata/before.png");  

  cf.debug(true);
  cf.setROI(150, 100, 500, 500);
  cf.find("testdata/after.png");

  while (cf.hasNext()){
    cf.next();
  }
}

int main(int argc, char** argv){

  //while (true)

  test_face_finder();
  test_change_finder();  
  test_sem();
  test_finder();

}
