/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the zone structure.
 *
 */

#ifndef ZONE_H_
#define ZONE_H_

// Standard Libraries
#include <iostream>
#include <list>

// Custom Libraries
#include "parseXML.h"
#include "handleChars.h"
#include "model.h"
#include "sceneobj.h"
#include "mob.h"
#include "projectile.h"
#include "particleeffects.h"

class Zone{
  public:
    Zone(const string);
    ~Zone();

    std::vector<string> const & getModelList() const{ return modelList; }
    std::vector<SceneOBJ*> const & getSceneObjList() const{ return sceneObjList; }
    std::list<MOB*> const & getMobList() const{ return mobList; }
    std::list<Projectile*> const & getProjectileList() const{ return projectileList; }
    std::vector<ParticleEffects*> const & getParticleEffectsList() const{ return particleEffectsList; }

    void addActiveProjectile(Projectile * lp){ projectileList.push_back(lp); }
    void loadModels(string const &);
    void loadSceneObj(string const &);
    void loadMob(string const &);
    void loadParticleEffects(string const &);
    void updateZone(float);
    void removeZone() const;

  private:
    string terrain;
    string sky;
    std::vector<string> modelList;
    std::vector<SceneOBJ*> sceneObjList;
    std::list<MOB*> mobList;
    std::list<Projectile*> projectileList;
    std::vector<ParticleEffects*> particleEffectsList;
};

#endif
