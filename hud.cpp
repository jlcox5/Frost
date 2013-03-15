/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the hud.
 *
 */

// Standard Libraries

// Custom Libraries
#include "client.h"
#include "hud.h"
#include "player.h"

Hud::Hud(){

  display = false;

}

Hud::~Hud(){

}

void Hud::loadHud(string const & fn){
  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();

  string modelFN, sceneObjFN;
  parser->parseXML(fn, handle);

  modelFN = handle->getXmlStr("model_list");
  loadModels(modelFN);

  sceneObjFN  = handle->getXmlStr("sceneobj_list");
  loadSceneObj(sceneObjFN);

  delete handle;
}

void Hud::loadModels(string const & fn){
  pair<multimap<string, string>::iterator, multimap<string, string>::iterator> _mrd;
  multimap<string, string>::iterator _ml;

  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();
  HandleChars * handleModelFile;
  parser->parseXML(fn, handle);
  Client * client = Client::getInstance();

  _mrd = handle->findRangeData("model");

  cout << "Loading hud model list file: " << fn << endl;
  for(_ml = _mrd.first; _ml != _mrd.second; _ml++){
     cout << "   Loading model file: " << (*_ml).second << endl;
     handleModelFile = new HandleChars();
     client->getUIAssets().getModelFactory().getModel((*_ml).second);
     parser->parseXML((*_ml).second, handleModelFile); 
     modelList.push_back((*_ml).second);
     delete handleModelFile;
  }
  delete handle;
}

void Hud::loadSceneObj(string const & fn){
  pair<multimap<string, string>::iterator, multimap<string, string>::iterator> _sld;
  multimap<string, string>::iterator _sl;

  string sofn;
  SceneOBJ * so;

  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();
  parser->parseXML(fn, handle);

  _sld = handle->findRangeData("sceneobj");

  cout << "Loading hud sceneobj list file: " << fn << endl;
  for(_sl = _sld.first; _sl != _sld.second; _sl++){
    cout << "   Loading sceneobj file: " << (*_sl).second << endl;
    so = new SceneOBJ((*_sl).second);
    so->saveDisplayList();
    sceneObjList.push_back(so);
  }
  delete handle;
}

void Hud::toggleDisplay(){
  if(display){
    display = false;
  }
  else{
    display = true;
  }
  cout << "display: " << display << endl;
}

void Hud::displayHealthBar() const{
  Client * client = Client::getInstance();
  double curHealth = client->getPlayer().getCurHealth();
  double maxHealth = client->getPlayer().getMaxHealth();

  // Draw max health bar
  glDisable(GL_TEXTURE_2D);
  glColor4f(0.25, 0.25, 0.25, 1.0);
  glBegin(GL_QUADS);
    glVertex3f(-4.25, -2.25, 0.0);
    glVertex3f(-1.0, -2.25, 0.0);
    glVertex3f(-1.0, -2.5, 0.0);
    glVertex3f(-4.25, -2.5, 0.0);
  glEnd();

  // Draw current health bar
  glColor4f(1.0, 0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
    glVertex3f(-4.25, -2.25, 0.0);
    glVertex3f(-4.25+(curHealth/maxHealth)*3.25, -2.25, 0.0);
    glVertex3f(-4.25+(curHealth/maxHealth)*3.25, -2.5, 0.0);
    glVertex3f(-4.25, -2.5, 0.0);
  glEnd();
  glEnable(GL_TEXTURE_2D);


}
