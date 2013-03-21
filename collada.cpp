/*  Jonathan Cox
 *  Clemson University
 *
 *  This class handles loading the collada file and stripping out the desired data.
 */

// Standard Libs
#include <iostream>
#include <string>
#include <sstream>
#include <stack>

// Imported Libs
#include "Eigen/Dense"

// Custom Libs
#include "collada.h"
#include "client.h"
#include "texturemanager.h"
#include "texture.h"
#include "joint.h"
#include "skeleton.h"

using std::cout; using std::endl;
using std::string; using std::stringstream;

Collada::Collada(){

  // Open file and load root element
  root = collada_dom.open("assets/spine.dae");

  if(root){
    cout << "File loaded ok." << endl;
  }
  else if(!root){
    cout << "File not loaded... is the path correct?" << endl;
    exit(1);
  }

  PNT = 0;
  loadElements();
  collada_dom.close("assets/spine.dae");
}

Collada::Collada(string const & s){

  // Open file and load root element
  cout << "Opening collada file " << s << endl;
  root = collada_dom.open(s);

  if(root){
    cout << "File loaded ok." << endl;
  }
  else if(!root){
    cout << "File not loaded... is the path correct?" << endl;
    exit(1);
  }

  PNT = 0;
  loadElements();
  collada_dom.close(s);
}

Collada::~Collada(){
  cout << endl << "Deleting DAE - I hope you were done!" << endl;

  // -- Uncommenting this line causes a seg fault for some reason... should investigate -- //
  //delete root;
}

void Collada::loadElements(){

  //Load the mesh
  loadMesh();
  loadTexture();
  loadJoints();
  loadWeights();
  loadAnimation();

  return;
}

void Collada::loadMesh(){
  daeElement * node;
  daeElement * curSource;
  string meshId;
  string meshSource;
  string tempId;

  cout << "Pulling mesh..." << endl; 
  node = root->getDescendant("library_geometries");
  if(!testNode(node, "library_geometries")){ exit(1); }

  meshId = node->getChild("geometry")->getAttribute("id").data();

  cout << "   MeshId: " << meshId << endl;

  curSource = collada_dom.getDatabase()->idLookup(meshId+"-positions", root->getDocument());
  if(!testNode(curSource, meshSource.c_str())){ exit(1); }
  PNT = 1;
  readFloatData3f(meshId, "-positions"); 

  curSource = collada_dom.getDatabase()->idLookup(meshId+"-normals", root->getDocument());
  if(testNode(curSource, meshSource.c_str())){
    PNT = 2;
    readFloatData3f(meshId, "-normals");
  } 
  curSource = collada_dom.getDatabase()->idLookup(meshId+"-map-0", root->getDocument());
  if(testNode(curSource, meshSource.c_str())){
    if(PNT == 2){
      PNT = 3;
    }
    else{
      PNT = 4;
    }
    readFloatData2f(meshId, "-map-0");
  } 

  readVertexData(meshId);

  cout << "Points size: " << points->size() << endl;
  cout << "Normals size: " << normals->size() << endl;
  cout << "TextCood size: " << texCoords->size() << endl;
}

void Collada::loadTexture(){
  daeElement * node;
  daeElement * curSource;
  string textId;

  Magick::Image image;

  unsigned int lTextObj = 0;
  size_t width = 0;
  size_t height = 0;
  char * imgData;

  cout << "Pulling texture..." << endl;
  node = root->getDescendant("library_images");
  if(!testNode(node, "library_images")){ exit(1); }

  curSource = node->getDescendant("init_from");
  textId = curSource->getCharData();

  cout << "   Found texture file: " << textId << endl;

  image.read("assets/" + textId);
  image.flip();
  width = image.columns();
  height = image.rows();

  imgData = (char*)malloc(width*height*4);

  image.write(0, 0, width, height, "RGBA", MagickCore::CharPixel, imgData);

  glGenTextures(1, &lTextObj);
  glBindTexture(GL_TEXTURE_2D, lTextObj);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

  free(imgData);

  // Create texture, push texture to texture manager, bada bing bada boom
  textID = lTextObj;
}

