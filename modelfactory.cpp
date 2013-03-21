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
#include "collada.h"

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

  _m = modelList.find(fn);

  if(_m == modelList.end()){
    parser->parseXML(fn, handle);
    
    // Test for composite collada file
    modelName = handle->getXmlStr("model_name");
    cout << "In Model Factory..." << endl;
    cout << "  Mesh FN: " << meshFN << endl;
    cout << "  Model name: " << modelName << endl;
    meshFN = handle->getXmlStr("composite_file");
    if(meshFN == "NOFILE"){
      meshFN = handle->getXmlStr("mesh_file");
      found = meshFN.find_last_of(".");
      cout << "In Model Factory..." << endl;
      cout << "  Mesh FN: " << meshFN << endl;
      cout << "  Model name: " << modelName << endl;
      if(meshFN == "NOFILE"){
        return NULL;
      }
      if(found == static_cast<unsigned int>(-1)){
        cout << "  Invalid filename given to model factory: " << meshFN << endl;
        exit(1);
      }
      Mesh * mesh;
      mesh = Client::getInstance()->getAssets().getMeshFactory().getMesh(meshFN);
      modelList[fn] = new Model(*mesh);
      ret = modelList[fn];
    }
    else{
      // Collada only handles polymesh
      cout << "Loading composite file: " << meshFN << endl;
      PolyMesh * mesh;
      Texture * text;
      Animate * anim;
      Skeleton * skel;
      
      Collada  col(meshFN);
      mesh = col.buildPolyMesh();
      text = col.buildTexture();
      anim = col.buildAnimate();
      skel = col.buildSkeleton();

      Client::getInstance()->getAssets().getMeshFactory().pushMesh(meshFN, mesh);
      Client::getInstance()->getAssets().getTextureManager().add(*text);

      modelList[fn] = new Model(*mesh, *text, *anim, *skel);
      ret = modelList[fn];
    }
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

