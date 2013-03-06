/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the world structure.
 *
 */

#ifndef WORLD_H_
#define WORLD_H_

// Standard Libraries

// Custom Libraries
#include "zone.h"
#include "projectile.h"
#include "parseXML.h"

class World{
  public:
    World();
    ~World();

    void loadWorld();
    Zone * getActiveZone() const{ return activeZone; }
    std::vector<Projectile*> const & getBaseProjectileList() const{ return baseProjectile; }
    void updateWorld(float);

  private:
    std::map<string, Zone*> zoneMap;
    std::vector<Projectile*> baseProjectile;
    Zone * activeZone;

    World(World const &);
    World& operator=(World const &);
};

#endif
