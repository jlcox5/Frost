/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class represent the client side clock for the BWS MMO Engine. The file
 * sys/time.h is used in place of ctime for a much finer grain time scale.
 *
 */

#ifndef CLOCK_H_
#define CLOCK_H_

// Standard Libraries
#include <sys/time.h>
#include <iostream>

using std::cout; using std::endl;
using std::ostream;

class Clock{

  public:
    Clock(double delay);
    ~Clock();

    bool update();
    const double getDelay() const{ return TIME_DELAY; }
    const int getPost() const{ return mPost.tv_sec*10000000; }
    const int getCur() const{ return mCur.tv_sec*10000000; }
    const double getElapsed() const{ return mElapsed; }
    const double getPhysElapsed() const{ return pElapsed/10000000000.0; }
    void resetPhysElapsed(){ pElapsed = 0.0; }

    friend ostream& operator<<(ostream &, Clock const &);
  private:
    const double TIME_DELAY;
    timespec mPost;
    timespec mCur;
    double mElapsed;
    double pElapsed;

    Clock();
    Clock(Clock&);
    Clock& operator=(Clock&);

};

#endif
