/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the skeleton class.  A skeleton is a set of defined joints.
 *
 */

#ifndef SKELETON_H_
#define SKELETON_H_

// Standard Libraries
#include <vector>

// Imported Libraries
#include "Eigen/Dense"

// Include Custom Libraries
#include "joint.h"

using namespace Eigen;

class Skeleton{
  public:
    Skeleton();
    ~Skeleton();

  private:
    std::vector<Joint*> jointList;

    Skeleton& operator=(Skeleton&);
    Skeleton(Skeleton&);
};

#endif
