/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles particle systems.
 *
 */

// Standard Libraries
#include <limits.h>

// Custom Libraries
#include "particlesystem.h"

extern unsigned int counter;

ParticleSystem::ParticleSystem(Vector3f const & topRight, 
                               Vector3f const & backLeft, 
                               float ls = 100.0,
                               int mp = 1000,
                               int sr = 100,
                               float maxV = 5,
                               float minV = 1,
                               float maxA = 1,
                               float minA = 0,
                               float maxM = 1.0,
                               float minM = 0.1):
                               fr(topRight), 
                               bl(backLeft), 
                               lifeSpan(ls), 
                               maxParticles(mp),
                               spawnRate(sr),
                               maxVel(maxV),
                               minVel(minV),
                               maxAcc(maxA),
                               minAcc(minA),
                               maxMass(maxM),
                               minMass(minM){
}

ParticleSystem::~ParticleSystem(){
  std::vector<Particle*>::iterator _p;

  for(_p = particles.begin(); _p != particles.end(); _p++){
    delete (*_p);
  }

}

void ParticleSystem::update(float ts){
  // Check to see if particles need to be created
  if(particles.size() < static_cast<unsigned int>(maxParticles)){
    for(int i = 0; i < spawnRate; i++){
      spawnParticle();

      // Check size
      if(particles.size() >= static_cast<unsigned int>(maxParticles)){
        break;
      }
    }
  }

  // Update all of the particles in the list
  for(unsigned int i = 0; i < particles.size(); i++){
    particles[i]->update(ts);
  }
}

void ParticleSystem::drawParticles(){
  for(unsigned int i = 0; i < particles.size(); i++){
    particles[i]->draw();
  }
}

void ParticleSystem::spawnParticle(){
  Particle * p;

  // Generate random position within bounds of particle plane
  float x, y, z, r, velMag, accMag, mass;
  float vx, vy, vz;
  float ax, ay, az;
  Vector3f vel, acc; 
  int j;

  srand(counter);
  counter = (counter*21)%UINT_MAX;

  // Find position on emitter
  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  if(bl(0) < fr(0)){
    x = bl(0) + r*(fr(0)-bl(0));
  }
  else{
    x = fr(0) + r*(bl(0)-fr(0));
  }
  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  if(bl(1) < fr(1)){
    y = bl(1) + r*(fr(1)-bl(1));
  }
  else{
    y = fr(1) + r*(bl(1)-fr(1));
  }
  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  if(bl(2) < fr(2)){
    z = bl(2) + r*(fr(2)-bl(2));
  }
  else{
    z = fr(2) + r*(bl(2)-fr(2));
  }

  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  velMag = minVel + r*(maxVel-minVel);

  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  accMag = minAcc + r*(maxAcc-minAcc);

  // Velocity components
  // Velocity is set so it always falls - should be changed later for more dynamic
  // options.
  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  vx = -0.5 + r*1;
  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  vy = -1;
  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  vz = -0.5 + r*1;

   vx = 0;
   vy = -1;
   vz = 0;

  vel << vx, vy, vz;
  vel.normalize();
  vel = vel*velMag;

  // Acceleration components
  // Acceleration is set so it always falls - should be changed later for more dynamic
  // options.
  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  ax = -1 + r*2;
  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  ay = -1 + r*1;
  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  az = -1 + r*2;

  acc << ax, ay, az;
  acc.normalize();
  acc = acc*accMag;

  // Mass
  j = rand() % 100000000+1;
  r = static_cast<float>(j)/100000000.0;
  mass = minMass + r*(maxMass-minMass);
  p = new Particle(*(new Vector3f(x, y, z)), vel, acc, lifeSpan, 0, mass);
  particles.push_back(p);

}
