/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the mesh list.
 *
 */
 
// Custom Libraries
#include "client.h"
#include "meshfactory.h"
#include "lsystem.h"
#include "fbxparser.h"
#include "collada.h"

MeshFactory::~MeshFactory(){
  map<string, Mesh*>::iterator _m;

  for(_m = meshList.begin(); _m != meshList.end(); _m++){
    delete _m->second;
  }

  meshList.clear();
}

Mesh * MeshFactory::getMesh(string const & fn){
  map<string, Mesh*>::iterator _m;
  Mesh * ret;

  string ext;
  size_t found;

  _m = meshList.find(fn);

  cout << "Loading mesh: " << fn << endl;

  if(_m == meshList.end()){
    cout << "   Mesh not found: " << fn << endl;
    found = fn.find_last_of(".");
    if(found == static_cast<unsigned int>(-1)){
      cout << "Invalid filename given to mesh factory: " << fn << endl;
      exit(-1);
    }
    ext = fn.substr(found+1);
    // Handle fbx file format
    if(ext.compare("fbx") == 0){
      FBXParser * fbx;
      fbx = FBXParser::getInstance();
      fbx->loadFBX(fn, Client::getInstance()->getAssets());
      // Mesh is pushed to meshList from FBXParser.  This might need to be revisited.
      _m = meshList.find(fn);
      if(_m == meshList.end()){
        cout << "FBX Parser failed to push mesh: " << fn << endl;
      }
      return _m->second;
    }
    // Handle lsystem file
    else if(ext.compare("xml") == 0){
      LSystem * l;
      PolyMesh * p;
      l = new LSystem("lsystem/test.xml");
      p = l->buildMesh();
      meshList[fn] = p;
      delete l;
      return p;
    }
    // Handle collada files -> Only file type that I have animation support for
    else if(ext.compare("dae") == 0){
      PolyMesh * p;
      Collada * col;
      col = new Collada(fn);
      p = col->buildPolyMesh();
      meshList[fn] = p;
      delete col;
      return p;
    }
    else{
      cout << "Unrecognized file format.  Current recognized types are xml, collada, and fbx: " << endl;
      cout << "    " << fn << endl;
      exit(-1);
    }
  }
  else{
    cout << "   Mesh found: " << fn << endl;
    ret = _m->second;
  }

  return ret;
}

bool MeshFactory::eraseMesh(string const & fn){
  int i = meshList.erase(fn);
 
  if(i == 0){
    cout << "Mesh not found: " << fn << endl;
    return false;
  }

  return true;
}

void MeshFactory::pushMesh(string const & fn, Mesh * m){
  cout << "Storing fbx mesh: " << fn << "  at address: " << m << endl;
  meshList[fn] = m;
}