void Collada::loadJoints(){
  daeElement * node;
  daeElement * curSource;
  daeElement * curSkel;

  daeTArray<daeElementRef> children;
  daeTArray<daeElementRef> skeleton;

  Joint * rootJoint;

  string id;
  string floatData;
  size_t found;
  unsigned int matrixSize = 16;

  cout << "Loading Joints" << endl;

  skel = new Skeleton();

  node = root->getDescendant("library_visual_scenes");
  if(!testNode(node, "library_visual_scenes")){ exit(1); }
  node = node->getDescendant("visual_scene");
  if(!testNode(node, "visual_scene")){ exit(1); }

  children = node->getChildren();
  for(unsigned int i=0; i < children.getCount(); i++){
    curSource = children[i];

    if(curSource->getAttribute("type") == "NODE"){
      id = "Armature"; 
      found = curSource->getAttribute("id").find(id);
      // Found an armature node, now try to find the joint info
      if(found != string::npos){
        cout << "     Found Armature: " << curSource->getAttribute("id") << endl;

        // Step through nodes to find joint and skeleton structure
        curSkel = curSource->getDescendant("node");
        skeleton = curSource->getChildren();
        for(unsigned int j=0; j < skeleton.getCount(); j++){
          curSkel = skeleton[j];
          if(curSkel->getAttribute("type") == "JOINT"){
            string jointId(curSkel->getAttribute("id"));
            cout << "       Found A Root Joint: " << jointId << endl;
            rootJoint = new Joint(jointId);
            skel->pushRoot(rootJoint); 
            processJoint(rootJoint, curSkel);
          }
        }
      }
    }
  }

  // Load Joints Array
  node = root->getDescendant("library_controllers");
  if(!testNode(node, "library_controllers")){ exit(1); }
  node = node->getDescendant("controller");
  if(!testNode(node, "controller")){ exit(1); }

  node = node->getDescendant("Name_array");
  if(!testNode(node, "Name_array")){ exit(1); }
  id = node->getAttribute("id");
  cout << "Found Joint Name Array: " << id << endl;

  string jointNameData = node->getCharData();
  jointArrayCollada = new std::vector<string>();
  string testStr;
  size_t len = 0;

  cout << "Joint name data: " << jointNameData << endl;

  // Tokenize joint name data
  len = jointNameData.find(" ");
  testStr = jointNameData.substr(0, len);
  jointArrayCollada->push_back(testStr);

  while(len != string::npos){
    jointNameData = jointNameData.substr(len+1);
    len = jointNameData.find(" ");
    testStr = jointNameData.substr(0, len);
    jointArrayCollada->push_back(testStr);
  }

  // Load Bind Shape Matrix
  node = root->getDescendant("library_controllers");
  if(!testNode(node, "library_controllers")){ exit(1); }
  node = node->getDescendant("controller");
  if(!testNode(node, "controller")){ exit(1); }
  node = node->getDescendant("skin");
  if(!testNode(node, "skin")){ exit(1); }
  
  node = node->getDescendant("bind_shape_matrix");
  if(!testNode(node, "bind_shape_matrix")){ exit(1); };

  floatData = node->getCharData();
  stringstream stm(floatData);
  bindShapeMatrix = new Matrix4f();
  for(unsigned int i=0; i < matrixSize; i++){
    float t;
    stm >> t;
    bindShapeMatrix->row(i/4).col(i%4) << t;
  }
  bindShapeMatrix->transposeInPlace();
  stm.flush();

  // Load Inverse Matricies
  node = root->getDescendant("library_controllers");
  if(!testNode(node, "library_controllers")){ exit(1); }
  node = node->getDescendant("controller");
  if(!testNode(node, "controller")){ exit(1); }
  id = node->getAttribute("id");

  node = node->getDescendant("skin");
  if(!testNode(node, "skin")){ exit(1); }
  children = node->getChildren();
  id = id + "-bind_poses";

  Matrix4f * m;
  jointInverseBind = new std::vector<Matrix4f *, aligned_allocator<Matrix4f *> >();

  for(unsigned int i=0; i < children.getCount(); i++){
    if(children[i]->getAttribute("id") == id){
      cout << "Reading inverse from " << id << endl;
      floatData = children[i]->getChild("float_array")->getCharData();
      cout << "floatData: " << floatData << endl;
      stringstream stm2(floatData);
      for(unsigned int i=0; i < jointArrayCollada->size(); i++){
        m = new Matrix4f();
        for(unsigned int j=0; j < matrixSize; j++){
          float t;
          stm2 >> t;
          m->row(j/4).col(j%4) << t;
        }
        m->transposeInPlace();
        cout << "Pushing inverse: " << endl;
        cout << *(m) << endl;
        jointInverseBind->push_back(m);
      } 
    }
  }
}

