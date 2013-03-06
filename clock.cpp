/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class represent the client side clock for the BWS MMO Engine.
 *
 */

#include <iostream>

// Custom Libraries
#include "clock.h"

using std::cout; using std::endl;

Clock::Clock(double delay): TIME_DELAY(delay), mPost(), mCur(), mElapsed(delay+1), pElapsed(0){
  cout << "Building clock..." << endl;
  clock_gettime(CLOCK_MONOTONIC, &mPost);
  clock_gettime(CLOCK_MONOTONIC, &mCur);
}

Clock::~Clock(){

}

bool Clock::update(){
  clock_gettime(CLOCK_MONOTONIC, &mCur);
  mElapsed += ((mCur.tv_sec * 1000000000) + mCur.tv_nsec) - 
              ((mPost.tv_sec * 1000000000) + mPost.tv_nsec);

  pElapsed += mElapsed;
  if(mElapsed > TIME_DELAY){
    mPost = mCur;
    mElapsed = 0;
    return true;
  }

  return false;
}

ostream& operator<<(ostream & o, Clock const & c){
  o << "Clock State: " << endl;
  o << "   Post: " << c.getPost() << endl;
  o << "   Current: " << c.getCur() << endl;
  o << "   Elapsed: " << c.getElapsed() << "    with delay: " << c.getDelay() << endl;
  return o;
}

