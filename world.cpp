/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the world structure.
 *
 */

// Standard Libraries

// Custom Libraries
#include "world.h"

World::World(){

}

void World::loadWorld(){
  cout << "Building world..." << endl;
  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();
  string fn = "zones/zone_list.xml";
  Zone * zone;
  Projectile * proj;

  pair<multimap<string, string>::iterator, multimap<string, string>::iterator> _zlP;
  pair<multimap<string, pair<string, string> >::iterator, 
       multimap<string, pair<string, string> >::iterator> _zdP;
  multimap<string, string>::iterator _zl;
  multimap<string, pair<string, string> >::iterator _zd;
 
  parser->parseXML(fn, handle);

  _zlP = handle->findRangeData("zone_name");
  _zdP = handle->findRangeAttr("zone_name");

  _zd = _zdP.first;
  for(_zl = _zlP.first; _zl != _zlP.second; _zl++){
    zone = new Zone((*_zl).second);
    zoneMap[ ((*_zd).second).second ] = zone;

    cout << "Loading zone file from world: " << endl;
    cout << "   " << (*_zl).second << endl;
    cout << "    with name: " << ((*_zd).second).second << endl;

    _zd++;
  }

  activeZone = zoneMap[(*_zdP.first).second.second]; 
  delete handle;

  // Load combat assets
  Vector3f initialDir;

  fn = "combat_assets/combat_assets.xml";
  cout << "Loading combat assets from file: " << fn << endl;
  handle = new HandleChars();
  parser->parseXML(fn, handle);

  _zlP = handle->findRangeData("projectile");
  initialDir << 1, 1, 1;
  for(_zl = _zlP.first; _zl != _zlP.second; _zl++){
    cout << "   Loading combat asset: " << (*_zl).second << endl;
    proj = new Projectile((*_zl).second, initialDir);
    baseProjectile.push_back(proj);
  }
  
  delete handle;
}

World::~World(){

}

void World::updateWorld(float dT){
  activeZone->updateZone(dT);
}
