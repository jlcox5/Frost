/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles rotations, translations, and scaling of model objects.
 *
 */

#ifndef ANIMATE_H_
#define ANIMATE_H_

// Standard Libraries
#include <iostream>
#include <vector>

// SDL Libraries
#include <SDL.h>
#include <SDL_opengl.h>

// FBX Library
#include <fbxsdk.h>

// Imported Libraries
#include "Eigen/Dense"

using namespace Eigen;
using std::cout; using std::endl;

class Animate{

  public:
    Animate();
    Animate(std::vector< std::vector<float> *> *,
            std::vector< std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > *> *);
    Animate(Vector3d const &);

    Animate(Animate&);
    Animate& operator=(Animate const &);
    ~Animate();

    void runAnimation();
    void setVelocity(Vector3d const & v);
    void setRotateSpeed(float const f);
    void updateJoints();
    std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > & getPoses(){ return currentPoses; }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:
    Vector3d velocity;

    float minKeyFrame;
    float maxKeyFrame;
    float curKeyFrame;
    
    std::vector< std::vector<float> *> * jointKeyFrames;
    std::vector< std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > *> * jointPoses;
    std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > currentPoses;
};

#endif
