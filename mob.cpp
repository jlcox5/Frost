/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles mobile objects, such as monters, NPC's, etc.
 *
 */

// Standard Libraries
#include <sstream>

// Custom Libraries
#include "mob.h"
#include "client.h"
#include "handleChars.h"

MOB::MOB(string const & fn): WOB(){
  string modelName;
  string degStr;
  string distStr;
  int pathSize;
  std::stringstream num;

  ParseXML * parse = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();

  parse->parseXML(fn, handle);
  // Load and set WOB parameters 
  cout << "fn: " << fn << endl;

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

  facing << handle->getXmlFloat("facing_x"), 
            handle->getXmlFloat("facing_y"), 
            handle->getXmlFloat("facing_z");

  pathSize = handle->getXmlInt("path_size");
  for(int i = 1; i <= pathSize; i++){
    num << i;

    degStr = "rot_";
    degStr.append(num.str());
    degList.push_back(handle->getXmlFloat(degStr));

    distStr = "distance_";
    distStr.append(num.str());
    distList.push_back(handle->getXmlFloat(distStr));

    num.str("");
  }

  posIt = 0;
  speed = handle->getXmlFloat("speed");
  damage = handle->getXmlFloat("damage");
  curHealth = handle->getXmlFloat("curHealth");
  maxHealth = handle->getXmlFloat("maxHealth");
  aggressive = handle->getXmlInt("aggressive");
  curDeg = 0;
  dead = false;
  chasingPlayer = false;
  chaseDistance = 0.0;
  maxChase = handle->getXmlFloat("maxChase");
  aggrDist = handle->getXmlFloat("aggrDist");
  coolingDown = false;
  attackSpeed = handle->getXmlFloat("attackSpeed");
  attackTimer = 0;

  delete handle;
}

MOB::~MOB(){

}

