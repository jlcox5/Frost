/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the player object.
 *
 */

// Standard Libraries

// Custom Libraries
#include "player.h"
#include "client.h"
#include "projectile.h"

Player::Player(): wob(), curHealth(-1), maxHealth(-1){
}

void Player::loadPlayer(string const & fn){
  ParseXML * parse = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();
  Vector3d trans;

  parse->parseXML(fn, handle);

  wob = new WOB(handle->getXmlStr("wob_file"));

  curHealth = handle->getXmlFloat("current_health");
  maxHealth = handle->getXmlFloat("max_health");
  canMove = true;

  facing << 0, 0, -1;

  dead = false;
  attackSpeed = handle->getXmlFloat("attack_speed");
  attackTimer = 0.0;

  delete handle;
}

Player::~Player(){
  delete wob;
}

void Player::rotatePlayer(int i){
  Vector3f v1, v2;
  float ang = 3.0*i;
  Vector4f v;

  // This will need to be changed as the mouse can rotate freely....
  v1 << Client::getInstance()->getRenderer().getCamera().getAim() -
       Client::getInstance()->getRenderer().getCamera().getEye();
  v1 << v1(0), 0.0, v1(2);
  v1.normalize();

  v2 << 0.0, 1.0, 0.0;
  v2 << v2.cross(v1);
  v2.normalize();
  facing = v1;

  v << 0, 0, 0, ang;

  wob->setRotation(wob->getRotation() + v);
}

void Player::translatePlayer(int dir){
  Vector3f v1, v2;
  Vector3f oldTrans;
  double dist = 1.0;

  // This will need to be changed as the mouse can rotate freely....
  v1 << Client::getInstance()->getRenderer().getCamera().getAim() -
       Client::getInstance()->getRenderer().getCamera().getEye();
  v1.normalize();
  v1 << v1(0), 0.0, v1(2);
  v1.normalize();

  v2 << 0.0, 1.0, 0.0;
  v2 << v2.cross(v1);
  v2.normalize();
  canMove = true;

  facing = v1;

  switch(dir){
    // W is pressed
    case 0:
      oldTrans = wob->getTranslation();
      wob->setTranslation(wob->getTranslation() + v1*dist);
      if(!testMove()){
        canMove = false;
        wob->setTranslation(oldTrans);
      }
      break;

    // S is pressed
    case 1:
      oldTrans = wob->getTranslation();
      wob->setTranslation(wob->getTranslation() - v1*dist);
      if(!testMove()){
        canMove = false;
        wob->setTranslation(oldTrans);
      }
      break;

    // Q is pressed
    case 2:
      oldTrans = wob->getTranslation();
      wob->setTranslation(wob->getTranslation() + v2*dist);
      if(!testMove()){
        canMove = false;
        wob->setTranslation(oldTrans);
      }
      break;

    // E is pressed
    case 3:
      oldTrans = wob->getTranslation();
      wob->setTranslation(wob->getTranslation() - v2*dist);
      if(!testMove()){
        canMove = false;
        wob->setTranslation(oldTrans);
      }
      break;
  }
}

bool Player::testMove() const{
  Client * client;
  std::vector<SceneOBJ*> so;
  std::vector<SceneOBJ*>::iterator _so;
  std::list<MOB*> mo;
  std::list<MOB*>::iterator _mo;
  bool canMove = true;

  // Check collisions....
  client = Client::getInstance();
  so = client->getWorld().getActiveZone()->getSceneObjList();
  for(_so = so.begin()+2; _so != so.end(); _so++){
    if(wob->testCollision((*_so))){
      canMove = false;
      break;
    }
  }

  if(canMove == true){
    mo = client->getWorld().getActiveZone()->getMobList();
    for(_mo = mo.begin(); _mo != mo.end(); _mo++){
      if(wob->testCollision((*_mo))){
        canMove = false;
        break;
      }
    }
  }

  return canMove;
}

void Player::modHealth(double h){
  curHealth += h;
  if(curHealth > maxHealth){
    curHealth = maxHealth;
  }
  else if(curHealth <= 0){
    dead = true;
    cout << "You died!!! Better luck next time!" << endl;
    exit(0);
  }
}

void Player::update(float dT){
  if(attackTimer <= attackSpeed){
    attackTimer += dT;
  }
}

void Player::attack(){
  if(attackTimer > attackSpeed){
    Vector3f fireDir, firePos, dirPush;
    Client * client = Client::getInstance();

    Projectile * liveProjectile = new Projectile(*(client->getWorld().
                                                         getBaseProjectileList()[0]));
    fireDir << facing;
    fireDir(1) = 0.0;
    fireDir.normalize();
    liveProjectile->setDirection(fireDir);
    firePos << wob->getTranslation();
    firePos(1) = 1.0;
    dirPush = facing;
    dirPush.normalize();
    dirPush(1) = 0;
    firePos = firePos + dirPush*5.0;
    liveProjectile->setPosition(firePos);
    liveProjectile->setRotation(wob->getRotation());
    client->getWorld().getActiveZone()->addActiveProjectile(liveProjectile);
    attackTimer = 0;
  }
}
