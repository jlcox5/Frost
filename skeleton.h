/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the skeleton class.  A skeleton is a set of defined joints.
 *
 */
#ifndef SKELETON_H_
#define SKELETON_H_

// Standard Libraries
#include <string>
#include <vector>

// Imported Libraries
#include "Eigen/Dense"

// Include Custom Libraries
#include "joint.h"

using namespace Eigen;
using std::string;

class Skeleton{
  public:
    Skeleton();
    ~Skeleton();

    void pushRoot(Joint *);
    void addJoint(Joint *, Joint *);
    int getJointCount(){ return jointCount; }

    std::vector<Joint*> const & getFullJointList() const { return fullJointList; }
    int getFullJointCount(){ return fullJointList.size(); }

    void updateJointsAnim( std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > & );

    void mapJointInverseBind(std::vector<string> *,
                             std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > *);

    Joint const * getJointAt(int i){ return fullJointList[i]; }

  private:
    std::vector<Joint*> rootJointList;
    std::vector<Joint*> fullJointList;
    int jointCount;

    void updateJointMatrix(Joint * j);

    Skeleton& operator=(Skeleton&);
    Skeleton(Skeleton&);
};

#endif
