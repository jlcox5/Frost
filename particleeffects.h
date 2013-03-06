/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles particle effects.
 *
 */

#ifndef PARTICLEEFFECTS_H_
#define PARTICLEEFFECTS_H_

// Standard Libraries

// Custom Libraries
#include "particlesystem.h"

class ParticleEffects{
  public:
    ParticleEffects(string const &);
    ~ParticleEffects();

    void draw() const;
    void update(float);
    void setRotation(Vector4f const &);
    void setTranslation(Vector3f const &);
    void setScale(Vector3f const &);
    void setParticleSystem(ParticleSystem *);

    Vector3f const & getTranslation(){ return translate; }

    Vector4f const & getRotation(){ return rotate; }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:
    ParticleSystem * partSys;
    Vector4f rotate;
    Vector3f translate;
    Vector3f scale;

    // Orient the model according to initial conditions
    void orient() const;
   
    // Should not have a default constructor 
    ParticleEffects();

    // Implement later
    ParticleEffects(ParticleEffects const &);
    ParticleEffects const & operator=(ParticleEffects const &);
};

#endif
