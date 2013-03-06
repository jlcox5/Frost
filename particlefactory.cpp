/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the particle factory.
 *
 */

// Standard Libraries

// Custom Libraries
#include "particlefactory.h"

ParticleFactory::~ParticleFactory(){
  map<string, ParticleSystem*>::iterator _p;

  for(_p = particleSystemList.begin(); _p != particleSystemList.end(); _p++){
    delete (*_p).second;
  }

  particleSystemList.clear();
}

ParticleSystem * ParticleFactory::getParticleSystem(string const & fn){
  map<string, ParticleSystem*>::iterator _p;
  ParticleSystem * ret;

  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();

  _p = particleSystemList.find(fn);

  if(_p == particleSystemList.end()){
    cout << "Loading particle system from factory: " << fn << endl;
    parser->parseXML(fn, handle);

    particleSystemList[fn] = new ParticleSystem(*(new Vector3f(handle->getXmlFloat("tr_x"),
                                          handle->getXmlFloat("tr_y"),
                                          handle->getXmlFloat("tr_z"))),
                             *(new Vector3f(handle->getXmlFloat("bl_x"),
                                          handle->getXmlFloat("bl_y"),
                                          handle->getXmlFloat("bl_z"))),
                             handle->getXmlFloat("ls"),
                             handle->getXmlInt("mp"),
                             handle->getXmlInt("sr"),
                             handle->getXmlFloat("maxV"),
                             handle->getXmlFloat("minV"),
                             handle->getXmlFloat("maxA"),
                             handle->getXmlFloat("minA"),
                             handle->getXmlFloat("maxM"),
                             handle->getXmlFloat("minM"));
    
    ret = particleSystemList[fn];                       
  }
  else{
    ret = _p->second;
  }

  return ret;
}

bool ParticleFactory::eraseParticleSystem(string const & fn){
  ParticleSystem * toDel = getParticleSystem(fn);
  delete toDel;

  int i = particleSystemList.erase(fn);

  if(i == 0){
    cout << "Particle system not found: " << fn << endl;
    return false;
  }

  return true;
}