void MOB::update(float dT){
  float ang = 300.0*dT;
  float angMod = 1;
  Vector4f v;
  Vector3f oldTrans;
  Client * client;
  std::vector<SceneOBJ*> so;
  std::vector<SceneOBJ*>::iterator _so;
  std::list<MOB*> mo;
  std::list<MOB*>::iterator _mo;
  bool canMove = true;

  client = Client::getInstance();

  // Should mob chase the player
  if(aggressive == 1 && chasingPlayer == false && coolingDown == false){
    float dtPlayer;
    Vector3f pTrans = client->getPlayer().getWOB().getTranslation();
    dtPlayer = sqrt((pTrans(0)-translate(0))*(pTrans(0)-translate(0)) + 
                    (pTrans(1)-translate(1))*(pTrans(1)-translate(1)) + 
                    (pTrans(2)-translate(2))*(pTrans(2)-translate(2)) );

    if(dtPlayer <= aggrDist){
      chasingPlayer = true;
      chaseDistance = 0.0;
    }
  }
  // Chasing player
  if(aggressive == 1 && chasingPlayer == true && coolingDown == false){
    float turnAng;
    Vector3f newFacing;
    Vector3f uX, uY, uZ;
    Matrix3f m;
    float dot;
    float dotUF, dotUN;
    int usingX = 0;
    float distToPlayer = -1;
    newFacing << client->getPlayer().getWOB().getTranslation() - translate;

    distToPlayer = sqrt( (newFacing(0)-translate(0))*(newFacing(0)-translate(0)) +
                         (newFacing(1)-translate(1))*(newFacing(1)-translate(1)) + 
                         (newFacing(2)-translate(2))*(newFacing(2)-translate(2)));
 
    newFacing(1) = 0;
    facing(1) = 0;
    newFacing.normalize();
    facing.normalize();

    if(chaseDistance >= maxChase && distToPlayer > 25.0){
       chasingPlayer = false;
       //facing = -1*facing;
       coolingDown = true;
    }

    // Turn if needed
    float vDif = sqrt( (facing(0)-newFacing(0))*(facing(0)-newFacing(0)) +
                       (facing(1)-newFacing(1))*(facing(1)-newFacing(1)) +
                       (facing(2)-newFacing(2))*(facing(2)-newFacing(2)));
    //else if(facing != newFacing){
    if(vDif > 0.01){

      // Figure out which direction to turn
      uX << 1, 0, 0;
      dotUF = uX.dot(facing);
      dotUN = uX.dot(newFacing);
      dot = facing.dot(newFacing);

      // Cap dot
      if(dot > 1) dot = 1;
      else if(dot < -1) dot = -1;
      if(dotUF > 1) dotUF = 1;
      else if(dotUF < -1) dotUF = -1;
      if(dotUN > 1) dotUN = 1;
      else if(dotUN < -1) dotUN = -1;

      turnAng = acos(dot);
      turnAng = (turnAng*180.0)/M_PI;
      dotUF = acos(dotUF);
      dotUF = (dotUF*180.0)/M_PI;
      dotUN = acos(dotUN);
      dotUN = (dotUN*180.0)/M_PI;

      // This seems to be unnecessary but I need to convince myself first
      /*if(turnAng < dotUF && turnAng < dotUN){
        // Try with zAxis
        usingX = 1;
        uZ << 0, 0, 1;
        dotUF = uZ.dot(facing);
        dotUN = uZ.dot(newFacing);
        if(dotUF > 1) dotUF = 1;
        else if(dotUF < -1) dotUF = -1;
        if(dotUN > 1) dotUN = 1;
        else if(dotUN < -1) dotUN = -1;

        // Find angles from dot products
        dotUF = acos(dotUF);
        dotUF = (dotUF*180.0)/M_PI;
        dotUN = acos(dotUN);
        dotUN = (dotUN*180.0)/M_PI;
      } */

      if(usingX == 0){
        if((dotUF < dotUN && facing(2) > 0) || (dotUF > dotUN && facing(2) < 0)){
          angMod = -1.0;
        }
        else{
          angMod = 1.0;
        }
      }
      if(usingX == 1){
        if((dotUF < dotUN && facing(0) > 0) || (dotUF > dotUN && facing(0) < 0)){
          angMod = 1.0;
        }
        else{
          angMod = -1.0;
        }
      }

      turnAng = turnAng*angMod;

      curDeg = fmod(curDeg+turnAng, 360.0);

      v << 0, 0, 0, turnAng;
      WOB::setRotation(WOB::getRotation() + v);

      uY << 0, 1, 0;
      uZ << facing;
      uZ.normalize();
      uX = uZ.cross(uY);
      m = AngleAxisf((0.0*M_PI)/180.0, uX)
          * AngleAxisf((turnAng*M_PI)/180.0, uY)
          * AngleAxisf((0.0*M_PI)/180.0, uZ);
      facing << m*facing;
    }
    // Otherwise chase him!!!
    float d = dT * speed * 2;
    Vector3f trav = facing;
    trav.normalize();

    oldTrans = translate;
    translate = translate + trav*d;
    
    // Check collisions....
    so = client->getWorld().getActiveZone()->getSceneObjList();
    // First scene object is always terrain.... don't test against that
    for(_so = so.begin()+2; _so != so.end(); _so++){
      if(testCollision((*_so))){
        canMove = false;
        break;
      }
    }

    if(canMove == true){
      mo = client->getWorld().getActiveZone()->getMobList();
      for(_mo = mo.begin(); _mo != mo.end(); _mo++){
        if((*_mo) != this){
          if(testCollision((*_mo))){
            canMove = false;
            break;
          }
        }
      }
    }

    if(canMove == true){
      if(testCollision(&(client->getPlayer().getWOB()))){
        if(aggressive == 1 && attackTimer > attackSpeed){
          cout << "Player health: " << client->getPlayer().getCurHealth() << endl;
          client->getPlayer().modHealth(-1*damage);
          attackTimer = 0;
        }
        canMove = false;
      }
    }
    
    if(canMove == false){
      translate = oldTrans;
    } 
    else{
      chaseDistance += d;
    }
  }

  // Normal Movement
  if(aggressive == 0 || chasingPlayer == false || coolingDown == true){
    bool tooClose = false;
    float triggerDist = 10.0;
    float closestDist = 10.0;
    Vector3f objTrans;
    // Turning
    if(degList.size() > 0 && fabs(curDeg) <= fabs(degList[posIt])){

      if(curDeg < degList[posIt]){
        curDeg = fmod(curDeg+ang, 360.0);
      }
      else{
        curDeg = fmod(curDeg-ang, 360.0);
        angMod = -1;
      }

      turnMOB(ang*angMod);
    }
    // Moving forward
    else{
      float d = dT * speed;
      Vector3f trav = facing;
      trav.normalize();

      oldTrans = translate;
      translate = translate + trav*d;
      
      // Check collisions....
      so = client->getWorld().getActiveZone()->getSceneObjList();
      // First scene object is always terrain.... don't test against that
      for(_so = so.begin()+2; _so != so.end(); _so++){
        if(testCollision((*_so))){
          canMove = false;
          break;
        }
        if(tooClose == false){
          objTrans << (*_so)->getTranslation();
          closestDist = sqrt( (objTrans(0)-translate(0))*(objTrans(0)-translate(0)) +
                              (objTrans(1)-translate(1))*(objTrans(1)-translate(1)) +
                              (objTrans(2)-translate(2))*(objTrans(2)-translate(2)) );
          if(closestDist < triggerDist ){
            tooClose = true;
          }
        }
      }

      if(canMove == true){
        mo = client->getWorld().getActiveZone()->getMobList();
        for(_mo = mo.begin(); _mo != mo.end(); _mo++){
          if((*_mo) != this){
            if(testCollision((*_mo))){
              canMove = false;
              break;
            }
            if(tooClose == false){
              objTrans << (*_mo)->getTranslation();
              closestDist = sqrt( (objTrans(0)-translate(0))*(objTrans(0)-translate(0)) +
                                  (objTrans(1)-translate(1))*(objTrans(1)-translate(1)) +
                                  (objTrans(2)-translate(2))*(objTrans(2)-translate(2)) );
              if(closestDist < triggerDist ){
                tooClose = true;
              }
            }
          }
        }
      }

      if(canMove == true){
        if(testCollision(&(client->getPlayer().getWOB()))){
          if(aggressive == 1 && attackTimer > attackSpeed){
            client->getPlayer().modHealth(-1*damage);
            attackTimer = 0;
          }
          canMove = false;
        }
        if(tooClose == false){
          objTrans << client->getPlayer().getWOB().getTranslation();
          closestDist = sqrt( (objTrans(0)-translate(0))*(objTrans(0)-translate(0)) +
                              (objTrans(1)-translate(1))*(objTrans(1)-translate(1)) +
                              (objTrans(2)-translate(2))*(objTrans(2)-translate(2)) );
          if(closestDist < triggerDist ){
            tooClose = true;
          }
        }
      }
      
      if(canMove == false){
        translate = oldTrans;
        turnMOB(-300*dT);
      }
      else if(tooClose == true){
        turnMOB(-300*(closestDist/triggerDist)*dT);
      }
      else{
        if(coolingDown == true){
          if( chaseDistance <= 0.0){
            chaseDistance = 0.0;
            coolingDown = false;
          }
          else{
            //*HACK* Should have a cooldown defined in xml files
            chaseDistance -= 2.5*d;
          }
        }
        else{
          curDist += d;
          
        }
      }

      if(distList.size() > 0 && curDist >= distList[posIt]){
        posIt = (posIt+1)%degList.size();
        curDist = 0;
        curDeg = 0;
      }
    }

    // Test against skysphere....
    float skyRad = client->getWorld().getActiveZone()->getSceneObjList()[1]->
                   getModel().getMesh().getRadius();
    float rm = client->getWorld().getActiveZone()->getSceneObjList()[1]->getRadMod();

    // Sky is cenetered on origin
    float d = sqrt( translate(0)*translate(0) + translate(1)*translate(1) + 
              translate(2)*translate(2));
    Vector3f tempFac, tempTrans;
    if(d >= skyRad*rm-5.0){
      tempFac << facing;
      tempFac.normalize();
      tempTrans << translate;
      tempTrans.normalize();
      float dot = tempFac.dot(-1*tempTrans);
      if(dot > 1){
        dot = 1;
      }
      if(dot < -1){
        dot = -1;
      }
      if( dot < 0 ){
        ang = acos( dot );
        turnMOB(150);
      }
    }
  }
  if(attackTimer < attackSpeed){
    attackTimer += dT;
  }
}

void MOB::modHealth(float h){
  curHealth += h;
  if(curHealth > maxHealth){
    curHealth = maxHealth;
  }
  else if(curHealth <= 0){
    dead = true;
  }
}

void MOB::turnMOB(float ang){
  Vector3f uX, uY, uZ;
  Vector4f v;
  Matrix3f m;

  v << 0, 0, 0, ang;
  WOB::setRotation(WOB::getRotation() + v);

  uY << 0, 1, 0;
  uZ << facing;
  uZ.normalize();
  uX = uZ.cross(uY);

  m = AngleAxisf((0.0*M_PI)/180.0, uX)
      * AngleAxisf((ang*M_PI)/180.0, uY)
      * AngleAxisf((0.0*M_PI)/180.0, uZ);
  facing << m*facing;

}