void Collada::loadWeights(){
  daeElement * node;
  daeElement * curSource;
  daeElement * tempNode;

  string id, rawData;
  int count, count2;

  cout << "Loading weights..." << endl;

  node = root->getDescendant("library_controllers");
  if(!testNode(node, "library_controllers")){ exit(1); }

  node = node->getDescendant("controller");
  if(!testNode(node, "controller")){ exit(1); }
  
  id = node->getAttribute("id");
  cout << "   Found controller: " << id << endl;
  node = node->getChild("skin");

  // Probably a better way to find this node... just need to look it up
  curSource = collada_dom.getDatabase()->idLookup(id+"-weights", root->getDocument());
  if(!testNode(curSource, (id+"-weights").c_str())){ exit(1); }

  // Pull weights
  cout << "     Found weights array: " << curSource->getAttribute("id") << endl;
  tempNode = curSource->getChild("float_array");
  if(!testNode(tempNode, "float_array")){ exit(1); }

  // Read in weights
  count = atoi(tempNode->getAttribute("count").data());
  rawData = tempNode->getCharData();
  stringstream stm(rawData);
  cout << "       Weights: " << endl;
  weightList = new std::vector<float>();
  for(int j=0; j<count; j++){
    float t = 0;
    stm >> t;
    weightList->push_back(t);
    cout << t << " ";
  }
  cout << endl;

  curSource = node->getChild("vertex_weights");
  if(!testNode(curSource, "vertex_weights")){ exit(1); }

  tempNode = curSource->getChild("vcount");
  if(!testNode(tempNode, "vcount")){ exit(1); }

  // Read in weights per vertex
  count = atoi(curSource->getAttribute("count").data());
  rawData = tempNode->getCharData();
  stringstream stm2(rawData);
  cout << "       Weights Per Vertex: " << endl;
  weightsPerVertex = new std::vector<int>();
  count2 = 0;
  for(int j=0; j<count; j++){
    int t = 0;
    stm2 >> t;
    weightsPerVertex->push_back(t);
    cout << t << " ";
    count2 += t;
  }
  cout << endl;

  cout << "count2: " << count2 << endl;
  count2 = count2*2;

  tempNode = curSource->getChild("v");
  if(!testNode(tempNode, "v")){ exit(1); } 

  // Read in weights list index
  rawData = tempNode->getCharData();
  stringstream stm3(rawData);
  cout << "      Weight List Index: " << endl;
  weightListIndex = new std::vector<int>();
  for(int j=0; j<count2; j++){
    int t = 0;
    stm3 >> t;
    weightListIndex->push_back(t);
    cout << t << " ";
  }
  cout << endl;

  cout << "Finished loading weights." << endl;
}

