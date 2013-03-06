/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the zone structure.
 *
 */

// Standard Libraries

// Custom Libraries
#include "client.h"
#include "zone.h"

Zone::Zone(const string fn){
  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();
  HandleChars * handleTer = new HandleChars();
  HandleChars * handleSky = new HandleChars();
  string modelFN;
  string sceneObjFN;
  string mobFN;
  string partSysFN;
  SceneOBJ * so;

  cout << "Loading zone file from zone: " << fn << endl;
  parser->parseXML(fn, handle);
  terrain = handle->getXmlStr("terrain");
  sky = handle->getXmlStr("sky");

  // Push terrain and sky onto model list so it can be drawn
  parser->parseXML(terrain, handleTer);
  so = new SceneOBJ(terrain);
  cout << "**********Saving terrain display list!" << endl;
  so->saveDisplayList();
  sceneObjList.push_back(so);
  delete handleTer;

  parser->parseXML(sky, handleSky);
  so = new SceneOBJ(sky);
  cout << "**********Saving sky display list!" << endl;
  so->saveDisplayList();
  sceneObjList.push_back(so);
  delete handleSky;

  modelFN = handle->getXmlStr("model_list");
  loadModels(modelFN);

  sceneObjFN  = handle->getXmlStr("sceneobj_list");
  loadSceneObj(sceneObjFN);

  mobFN  = handle->getXmlStr("mob_list");
  loadMob(mobFN);

  partSysFN = handle->getXmlStr("particle_effects_list");
  loadParticleEffects(partSysFN);

  cout << "modelList size from Zone: " << modelList.size() << endl;

  delete handle;
}

Zone::~Zone(){
  cout << "Goodbye zone!" << endl;
}

void Zone::loadModels(string const & fn){
  pair<multimap<string, string>::iterator, multimap<string, string>::iterator> _mrd;
  multimap<string, string>::iterator _ml;

  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();
  HandleChars * handleModelFile;
  parser->parseXML(fn, handle);
  Client * client = Client::getInstance();

  _mrd = handle->findRangeData("model");

  cout << "Loading zone model list file: " << fn << endl;
  for(_ml = _mrd.first; _ml != _mrd.second; _ml++){
     cout << "   Loading model file: " << (*_ml).second << endl;
     handleModelFile = new HandleChars();
     client->getAssets().getModelFactory().getModel((*_ml).second);
     parser->parseXML((*_ml).second, handleModelFile); 
     modelList.push_back((*_ml).second);
     delete handleModelFile;
  }
  delete handle;
}

void Zone::loadSceneObj(string const & fn){
  pair<multimap<string, string>::iterator, multimap<string, string>::iterator> _sld;
  multimap<string, string>::iterator _sl;

  string sofn;
  SceneOBJ * so;

  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();
  parser->parseXML(fn, handle);

  _sld = handle->findRangeData("sceneobj");

  cout << "Loading zone sceneobj list file: " << fn << endl;
  for(_sl = _sld.first; _sl != _sld.second; _sl++){
    cout << "   Loading sceneobj file: " << (*_sl).second << endl;
    so = new SceneOBJ((*_sl).second);
    sceneObjList.push_back(so);
  }
  delete handle;
}

void Zone::loadMob(string const & fn){
  pair<multimap<string, string>::iterator, multimap<string, string>::iterator> _mld;
  multimap<string, string>::iterator _ml;

  string mofn;
  MOB * mo;

  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();
  parser->parseXML(fn, handle);

  _mld = handle->findRangeData("mob");

  cout << "Loading zone mob list file: " << fn << endl;
  for(_ml = _mld.first; _ml != _mld.second; _ml++){
    cout << "   Loading mob file: " << (*_ml).second << endl;
    mo = new MOB((*_ml).second);
    mobList.push_back(mo);
  }
  delete handle;
}

void Zone::loadParticleEffects(string const & fn){
  pair<multimap<string, string>::iterator, multimap<string, string>::iterator> _sld;
  multimap<string, string>::iterator _sl;

  string pefn;
  ParticleEffects * pe;

  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();
  parser->parseXML(fn, handle);

  _sld = handle->findRangeData("particle_effects");

  cout << "Loading particle effects list file: " << fn << endl;
  for(_sl = _sld.first; _sl != _sld.second; _sl++){
    cout << "   Loading particle effects file: " << (*_sl).second << endl;
    pe = new ParticleEffects((*_sl).second);
    particleEffectsList.push_back(pe);
  }
}

void Zone::updateZone(float dT){
  std::vector<ParticleEffects*>::iterator _pe;
  std::list<MOB*>::iterator _pm;
  std::list<Projectile*>::iterator _pr;


  for(_pe = particleEffectsList.begin(); _pe != particleEffectsList.end(); _pe++){
    (*_pe)->update(dT);
  }
  for(_pm = mobList.begin(); _pm != mobList.end(); _pm++){
    (*_pm)->update(dT);
  }
  for(_pr = projectileList.begin(); _pr != projectileList.end(); _pr++){
    (*_pr)->update(dT);
  }

  // Remove dead things
  for(_pm = mobList.begin(); _pm != mobList.end(); _pm++){
    if((*_pm)->getDead()){
      delete (*_pm);
      _pm = mobList.erase(_pm);
    }
  }

  for(_pr = projectileList.begin(); _pr != projectileList.end(); _pr++){
    if((*_pr)->getDead()){
      delete (*_pr);
      _pr = projectileList.erase(_pr);
    }
  }

  if(mobList.size() == 0){
    cout << "You win!!!" << endl;
  }
  
}
