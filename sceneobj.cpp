/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This abstract class handles static world objects.
 *
 */

// Standard Libraries

// Imported Libraries

// Custom Libraries
#include "parseXML.h"
#include "handleChars.h"
#include "sceneobj.h"
#include "client.h"

SceneOBJ::SceneOBJ(string const & fn): WOB(){
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

  delete handle;
}

SceneOBJ::~SceneOBJ(){

}

void SceneOBJ::draw() const{
  glEnable(GL_NORMALIZE);
  glEnable(GL_TEXTURE_2D);
  glPushMatrix();
  orient(); 
  model->getMesh().drawDisplayList();
  //model->getMesh().drawMesh();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void SceneOBJ::saveDisplayList(){
  cout << "SceneOBJ saving display list!!!" << endl;
  model->getMesh().saveDisplayList();
}

