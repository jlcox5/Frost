/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the joint class.  A joint is defined as a translation, a rotation,
 * and a scale.
 *
 */

#ifndef JOINT_H_
#define JOINT_H_

// Standard Libraries
#include <vector>
#include <iostream>
#include <string>

// Imported Libraries
#include "Eigen/Dense"

using namespace Eigen;

using std::cout; using std::endl;

class Joint{
  public:
    Joint();
    Joint(std::string &);
    ~Joint();

    void setMatrix(Matrix4f &);
    void addChild(Joint *);
    void setParent(Joint *);
    void setInverseBind(Matrix4f &);

    std::string const & getId() const{ return id; }

    void updateJoint(Matrix4f *);
    Matrix4f const & getCurTrans() const{ return currentTransform; }
    Matrix4f const & getInverseBind() const{ return inverseBind; }
    Matrix4f const & getCombined() const{ return combined; }
    void updateChildren();

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

  private:
    Vector3f pos;
    Quaternionf rot;
    float scale;
    Joint * parent;
    std::vector<Joint*> children;

    std::string id;
    Matrix4f transform; // Just until converstion to quaternion is completed
    Matrix4f currentTransform;
    Matrix4f inverseBind;
    Matrix4f combined;

    Joint(Joint&);
    Joint& operator=(Joint&);
};

#endif