void Collada::loadAnimation(){
  daeElement * node;
  daeElement * curAnim;
  daeElement * curChild;
  daeElement * data;

  daeTArray<daeElementRef> animations;
  daeTArray<daeElementRef> curAnimChildren;

  string id;
  string floatData;

  int count = 0;
  int matrixSize = 16;

  Matrix4f * m;

  cout << "*** Processing Animation Data *** " << endl;

  node = root->getChild("library_animations");
  if(!testNode(node, "library_animations")){ exit(1); }

  animations = node->getChildren();

  jointKeyFrames = new std::vector< std::vector<float> *>();
  jointPoses = new std::vector< std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > *>();

  std::vector<float> * jkf;
  std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > * jp;
  for(unsigned int i=0; i < animations.getCount(); i++){
    curAnim = animations[i];
    id = curAnim->getAttribute("id");
    curAnimChildren = curAnim->getChildren(); 
    cout << "   Processing Animation: " << id << endl;
    for(unsigned int j=0; j < curAnimChildren.getCount(); j++){
      curChild = curAnimChildren[j];
      // Get time values for each keyframe
      if(curChild->getAttribute("id") == (id + "-input")){
        jkf = new std::vector<float>();
        data = curChild->getDescendant("float_array");
        if(!testNode(data, "float_array")){ exit(1); }

        count = atoi((data->getAttribute("count")).c_str());
        floatData = data->getCharData();
        stringstream stm1(floatData);
        //cout << "      keytimes: ";
        for(int k=0; k < count; k++){
          float t = 0;
          stm1 >> t;
          jkf->push_back(t);
          //cout << " " << t;
        }
        //cout << endl;
        stm1.flush();
        jointKeyFrames->push_back(jkf);
      }
      // Get transformation matrix for each keyframe
      if(curChild->getAttribute("id") == (id + "-output")){
        jp  = new std::vector<Matrix4f *, aligned_allocator<Matrix4f *> >();
        data = curChild->getDescendant("float_array");
        if(!testNode(data, "float_array")){ exit(1); }

        floatData = data->getCharData();
        stringstream stm1(floatData);
        for(int k=0; k < count; k++){
          m = new Matrix4f();
          //cout << "      matrix " << k << ": ";
          for(int l=0; l < matrixSize; l++){
            float t = 0;
            stm1 >> t;
            m->row(l/4).col(l%4) << t;
            //cout << " " << t;
          }
          //cout << endl;
          stm1.flush();
          m->transposeInPlace();
          jp->push_back(m);
        } 
        jointPoses->push_back(jp);
      }
    }
  }

}

void Collada::processJoint(Joint * parent, daeElement * parentNode){
  daeTArray<daeElementRef> children;
  daeElement * childNode;
  Joint * childJoint;

  children = parentNode->getChildren(); 

  childNode = parentNode->getChild("matrix");

  if(testNode(childNode, "matrix")){
    readMatrixData4(parent, childNode);
  }

  for(unsigned int i=0; i < children.getCount(); i++){
    childNode = children[i];
    if(childNode->getAttribute("type") == "JOINT"){
      string id = childNode->getAttribute("id");
      childJoint = new Joint(id);
      parent->addChild(childJoint);
      childJoint->setParent(parent);
      skel->addJoint(parent, childJoint);
      cout << "          pushed joint: " << childNode->getAttribute("id") << endl;
      processJoint(childJoint, childNode); 
    }
  }
}

// -- Helper functions -- //

void Collada::readFloatData3f(string const s1, string const s2){
  daeElement * curSource;
  daeElement * floatArray;
  string meshId;
  string meshSource;
  string tempId;
  int count;
  int stride;

  Vector3f * point;
  std::vector<Vector3f*> * vecNew;

  vecNew = new std::vector<Vector3f*>();

  if(s2 == "-positions"){
    points = vecNew;
  }
  if(s2 == "-normals"){
    normals = vecNew;
  }

  // Find source node that handles position data
  meshSource = s1 + s2;
  cout << "   Mesh Source: " << meshSource << endl;
  curSource = collada_dom.getDatabase()->idLookup(meshSource, root->getDocument());
  if(!testNode(curSource, meshSource.c_str())){ exit(1); }

  floatArray = curSource->getChild("float_array");

  // Find node with float data
  if(!testNode(floatArray, "float_array")){ exit(1); }
  tempId = floatArray->getAttribute("id").data();
  cout << "   Float Array ID: " << tempId << endl;
  count = atof(floatArray->getAttribute("count").data());

  // Check stride to make sure we're looking at vectors
  stride = atoi(curSource->getChild("technique_common")->getChild("accessor")->
                           getAttribute("stride").data());
  if(stride != 3){
    cout << "   Aborting... stride is not 3 for " << s2 << ": " << stride << endl;
    exit(1);
  }
  tempId = floatArray->getCharData();
  stringstream stm(tempId);

  // Read each float and add to points
  for(int i = 0; i < count/stride; i++){
    float x, y, z = 0;
    x = y = z;
    point = new Vector3f();
    stm >> x;
    stm >> y;
    stm >> z;
    
    (*point) << x, y, z;
    //cout << "       (" << (*point)(0) << ", " << (*point)(1) 
    //     << ", " << (*point)(2) << ")" << endl;

    vecNew->push_back(point);
  }

  cout << "       Final size of " << s2 << " vector: " << vecNew->size() << endl;
}


