/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles particle systems.
 *
 */

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

// Standard Libraries
#include <list>
#include <iostream>

// Imported Libraries
#include "Eigen/Dense"

// Custom Libraries
#include "particle.h"

using Eigen::Vector3f; using Eigen::Vector4f;
using std::cout; using std::endl;

class ParticleSystem{
  public:
    ParticleSystem(){}
    ParticleSystem(Vector3f const &, Vector3f const &, float, int, int, float, float, float, 
                   float, float, float);
    ~ParticleSystem();

    void update(float);
    void drawParticles();

  private:
    // Default particle generator is a plane defined by two corner points
    Vector3f fr; // Front right
    Vector3f bl; // Back left

    std::vector<Particle*> particles;    

    float lifeSpan;
    int maxParticles;
    int spawnRate;

    float maxVel;
    float minVel;
    float maxAcc;
    float minAcc;
    float maxMass;
    float minMass;

    void spawnParticle();

    // These will probably be super useful later, but until then, keep them private
    ParticleSystem(ParticleSystem const &);
    ParticleSystem& operator=(ParticleSystem const &);
};

#endif
