/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the world object class.
 *
 */

// Standard Libraries

// Custom Libraries
#include "wob.h"
#include "client.h"
#include "handleChars.h"

WOB::WOB(string const & fn){
  string modelName;
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
}

WOB::WOB(Model * m, Vector4f const & r, Vector3f const & t, Vector3f const & s){
  model = m;

  rotate << r(0), r(1), r(2), r(3);
  translate << t(0), t(1), t(2);
  scale << s(0), s(1), s(2);
}

WOB::~WOB(){
}

void WOB::draw() const{
  glEnable(GL_NORMALIZE);
  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  orient(); 
  model->drawModel();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void WOB::animate(){
  model->animateModel();
}

void WOB::orient() const{
  glTranslatef(translate[0], translate[1],translate[2]);
  glRotatef(rotate[3], rotate[0], rotate[1], rotate[2]);
  glScalef(scale[0], scale[1], scale[2]);
}

void WOB::setRotation(Vector4f const & v){
  rotate << v(0), v(1), v(2), v(3);
}

void WOB::setTranslation(Vector3f const & v){
  translate = v;
}

void WOB::setScale(Vector3f const & v){
  scale = v;
}

void WOB::setModel(Model * m){
  model = m;
}

bool WOB::testCollision(WOB const * w) const{
  float r1, r2;
  float s1, s2;
  float d;
  Vector3f wT;
  wT << w->getTranslation()(0), w->getTranslation()(1), w->getTranslation()(2);

  r1 = w->getModel().getMesh().getRadius()*w->getRadMod();
  r2 = model->getMesh().getRadius()*radMod;

  s1 = w->getLargeScale();
  s2 = largeScale;

  // Sphere bound test
  d = sqrt((wT[0]-translate[0])*(wT[0]-translate[0]) +
           (wT[1]-translate[1])*(wT[1]-translate[1]) +
           (wT[2]-translate[2])*(wT[2]-translate[2])); 

  if(d < r1*s1+r2*s2){
    return true;
  }

  return false;
}

