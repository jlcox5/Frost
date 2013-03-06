/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the particle factory.
 *
 */

#ifndef PARTICLEFACTORY_H_
#define PARTICLEFACTORY_H_

// Standard Libraries
#include <map>
#include <algorithm>
#include <string>

// Custom Libraries
#include "parseXML.h"
#include "handleChars.h"
#include "particlesystem.h"

class ParticleFactory{
  public:
    ParticleFactory() : particleSystemList( std::map<string, ParticleSystem*>() ){}
    ~ParticleFactory();
    ParticleSystem * getParticleSystem(string const &);
    bool eraseParticleSystem(string const &);

  private:
    map<string, ParticleSystem*> particleSystemList;

    ParticleFactory(ParticleFactory const &);
    ParticleFactory& operator=(ParticleFactory const &);
};

#endif
