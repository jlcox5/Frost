/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles all rendering issues.
 *
 */

// Standard Libraries
#include <iostream>

// Custom Libraries
#include "world.h"
#include "renderer.h"
#include "client.h"
#include "sceneobj.h"
#include "mob.h"
#include "particleeffects.h"

using std::cout; using std::endl;

Renderer::Renderer(Assets &a): camera(), assets(a){
   cout << "Trying to build a camera." << endl;
   cout << "Building renderer" << endl;
}

Renderer::~Renderer(){
  cout << "Goodbye Renderer!" << endl;
}

void Renderer::renderScene() const{
  Client * client = Client::getInstance();

  std::vector<SceneOBJ*> const & sl = client->getWorld().getActiveZone()->
                                      getSceneObjList();
  std::vector<SceneOBJ*>::const_iterator _sl;

  std::list<MOB*> const & ml = client->getWorld().getActiveZone()->
                                 getMobList();
  std::list<MOB*>::const_iterator _ml;

  std::list<Projectile*> const & pl = client->getWorld().getActiveZone()->
                                        getProjectileList();
  std::list<Projectile*>::const_iterator _pl;

  std::vector<ParticleEffects*> const & el = client->getWorld().getActiveZone()->
                                             getParticleEffectsList();
  std::vector<ParticleEffects*>::const_iterator _el;

  for(_sl = sl.begin(); _sl != sl.end(); _sl++){
    (*_sl)->draw();
  }
  glColor4f(0.5, 0.5, 0.5, 1.0);
  for(_ml = ml.begin(); _ml != ml.end(); _ml++){
    (*_ml)->animate();
    (*_ml)->draw();
  }
  for(_pl = pl.begin(); _pl != pl.end(); _pl++){
    (*_pl)->draw();
  }
  for(_el = el.begin(); _el != el.end(); _el++){
    (*_el)->draw();
  }

  client->getPlayer().getWOB().draw();

  camera.setCameraHud(); 
  glDisable(GL_DEPTH_TEST);
  client->getHud().displayHealthBar();
  if(client->getHud().displayHud()){
    std::vector<SceneOBJ*> const & sl2 = client->getHud().getSceneObjList();
    for(_sl = sl2.begin(); _sl != sl2.end(); _sl++){
      (*_sl)->draw();
    }
  }
  glEnable(GL_DEPTH_TEST);
  camera.setCamera(); 
}

void Renderer::rotateCamera(int i){
  camera.rotateCamera(i);
}

void Renderer::translateCamera(int i){
  camera.translateCamera(i);
}

void Renderer::renderCamera() const{
  camera.drawAimPoint();
}
