/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles mesh objects.
 *
 */

// Standard Libraries
#include <iostream>
#include <math.h>

// Custom Libraries
#include "fbxmesh.h"
#include "assets.h"
#include "client.h"

using std::cout; using std::endl;


FBXMesh::FBXMesh(){

}

FBXMesh::~FBXMesh(){
  cout << "Goodbye FBXMesh!" << endl;
  delete mesh;
  glDeleteLists(index, 1);
}

FBXMesh::FBXMesh(KFbxMesh & m){
  mesh = &m;

  cout << "UV Count: " << mesh->GetElementUVCount() << endl;
  cout << "Normal Count: " << mesh->GetElementNormalCount() << endl;
  cout << "Material Count: " << mesh->GetElementMaterialCount() << endl;
  if(mesh->GetElementMaterialCount() > 0){
    KFbxGeometryElementMaterial* lMaterialElement = mesh->GetElementMaterial(0);
    if( lMaterialElement->GetMappingMode() == KFbxGeometryElement::eALL_SAME){
      cout << "All Same!" << endl;
    }
  }

  KStringList lUVNames;
  mesh->GetUVSetNames(lUVNames);
  cout << "UV Names: " << lUVNames[0] << endl;

  buildBoundSphere(-1);
  buildBoundBox();
}

void FBXMesh::drawMesh() const{
  const int lVertexCount = mesh->GetControlPointsCount();
  KFbxVector4 * lVertexArray = new KFbxVector4[lVertexCount];
  memcpy(lVertexArray, mesh->GetControlPoints(),  lVertexCount*sizeof(KFbxVector4));

  Assets & assets = Client::getInstance()->getAssets();

  const int pc = mesh->GetPolygonCount();
  KFbxLayerElementArrayTemplate<int>* lMatInd = NULL;

  KStringList lUVNames;
  mesh->GetUVSetNames(lUVNames);

  KFbxVector2 lCurrentUV;
  KFbxVector4 lCurrentNormal;

  bool hasUV, hasNorm;

  hasUV = mesh->GetElementNormalCount() > 0;
  hasNorm = mesh->GetElementNormalCount() > 0;

  if(mesh->GetElementMaterial()){
    mesh->GetMaterialIndices(&lMatInd);
    assets.setTexture(*mesh->GetNode(), lMatInd->GetAt(0)); 
  }

  for(int i = 0; i < pc; i++){
    const int vc = mesh->GetPolygonSize(i);
    if(i != 3){
       //cout << "Someone isn't using triangles..... shame!" << endl;
    }
    glColor4f(0.0f, 0.0f, 0.75f, 1.0f);
    glBegin(GL_POLYGON);
    for(int j = 0; j < vc; j++){
      //cout << lVertexArray[mesh->GetPolygonVertex(i, j)][0] << " ";
      //cout << lVertexArray[mesh->GetPolygonVertex(i, j)][1] << " ";
      //cout << lVertexArray[mesh->GetPolygonVertex(i, j)][2] << endl;
      if(hasUV && mesh->GetPolygonVertexUV(i, j, lUVNames[0].Buffer(), lCurrentUV)){
        glTexCoord2dv(lCurrentUV.mData);
      }
      if(hasNorm && mesh->GetPolygonVertexNormal(i, j, lCurrentNormal)){
        glNormal3d(lCurrentNormal[0], lCurrentNormal[1], lCurrentNormal[2]);
      } 
      glVertex3dv((GLdouble *)lVertexArray[mesh->GetPolygonVertex(i, j)]);
    }
    glEnd();
  }

  delete [] lVertexArray;
}

void FBXMesh::assignText(Texture const & t){
  mesh->GetLayer(0)->GetTextures(KFbxLayerElement::eDIFFUSE_TEXTURES)->GetDirectArray().Add(const_cast<KFbxFileTexture*>(const_cast<Texture&>(t).getText()));
}

