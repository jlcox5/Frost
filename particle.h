/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the particle structure.  This class is meant to be a base class for
 * billboards and other more advanced particle systems.
 *
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

// Standard Libraries
#include <vector>
#include <iostream>

// SDL Libraries
#include <SDL.h>
#include <SDL_opengl.h>

// Imported Libraries
#include "Eigen/Dense"

// Custom Libraries

using Eigen::Vector3f; using Eigen::Vector4f;
using std::cout; using std::endl;

class Particle{
  public:
    Particle(){}
    Particle(Vector3f const &, Vector3f const &, Vector3f const&, float, float, float);
    Particle(Particle const &);
    Particle& operator=(Particle const &);
    ~Particle();

    void update(float);
    void draw();

    Vector3f const & getPos() const{ return position; }
    Vector3f const & getVel() const{ return velocity; }
    Vector3f const & getAccel() const{ return acceleration; }
    float getLifeSpan() const{ return lifeSpan; }

    void setPos(Vector3f const & v){ position << v(0), v(1), v(2); }
    void setVel(Vector3f const & v){ velocity << v(0), v(1), v(2); }
    void setAccel(Vector3f const & v){ acceleration << v(0), v(1), v(2); }
    void setEffects(int i){ effects = i; }

    void jitter();

  private:
    Vector3f position;
    Vector3f velocity;
    Vector3f acceleration;

    float lifeSpan;
    float age;
    float mass;
    std::vector<Vector3f> posList;
    int posIt;
    int effects;
    float deg;
    bool setPosList;

};

#endif

