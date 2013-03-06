/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles asset orginazation.
 *
 */

// Standard Libraries
#include <iostream>
#include <string>

// Added Libraries
#include "targa.h"

// Custom Libraries
#include "assets.h"
#include "fbxparser.h"
#include "lsystem.h"

using std::cout; using std::endl;

// Constructor //
Assets::Assets(): meshFact(), modelFact(), textMan(), particleFact(){
  cout << "Initializing Assets.... " << endl;  
}

// Destructor //
Assets::~Assets(){
  cout << "Goodbye assets!" << endl;
}

void Assets::loadAssets(const string& s){
  FBXParser * fbx;
  fbx = FBXParser::getInstance();
  fbx->loadFBX(s, *this);
}

void Assets::addTexture(Texture& t){
  textMan.add(t);
}

void Assets::loadTextureFBXNode(KFbxNode const &t){
  textMan.loadTextureFBXNode(t);
}

void Assets::setTexture(KFbxNode const & n, int i){
  textMan.setTexture(n, i);
}

void Assets::rebuildTree(){
  if(meshFact.eraseMesh("lsystem/test.xml") == true){
    meshFact.getMesh("lsystem/test.xml");
  }
  rebuildTreeModel();
}

// Fix.... maybe?

void Assets::rebuildTreeModel(){
  /*std::vector<Model*>::iterator _m = modelList.begin()+2;

  for(_m = modelList.begin()+2; _m != modelList.end(); _m++){
    (*_m)->setMesh(meshFact.getMesh("lsystem/test.xml"));
  }*/
}
