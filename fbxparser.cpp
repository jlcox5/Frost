/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles parsing fbx files and allocating the resources to the
 * appropriate data structuress.
 *
 */

// Standard Libraries
#include <iostream>

// Custom Libraries
#include "fbxparser.h"

using std::cout; using std::endl;

FBXParser * FBXParser::getInstance(){
  if(!inst){ inst = new FBXParser(); }
  return inst;
}

// FBXParser Constructor
FBXParser::FBXParser(): numTabs(0){
  // Init FBX SDK manager
  cout << "Creating lManager" << endl;
  lManager = KFbxSdkManager::Create();
}

// FBXParser Destructor
FBXParser::~FBXParser(){
  // Destroy SDK manager
  cout << "Good bye FBXParser!!!" << endl;
  for(unsigned int i = 0; i < lSceneList.size(); i++){
    lSceneList[i]->Destroy();
  }
  lManager->Destroy();
}

// Load an FBX File into the assets
void FBXParser::loadFBX(string const & s, Assets& assets){

  cout << "Loading fbx file: " << s << endl;
  // IO Settings object

  KFbxIOSettings * ios = KFbxIOSettings::Create(lManager, IOSROOT);
  lManager->SetIOSettings(ios);

  // Establish importer
  KFbxImporter * lImporter = KFbxImporter::Create(lManager, "");

  // File to import
  if(!lImporter->Initialize(s.c_str(), -1, lManager->GetIOSettings())){
    cout << "Failed on creating FBX Importer." << endl;
    cout << "   Error: " << lImporter->GetLastErrorString() << endl;
    exit(1);
  }

  // Create the scene
  KFbxScene * lScene = KFbxScene::Create(lManager, s.c_str());
  lSceneList.push_back(lScene);

  // Import scene
  lImporter->Import(lScene);

  KFbxNode * lRootNode = lScene->GetRootNode();
  traverseElements(assets, *lRootNode, s);

  lImporter->Destroy();

  cout << "   Finished loading fbx file." << endl;
  
}

void FBXParser::traverseElements(Assets & assets, KFbxNode const & n, string const & s) const{
  const KFbxNode * t;

  if(&n){
    for(int i=0; i < n.GetChildCount(); i++){
      t = n.GetChild(i);
      // Test node type
      if( t->GetNodeAttribute()->GetAttributeType() == KFbxNodeAttribute::eMESH ){
        cout << "   Storing mesh.... " << endl;
        storeMesh(*t, assets, s);
        storeTexture(*t, assets);
      }
    }
  }

}

void FBXParser::storeMesh(KFbxNode const & t, Assets & assets, string const & s) const{
  Mesh * m = new FBXMesh(*const_cast<KFbxNode&>(t).GetMesh());
  assets.getMeshFactory().pushMesh(s, m);
}

void FBXParser::storeTexture(KFbxNode const & t, Assets & assets) const{
  assets.loadTextureFBXNode(t);
}

//----- Print Functions -----//

// Print tabs function
void FBXParser::printTabs() const{
  for(int i = 0; i < numTabs; i++){
    cout << "   ";
  }
}

// Find and return the correct attribute value
const KString FBXParser::getAttributeTypeName(KFbxNodeAttribute::EAttributeType type) const{
  switch(type) {
    case KFbxNodeAttribute::eUNIDENTIFIED: return "unidentified";
    case KFbxNodeAttribute::eNULL: return "null";
    case KFbxNodeAttribute::eMARKER: return "marker";
    case KFbxNodeAttribute::eSKELETON: return "skeleton";
    case KFbxNodeAttribute::eMESH: return "mesh";
    case KFbxNodeAttribute::eNURB: return "nurb";
    case KFbxNodeAttribute::ePATCH: return "patch";
    case KFbxNodeAttribute::eCAMERA: return "camera";
    case KFbxNodeAttribute::eCAMERA_STEREO:    return "stereo";
    case KFbxNodeAttribute::eCAMERA_SWITCHER: return "camera switcher";
    case KFbxNodeAttribute::eLIGHT: return "light";
    case KFbxNodeAttribute::eOPTICAL_REFERENCE: return "optical reference";
    case KFbxNodeAttribute::eOPTICAL_MARKER: return "marker";
    case KFbxNodeAttribute::eNURBS_CURVE: return "nurbs curve";
    case KFbxNodeAttribute::eTRIM_NURBS_SURFACE: return "trim nurbs surface";
    case KFbxNodeAttribute::eBOUNDARY: return "boundary";
    case KFbxNodeAttribute::eNURBS_SURFACE: return "nurbs surface";
    case KFbxNodeAttribute::eSHAPE: return "shape";
    case KFbxNodeAttribute::eLODGROUP: return "lodgroup";
    case KFbxNodeAttribute::eSUBDIV: return "subdiv";
    default: return "unknown";
  }
}

// Print an attribute
void FBXParser::printAttribute(KFbxNodeAttribute const & pAttribute) const{
  if(!&pAttribute){ return; }

  KString typeName = getAttributeTypeName(pAttribute.GetAttributeType());
  KString attrName = pAttribute.GetName();
  printTabs();
  cout << "<attribute type= " << typeName.Buffer() << " name= " << attrName.Buffer();
  cout << "/>" << endl;
}

// Print a Node
void FBXParser::printNode(KFbxNode const & n){
  printTabs();
  const char * nodeName = n.GetName();

  fbxDouble3 tra = n.LclTranslation.Get();
  fbxDouble3 rot = n.LclRotation.Get();
  fbxDouble3 sca = n.LclScaling.Get();

  // Print node contents
  cout << "<node name = " << nodeName << "   ";
  cout << "translation = (" << tra[0] << ", " << tra[1] << ", " << tra[2] << ")" << "  ";
  cout << "rotation = (" << rot[0] << ", " << rot[1] << ", " << rot[2] << ")" << "  ";
  cout << "scaling = (" << sca[0] << ", " << sca[1] << ", " << sca[2] << ")" << endl;
  cout << "test 0" << endl;
  cout << "numTabs" << numTabs << endl;
  numTabs++;

  // Print node attributes
  for(int i=0; i < n.GetNodeAttributeCount(); i++){
    printAttribute(*n.GetNodeAttributeByIndex(i));
  }

  // Print children recursively
  for(int j = 0; j < n.GetChildCount(); j++){
    printNode(*n.GetChild(j));
  }

  numTabs--;
  printTabs();
  cout << "</node>" << endl;
}

// Print Scene
void FBXParser::printScene(KFbxScene const &s){
  numTabs = 0;
  cout << "Printing scene..." << endl;
  KFbxNode * lRootNode = s.GetRootNode();

  if(lRootNode){
    for(int i=0; i < lRootNode->GetChildCount(); i++){
      printNode(*lRootNode->GetChild(i));
    }
  }
}

