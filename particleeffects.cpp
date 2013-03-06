/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles particle effects.
 *
 */

// Standard Libraries

// Custom Libraries
#include "client.h"
#include "parseXML.h"
#include "particleeffects.h"

ParticleEffects::ParticleEffects(string const & fn){
  string particleSystemName;
  ParseXML * parse = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();

  parse->parseXML(fn, handle);

  rotate << handle->getXmlFloat("rotate_x"), 
            handle->getXmlFloat("rotate_y"), 
            handle->getXmlFloat("rotate_z"),
            handle->getXmlFloat("rotate_w");

  translate << handle->getXmlFloat("translate_x"), 
               handle->getXmlFloat("translate_y"), 
               handle->getXmlFloat("translate_z");
   
  scale << handle->getXmlFloat("scale_x"), 
           handle->getXmlFloat("scale_y"), 
           handle->getXmlFloat("scale_z");

  particleSystemName = handle->getXmlStr("particle_system");

  partSys = Client::getInstance()->getAssets().
                   getParticleFactory().getParticleSystem(particleSystemName);
}

void ParticleEffects::orient() const{
  glTranslatef(translate[0], translate[1],translate[2]);
  glRotatef(rotate[3], rotate[0], rotate[1], rotate[2]);
  glScalef(scale[0], scale[1], scale[2]);
}

void ParticleEffects::draw() const{
  glEnable(GL_NORMALIZE);
  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  orient();
  partSys->drawParticles();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void ParticleEffects::update(float dT){
  partSys->update(dT);
}

void ParticleEffects::setRotation(Vector4f const & v){
  rotate = v;
}

void ParticleEffects::setTranslation(Vector3f const & v){
  translate = v;
}

void ParticleEffects::setScale(Vector3f const & v){
  scale = v;
}

void ParticleEffects::setParticleSystem(ParticleSystem * p){
  partSys = p;
}