void FBXMesh::buildBoundBox(){
  const int lVertexCount = mesh->GetControlPointsCount();
  KFbxVector4 * lVertexArray = new KFbxVector4[lVertexCount];
  memcpy(lVertexArray, mesh->GetControlPoints(),  lVertexCount*sizeof(KFbxVector4));
  const int pc = mesh->GetPolygonCount();

  double * p;
  trc << 0, 0, 0;
  blc << 0, 0, 0;

  for(int i = 0; i < pc; i++){
    const int vc = mesh->GetPolygonSize(i);
    for(int j = 0; j < vc; j++){
      p = (GLdouble *)lVertexArray[mesh->GetPolygonVertex(i, j)];
      if(trc(0) < p[0]){
        trc(0) = p[0];
      }
      else if(blc(0) > p[0]){
        blc(0) = p[0];
      }
      if(trc(1) < p[1]){
        trc(1) = p[1];
      }
      else if(blc(1) > p[1]){
        blc(1) = p[1];
      }
      if(trc(2) < p[2]){
        trc(2) = p[2];
      }
      else if(blc(2) > p[2]){
        blc(2) = p[2];
      }
    }
  }

  delete [] lVertexArray;
}

void FBXMesh::buildBoundSphere(float r){
  // Assuming no predifined radius by mesh
  if(r == -1){
    const int lVertexCount = mesh->GetControlPointsCount();
    KFbxVector4 * lVertexArray = new KFbxVector4[lVertexCount];
    memcpy(lVertexArray, mesh->GetControlPoints(),  lVertexCount*sizeof(KFbxVector4));
    const int pc = mesh->GetPolygonCount();

    double * p;
    float testR = -1;

    radius = -1;
    for(int i = 0; i < pc; i++){
      const int vc = mesh->GetPolygonSize(i);
      for(int j = 0; j < vc; j++){
        p = (GLdouble *)lVertexArray[mesh->GetPolygonVertex(i, j)];
        testR = sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]);
        if(radius < testR){
          radius = testR;
        }
      }
    }

    delete [] lVertexArray;
  }
  else{
    radius = r;
  }
  cout << "Bounding radius for fbx: " << radius << endl;
}


void FBXMesh::saveDisplayList(){
  index = glGenLists(1);

  const int lVertexCount = mesh->GetControlPointsCount();
  KFbxVector4 * lVertexArray = new KFbxVector4[lVertexCount];
  memcpy(lVertexArray, mesh->GetControlPoints(),  lVertexCount*sizeof(KFbxVector4));

  const int pc = mesh->GetPolygonCount();

  KStringList lUVNames;
  mesh->GetUVSetNames(lUVNames);

  KFbxVector2 lCurrentUV;
  KFbxVector4 lCurrentNormal;

  bool hasUV, hasNorm;

  hasUV = mesh->GetElementNormalCount() > 0;
  hasNorm = mesh->GetElementNormalCount() > 0;

  glNewList(index, GL_COMPILE);
  for(int i = 0; i < pc; i++){
    const int vc = mesh->GetPolygonSize(i);
    if(i != 3){
       //cout << "Someone isn't using triangles..... shame!" << endl;
    }
    glColor4f(0.0f, 0.0f, 0.75f, 1.0f);
    glBegin(GL_POLYGON);
    for(int j = 0; j < vc; j++){
      if(hasUV && mesh->GetPolygonVertexUV(i, j, lUVNames[0].Buffer(), lCurrentUV)){
        glTexCoord2dv(lCurrentUV.mData);
      }
      if(hasNorm && mesh->GetPolygonVertexNormal(i, j, lCurrentNormal)){
        glNormal3d(lCurrentNormal[0], lCurrentNormal[1], lCurrentNormal[2]);
      } 
      glVertex3dv((GLdouble *)lVertexArray[mesh->GetPolygonVertex(i, j)]);
    }
    glEnd();
  }
  glEndList();

  delete [] lVertexArray;
}

void FBXMesh::drawDisplayList() const{
  Assets & assets = Client::getInstance()->getAssets();
  KFbxLayerElementArrayTemplate<int>* lMatInd = NULL;
  if(mesh->GetElementMaterial()){
    mesh->GetMaterialIndices(&lMatInd);
    assets.setTexture(*mesh->GetNode(), lMatInd->GetAt(0)); 
  }
  glCallList(index);
}