void Collada::readFloatData2f(string const s1, string const s2){
  daeElement * curSource;
  daeElement * floatArray;
  string meshId;
  string meshSource;
  string tempId;
  int count;
  int stride;

  Vector2f * point;
  std::vector<Vector2f*> * vecNew;

  vecNew = new std::vector<Vector2f*>();

  if(s2 == "-map-0"){
    texCoords = vecNew;
  }

  // Find source node that handles position data
  meshSource = s1 + s2;
  cout << "   Mesh Source: " << meshSource << endl;
  curSource = collada_dom.getDatabase()->idLookup(meshSource, root->getDocument());
  if(!testNode(curSource, meshSource.c_str())){ exit(1); }

  floatArray = curSource->getChild("float_array");

  // Find node with float data
  if(!testNode(floatArray, "float_array")){ exit(1); }floatArray->getCharData();
  tempId = floatArray->getAttribute("id").data();
  cout << "   Float Array ID: " << tempId << endl;
  count = atof(floatArray->getAttribute("count").data());

  // Check stride to make sure we're looking at vectors
  stride = atoi(curSource->getChild("technique_common")->getChild("accessor")->
                           getAttribute("stride").data());
  if(stride != 2){
    cout << "   Aborting... stride is not 2 for " << s2 << ": " << stride << endl;
    exit(1);
  }
  tempId = floatArray->getCharData();
  stringstream stm(tempId);

  // Read each float and add to points
  for(int i = 0; i < count/stride; i++){
    float x, y, z = 0;
    x = y = z;
    point = new Vector2f();
    stm >> x;
    stm >> y;
    
    (*point) << x, y;
    //cout << "       (" << (*point)(0) << ", " << (*point)(1) << ")" <<  endl; 

    vecNew->push_back(point);
  }

  cout << "       Final size of " << s2 << " vector: " << vecNew->size() << endl;
}

