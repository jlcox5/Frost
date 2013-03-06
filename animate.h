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

// SDL Libraries
#include <SDL.h>
#include <SDL_opengl.h>

// FBX Library
#include <fbxsdk.h>

// Imported Libraries
#include "Eigen/Dense"

using Eigen::Vector3d; using Eigen::Vector4d;
using std::cout; using std::endl;

class Animate{

  public:
    Animate();
    Animate(Vector3d const &);

    Animate(Animate&);
    Animate& operator=(Animate const &);
    ~Animate();

    void runAnimation();
    void setVelocity(Vector3d const & v);
    void setRotateSpeed(float const f);

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:
    Vector3d velocity;
};

#endif
