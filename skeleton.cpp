/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the skeleton class.  A skeleton is a set of defined joints.
 *
 */

// Custom Libraries
#include "skeleton.h"

Skeleton::Skeleton(){

}

Skeleton::~Skeleton(){
  std::vector<Joint*>::iterator _j;

  for(_j = jointList.begin(); _j != jointList.end(); _j++){
    delete *(_j);
  }

  jointList.clear();

}
