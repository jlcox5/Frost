/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles objects that are thrown from players or mobs.  It is derived from 
 * WOB.
 *
 */

// Standard Libraries

// Custom Libraries
#include "parseXML.h"
#include "handleChars.h"
#include "projectile.h"
#include "client.h"

Projectile::Projectile(): WOB(){

}

Projectile::Projectile(string const & fn, Vector3f const & direction): WOB(){
  string modelName;
  ParseXML * parse = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();

  parse->parseXML(fn, handle);
  // Load and set WOB parameters 
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

  largeScale = 1.0;
  if(scale(0) > 0){
    largeScale = scale(0);
  }
  if(largeScale < scale(1)){
    largeScale = scale(1);
  }
  if(largeScale < scale(2)){
    largeScale = scale(2);
  }

  radMod = handle->getXmlFloat("radius_mod");
  cout << "Largescale: " << largeScale << endl;

  modelName = handle->getXmlStr("model");

  model = Client::getInstance()->getAssets().getModelFactory().getModel(modelName);

  acceleration << handle->getXmlFloat("acceleration_x"), 
                  handle->getXmlFloat("acceleration_y"), 
                  handle->getXmlFloat("acceleration_z");

  damage = handle->getXmlFloat("damage");

  velocityMag = handle->getXmlFloat("velocity_mag");

  velocity << direction;
  velocity.normalize();
  velocity = velocityMag*velocity;

  dead = true;

  delete handle;
}

Projectile::Projectile(Projectile const & p): WOB(){
   velocity << p.velocity;
   acceleration << p.acceleration;
   damage = p.damage;
   velocityMag = p.velocityMag;
   dead = false;

   // Set WOB attributes
   model = p.model;
   rotate << p.rotate;
   translate << p.translate;
   scale << p.scale;
   largeScale = p.largeScale;
   radMod = p.radMod;

}

Projectile::~Projectile(){

}


void Projectile::update(float dT){
  Client * client;
  std::vector<SceneOBJ*> so;
  std::vector<SceneOBJ*>::iterator _so;
  std::list<MOB*> mo;
  std::list<MOB*>::iterator _mo;

  // New position
  translate = translate + dT*velocity + 0.5*acceleration*dT*dT;
  velocity = velocity + acceleration*dT;

  if(translate(1) < -1.0){
    dead = true;
  }


  // Check collisions....
  client = Client::getInstance();
  so = client->getWorld().getActiveZone()->getSceneObjList();
  // First scene object is always terrain.... don't test against that
  for(_so = so.begin()+2; _so != so.end(); _so++){
    if(testCollision((*_so))){
      dead = true;
      break;
    }
  }

  if(dead == false){
    mo = client->getWorld().getActiveZone()->getMobList();
    for(_mo = mo.begin(); _mo != mo.end(); _mo++){
      if(testCollision((*_mo))){
        dead = true;
        cout << "hit for " << damage << endl;
        (*_mo)->modHealth(-1*damage);
        break;
      }
    }
  }

  if(dead == false){
    //if(testCollision(&(client->getPlayer().getWOB()))){
    //  dead = true;
    //}
  }

}

void Projectile::setDirection(Vector3f const & v){
  velocityMag = velocity.norm();
  velocity << v;
  velocity.normalize();
  velocity = velocityMag*velocity;
}

void Projectile::setPosition(Vector3f const & v){
  translate << v;
}