void Collada::readVertexData(string const s){
  daeElement * curSource;
  daeTArray<daeElementRef> errorCheck;
  daeElement * vcount;
  daeElement * p;

  int count;
  string rawData;

  std::vector<int> * pl;
  std::vector<int> * nl;
  std::vector<int> * tl;

  cout << "Reading Vertex Indeces" << endl;

  curSource = collada_dom.getDatabase()->idLookup(s, root->getDocument());
  curSource = curSource->getChild("mesh")->getChild("polylist");
  if(!testNode(curSource, "polylist")){
    curSource = collada_dom.getDatabase()->idLookup(s, root->getDocument());
    curSource = curSource->getChild("triangles");
    if(!testNode(curSource, "triangles")){ exit(1); }
  }

  errorCheck = curSource->getChildren();
  if(errorCheck.getCount() < 3 || errorCheck.getCount() > 5){
    cout << "Some wierd stuff going on in polylist/triangles node " << s << endl;
  }

  count = atoi(curSource->getAttribute("count").data());

  // Test to make sure that no polygons have more than three verticies
  vcount = curSource->getChild("vcount");
  if(!testNode(vcount, "vcount")){ exit(1); }
  rawData = vcount->getCharData();
  stringstream stm_a(rawData);
  for(int i = 0; i < count; i++){
    int t = 0;
    stm_a >> t;
    if(t != 3){
      cout << "*** Error: Model contains non-triangle polygons. ***" << endl;
      exit(1);
    }
  }

  polyIndex = new std::list<std::vector<int >*>();
  if(PNT > 1){
    if(PNT != 4){
      normalIndex = new std::vector<std::vector<int >*>();
    }
    if(PNT > 2){
      texIndex = new std::vector<std::vector<int >*>();
    }
  }

  // Get actual vertex indicies
  p = curSource->getChild("p");
  if(!testNode(p, "p")){ exit(1); } 
  rawData = p->getCharData();
  stringstream stm_b(rawData);
  for(int i = 0; i < count; i++){
    int x = 0; int y = 0; int z = 0;
    int nx = 0; int ny = 0; int nz = 0;
    int tx = 0; int ty = 0; int tz = 0;

    stm_b >> x;
    if(PNT == 2 || PNT == 3){ stm_b >> nx; }
    if(PNT == 3 || PNT == 4){ stm_b >> tx; }

    stm_b >> y;
    if(PNT == 2 || PNT == 3){ stm_b >> ny; }
    if(PNT == 3 || PNT == 4){ stm_b >> ty; }

    stm_b >> z;
    if(PNT == 2 || PNT == 3){ stm_b >> nz; }
    if(PNT == 3 || PNT == 4){ stm_b >> tz; }

    pl = new std::vector<int>();
    if(PNT == 2 || PNT == 3){ nl = new std::vector<int>(); }
    if(PNT == 3 || PNT == 4){ tl = new std::vector<int>(); }

    pl->push_back(x);
    pl->push_back(y);
    pl->push_back(z);

    if(PNT == 2 || PNT == 3){ 
      nl->push_back(nx);
      nl->push_back(ny);
      nl->push_back(nz);
    }
    if(PNT == 3 || PNT == 4){
      tl->push_back(tx);
      tl->push_back(ty);
      tl->push_back(tz);
    }

    polyIndex->push_back(pl);
    if(PNT == 2 || PNT == 3){ normalIndex->push_back(nl); }
    if(PNT == 3 || PNT == 4){ texIndex->push_back(tl); }

    //cout << "     PL: ";
    //cout << "  (" << (*pl)[0] << ", " << (*pl)[1] 
    //     << ", " << (*pl)[2] << ")" << endl;
    if(PNT == 2 || PNT == 3){
    //  cout << "     NL: ";
    //  cout << "  (" << (*nl)[0] << ", " << (*nl)[1] 
    //       << ", " << (*nl)[2] << ")" << endl;
    }
    if(PNT == 3 || PNT == 4){
    //  cout << "     TL: ";
    //  cout << "  (" << (*tl)[0] << ", " << (*tl)[1] 
    //       << ", " << (*tl)[2] << ")" << endl;
    }

  }
}

void Collada::readMatrixData4(Joint * j, daeElement * curNode){
  unsigned int count = 16; // 4x4 Matrix

  string rawData = curNode->getCharData();

  stringstream stm(rawData);

  Matrix4f m;

  for(unsigned int i=0; i < count; i++){
    float t = 0;
    stm >> t;
    m.row(i/4).col(i%4) << t;
  }
  cout << "Setting matrix for: " << j->getId() << endl;
  cout << m << endl;
  m.transposeInPlace();
  j->setMatrix(m);
}

