/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the models.
 *
 */
 
// Custom Libraries
#include "client.h"
#include "fbxparser.h"
#include "modelfactory.h"

ModelFactory::ModelFactory(): modelList(map<string, Model*>()){
}

ModelFactory::~ModelFactory(){
  map<string, Model*>::iterator _m;

  for(_m = modelList.begin(); _m != modelList.end(); _m++){
    delete (*_m).second;
  }

  modelList.clear();
}

Model * ModelFactory::getModel(string const & fn){
  map<string, Model*>::iterator _m;
  Model * ret;
  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();
  string meshFN, modelName;

  string ext;
  size_t found;

  cout << "Looking for model: " << fn << endl;

  _m = modelList.find(fn);

  if(_m == modelList.end()){
    parser->parseXML(fn, handle);
    meshFN = handle->getXmlStr("mesh_file");
    modelName = handle->getXmlStr("model_name");
    found = meshFN.find_last_of(".");
    cout << "Mesh FN: " << meshFN << endl;
    cout << "Model name: " << modelName << endl;
    if(found == static_cast<unsigned int>(-1)){
      cout << "Invalid filename given to model factory: " << meshFN << endl;
      exit(-1);
    }
    Mesh * mesh;
    mesh = Client::getInstance()->getAssets().getMeshFactory().getMesh(meshFN);
    modelList[fn] = new Model(*mesh);
    ret = modelList[fn];
  }
  else{
    ret = _m->second;
  }

  return ret;
}

bool ModelFactory::eraseModel(string const & fn){
  Model * toDel = getModel(fn);
  delete toDel;

  int i = modelList.erase(fn);
 
  if(i == 0){
    cout << "Mesh not found: " << fn << endl;
    return false;
  }

  return true;
}

void ModelFactory::pushModel(string const & fn, Model * m){
  modelList[fn] = m;
}

