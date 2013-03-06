/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the joint class.  A joint is defined as a translation, a rotation,
 * and a scale.
 *
 */

#ifndef JOINT_H_
#define JOINT_H_

// Imported Libraries
#include "Eigen/Dense"

using namespace Eigen;

class Joint{
  public:
    Joint();
    ~Joint();

  private:
    Vector3f pos;
    Quaternionf rot;
    float scale;

    Joint(Joint&);
    Joint& operator=(Joint&);
};

#endif