PolyMesh * Collada::buildPolyMesh(){
  PolyMesh * p;

  // Map weights from collada format to my format.
  std::vector<int>::iterator _wpv;
  std::vector<int>::iterator _wli;
  std::vector<float>::iterator _wl;
  std::vector<Joint*>::iterator _jl;
  std::vector<string>::iterator _jlc;

  std::vector<int> * jointMap;

  std::vector<Joint*> fullJointList = skel->getFullJointList();

  //Map index values from the skeleton's joint list to collada's
  if(fullJointList.size() != jointArrayCollada->size()){
    cout << "Joint list sizes do not match." << endl;

   cout << "full joint list:" << endl;
   for(unsigned int i=0; i < fullJointList.size(); i++){
     cout << " " << (fullJointList[i])->getId();
   }
   cout << endl;
   cout << "full collad joint list:" << endl;
   for(unsigned int i=0; i < jointArrayCollada->size(); i++){
     cout << " " << jointArrayCollada->at(i);
   }
   cout << endl;
    
    exit(1);
  }

  jointMap = new std::vector<int>();
  for(_jlc = jointArrayCollada->begin(); _jlc != jointArrayCollada->end(); _jlc++){
    int i = 0;
    for(_jl = fullJointList.begin(); _jl != fullJointList.end(); _jl++){
      if((*_jl)->getId() == (*_jlc)){
        jointMap->push_back(i);
        break;
      }
      i++;
    }
  }
  

  cout << "Joint List Mapping" << endl;
  for(_jlc = jointArrayCollada->begin(); _jlc != jointArrayCollada->end(); _jlc++){
    cout << (*_jlc) << " ";
  }
  cout << endl;
  for(_jl = fullJointList.begin(); _jl != fullJointList.end(); _jl++){
    cout << (*_jl)->getId() << " ";
  }
  cout << endl;
  for(_wli = jointMap->begin(); _wli != jointMap->end(); _wli++){
    cout << (*_wli) << " ";
  }
  cout << endl;


  cout << "Joint List size: " << fullJointList.size() << endl;
  cout << "   Skeleton Joint Cout: " << skel->getJointCount() << endl;

  // Create vectors that are used in the polyMesh structure to represent weights
  int totalCount = 0;
  std::vector<std::vector<int> *> * jointIndex = new std::vector<std::vector<int> *>();
  std::vector<std::vector<int> *> * weightIndex = new std::vector<std::vector<int> *>();
  std::vector<int> * ji;
  std::vector<int> * wi;
  for(_wpv = weightsPerVertex->begin(); _wpv != weightsPerVertex->end(); _wpv++){
    int i = (*_wpv);
    ji = new std::vector<int>();
    wi = new std::vector<int>();
    for(int j = 0; j < i; j++){
      ji->push_back(weightListIndex->at(totalCount*2));
      wi->push_back(weightListIndex->at((totalCount*2)+1));
    }
    jointIndex->push_back(ji);
    weightIndex->push_back(wi);
    totalCount += i;
  }

  // Debug printing... can remove at a later point, or at least wrap in
  // a conditional debug statement
  std::vector<std::vector<int> *>::iterator _li;
  std::vector<int>::iterator _ji;
  cout << "*** Joint Indexes Per Vertex Loaded and Mapped ***" << endl;
  int count = 0;
  for(_li = jointIndex->begin(); _li != jointIndex->end(); _li++){
    //cout << " " << count << ": ";
    for(_ji = (*_li)->begin(); _ji != (*_li)->end(); _ji++){
      //cout << " " << (*_ji);
    }
    //cout << endl;
    count++;
  }
  cout << "*** Weight Indexes Per Vertex Loaded and Mapped ***" << endl;
  count = 0;
  for(_li = weightIndex->begin(); _li != weightIndex->end(); _li++){
    //cout << " " << count << ": ";
    for(_ji = (*_li)->begin(); _ji != (*_li)->end(); _ji++){
      //cout << " " << (*_ji);
    }
    //cout << endl;
    count++;
  }
  
  p = new PolyMesh(points, polyIndex, normals, normalIndex, texCoords, texIndex, jointIndex,
                   weightIndex, weightList, jointMap, bindShapeMatrix, jointInverseBind, 
                   (float)1.0);
  return p;
}

Texture * Collada::buildTexture(){
  Texture * toReturn = new Texture(textID);
  return toReturn;
}

Animate * Collada::buildAnimate(){
  cout << "Sending joint keyframesize: " << jointKeyFrames->size() << endl;
  Animate * toReturn = new Animate(jointKeyFrames, jointPoses);
  return toReturn;
}

Skeleton * Collada::buildSkeleton(){
  skel->mapJointInverseBind(jointArrayCollada, jointInverseBind);
  return skel;
}

bool Collada::testNode(daeElement const * const node, const char * s){
  if(!node){
    cout << "***  Collada node failed on: " << s << " ***" << endl;
    return false;
  }
  return true;
}
