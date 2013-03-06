/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This abstract class handles world objects.  This is the base class for the mob and 
 * sceneobj classes.
 *
 */

#ifndef WOB_H_
#define WOB_H_

// Standard Libraries

// Imported Libraries
#include "Eigen/Dense"

// Custom Libraries
#include "model.h"

using Eigen::Vector3f; using Eigen::Vector4f;
using std::string;

class WOB{
  public:
    WOB(){}
    WOB(string const &);
    WOB(Model *, Vector4f const &, Vector3f const &, Vector3f const &);
    virtual ~WOB();

    virtual void draw() const;
    void setRotation(Vector4f const &);
    void setTranslation(Vector3f const &);
    void setScale(Vector3f const &);
    void setModel(Model *);
    Model const & getModel() const{ return *model; }

    Vector3f const & getTranslation() const{ return translate; }
    Vector3f const & getScale() const{ return scale; }
    Vector4f const & getRotation() const{ return rotate; }
    float getLargeScale() const{ return largeScale; }
    float getRadMod() const{ return radMod; }

    bool testCollision(WOB const *) const;


    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  protected:
    Model * model;
    Vector4f rotate;
    Vector3f translate;
    Vector3f scale;
    float largeScale;
    float radMod;

    // Orient the model according to initial conditions
    void orient() const;

    // No default constructor
};

#endif
