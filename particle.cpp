/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the particle structure.
 *
 */

// Standard Libraries

// Custom Libraries
#include "particle.h"

Particle::Particle(Vector3f const & p, 
                   Vector3f const & v, 
                   Vector3f const & a, 
                   float ls, 
                   float ag,
                   float ma):
                   position(p), 
                   velocity(v), 
                   acceleration(a), 
                   lifeSpan(ls), 
                   age(ag),
                   mass(ma),
                   posList(),
                   posIt(0),
                   effects(0),
                   deg(0),
                   setPosList(false){

  effects = 1;  // Jitter - should be read from xml file
}

Particle::Particle(Particle const & p){
  position = p.getPos();
  velocity = p.getVel();
  acceleration = p.getAccel();

  lifeSpan = p.getLifeSpan();
  age = 0;
}

Particle::~Particle(){
  
}

Particle& Particle::operator=(Particle const & p){
  position = p.getPos();
  velocity = p.getVel();
  acceleration = p.getAccel();

  lifeSpan = p.getLifeSpan();
  age = 0;

  return *this;
}

void Particle::update(float dT){
  // Check if we've filled the position list or are too old
  if( (posList.size() >= 2000 || age > lifeSpan) && setPosList == false){
    lifeSpan = age;
    setPosList = true;
  }

  if(setPosList == false){
    if(effects == 1){
      jitter();
    }
    position = position + dT*velocity + 0.5*acceleration*dT*dT;
    velocity = velocity + acceleration*dT;
    posList.push_back(position);
  }
  else{
    if(posIt == 0){
      age = 0;
    }
    position = posList[posIt];
    posIt = (posIt+1) % posList.size();
  }
  age += dT;
}

void Particle::jitter(){
  deg = fmod((deg + 5.0), 360);
  acceleration << 5, 0, 5;
  acceleration << acceleration(0)*cos((deg*M_PI)/180.0),
              acceleration(1),
              acceleration(2)*sin((deg*M_PI)/180.0);
}

void Particle::draw(){
  float alpha;

  alpha = fmax(((lifeSpan-age)/lifeSpan)*2, 0.25);
  glDisable(GL_TEXTURE_2D);
  glPointSize(mass);
  glColor4f(1.0, 1.0, 1.0, 1.0);
  glBegin(GL_POINTS);
    glVertex3f(position(0), position(1), position(2));
  glEnd();
  glEnable(GL_TEXTURE_2D);
}
