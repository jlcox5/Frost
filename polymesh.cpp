
/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the polymesh class, similiar in structure to an obj file.
 *
 */

// Standard Libraries
#include <iostream>

// Custom Libraries
#include "polymesh.h"

using std::cout; using std::endl;

PolyMesh::PolyMesh(std::vector<Vector3f*> * p, std::vector<std::vector<int> *> * pi, float r):
                   points(),
                   pointsBase(p),
                   polyIndex(pi),
                   normals(0),
                   normalIndex(0),
                   texCoords(0),
                   texIndex(0),
                   jointIndex(0),
                   weightIndex(0),
                   jointWeight(0),
                   jointMap(0),
                   bindShapeMatrix(0),
                   jointInverseBind(0){
  points = new std::vector<Vector3f*>();
  pointsBaseToPoints();
  buildBoundSphere(r);
  buildBoundBox();
  cout << "Bounding volumes complete" << endl;
  saveDisplayList();
}

PolyMesh::PolyMesh(std::vector<Vector3f*> * p, std::vector<std::vector<int> *> * pi, 
                   std::vector<Vector3f*> * n, std::vector<std::vector<int> *> * ni, float r):
                   points(),
                   pointsBase(p),
                   polyIndex(pi),
                   normals(n),
                   normalIndex(ni),
                   texCoords(),
                   texIndex(),
                   jointIndex(0),
                   weightIndex(0),
                   jointWeight(0),
                   jointMap(0),
                   bindShapeMatrix(0),
                   jointInverseBind(0){
  points = new std::vector<Vector3f*>();
  pointsBaseToPoints();
  buildBoundSphere(r);
  buildBoundBox();
  cout << "Bounding volumes complete" << endl;
  saveDisplayList();
}

PolyMesh::PolyMesh(std::vector<Vector3f*> * p, std::vector<std::vector<int> *> * pi, 
         std::vector<Vector3f*> * n, std::vector<std::vector<int> *> * ni, 
         std::vector<Vector2f*> * t, std::vector<std::vector<int> *> * ti, float r):
         points(),
         pointsBase(p),
         polyIndex(pi),
         normals(n),
         normalIndex(ni),
         texCoords(t),
         texIndex(ti),
         jointIndex(0),
         weightIndex(0),
         jointWeight(0),
         jointMap(0),
         bindShapeMatrix(0),
         jointInverseBind(0){
  points = new std::vector<Vector3f*>();
  pointsBaseToPoints();
  buildBoundSphere(r);
  buildBoundBox();
  cout << "Bounding volumes complete" << endl;
  saveDisplayList();
}

PolyMesh::PolyMesh(std::vector<Vector3f*> * p, std::vector<std::vector<int> *> * pi, 
           std::vector<Vector3f*> * n, std::vector<std::vector<int> *> * ni, 
           std::vector<Vector2f*> * t, std::vector<std::vector<int> *> * ti,
           std::vector<std::vector<int> *> * ji,
           std::vector<std::vector<int> *> * wi,
           std::vector<float> * jw,
           std::vector<int> * jm, float r):
           points(),
           pointsBase(p),
           polyIndex(pi),
           normals(n),
           normalIndex(ni),
           texCoords(t),
           texIndex(ti),
           jointIndex(ji),
           weightIndex(wi),
           jointWeight(jw),
           jointMap(jm),
           bindShapeMatrix(0),
           jointInverseBind(0){
  points = new std::vector<Vector3f*>();
  pointsBaseToPoints();
  buildBoundSphere(r);
  buildBoundBox();
  cout << "Bounding volumes complete" << endl;
  saveDisplayList();
}

PolyMesh::PolyMesh(std::vector<Vector3f*> * p, std::vector<std::vector<int> *> * pi, 
           std::vector<Vector3f*> * n, std::vector<std::vector<int> *> * ni, 
           std::vector<Vector2f*> * t, std::vector<std::vector<int> *> * ti,
           std::vector<std::vector<int> *> * ji,
           std::vector<std::vector<int> *> * wi,
           std::vector<float> * jw,
           std::vector<int> * jm, Matrix4f * bsm,
           std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > * jib, float r):
           points(),
           pointsBase(p),
           polyIndex(pi),
           normals(n),
           normalIndex(ni),
           texCoords(t),
           texIndex(ti),
           jointIndex(ji),
           weightIndex(wi),
           jointWeight(jw),
           jointMap(jm),
           bindShapeMatrix(bsm),
           jointInverseBind(jib){
  points = new std::vector<Vector3f*>();
  pointsBaseToPoints();
  buildBoundSphere(r);
  buildBoundBox();
  cout << "Bounding volumes complete" << endl;
  saveDisplayList();
}
           

/*PolyMesh::PolyMesh(std::vector<Vector3f*> & p, std::vector<std::vector<int> *> & pi, 
                   std::vector<std::vector<int> *> * ji, std::vector<float> * jw, float r):
                   points(p),
                   polyIndex(pi),
                   normals(),
                   normalIndex(),
                   texCoords(),
                   texIndex(),
                   jointIndex(ji),
                   jointWeight(jw){

  buildBoundSphere(r);
  buildBoundBox();
  cout << "Bounding volumes complete" << endl;
  saveDisplayList();
}*/

PolyMesh::~PolyMesh(){
  std::vector<std::vector<int> *>::iterator _p;

  for(_p = polyIndex->begin(); _p != polyIndex->end(); _p++){
    delete (*_p);
  }

  glDeleteLists(index, 1);
}

void PolyMesh::pointsBaseToPoints(){
  std::vector<Vector3f*>::iterator _p;
  Vector3f * v;

  for(_p = pointsBase->begin(); _p != pointsBase->end(); _p++){
    v = new Vector3f();
    (*v) << *(*_p);
    points->push_back(v);
  } 
}

void PolyMesh::drawMesh() const{
  std::vector<std::vector<int> *>::iterator _v;
  std::vector<std::vector<int> *>::iterator _vn;
  std::vector<std::vector<int> *>::iterator _vt;
  Vector3f * p;
  Vector3f * n;
  Vector2f * t;

  bool useNormals = false;
  bool useText = false;
  if(normals){
    if(normalIndex->size() != 0 && normals->size() != 0){
      useNormals = true;
    }
  }
  if(texCoords){
    if(texIndex->size() != 0 && texCoords->size() != 0){
      useText = true;
    }
  }

  if(useNormals){ _vn = normalIndex->begin(); }
  if(useText){
    _vt = texIndex->begin();
  }
  for(_v = polyIndex->begin(); _v != polyIndex->end(); _v++){
    glBegin(GL_POLYGON);
      glColor4f(0.3f, 0.2f, 0.0f, 1.0f);
      for(unsigned int i=0; i < (*_v)->size(); i++){
        if(useText){
          t = texCoords->at((*_vt)->at(i));
          glTexCoord2d((*t)(0), (*t)(1));
        }
        if(useNormals){
          n = normals->at((*_vn)->at(i));
          glNormal3f((*n)(0), (*n)(1), (*n)(2));
        }
        p = points->at((*_v)->at(i));
        glVertex3f((*p)(0), (*p)(1), (*p)(2));
      }
    glEnd();
    if(useNormals){ _vn++; }
    if(useText){ _vt++; }
  }
}

void PolyMesh::buildBoundBox(){
  std::vector<std::vector<int> *>::iterator _v;
  Vector3f * p;
  trc << 0, 0, 0;
  blc << 0, 0, 0;

  for(_v = polyIndex->begin(); _v != polyIndex->end(); _v++){
    for(unsigned int i=0; i < (*_v)->size(); i++){
      p = points->at((*_v)->at(i));
      if(trc(0) < (*p)(0)){
        trc(0) = (*p)(0);
      }
      else if(blc(0) > (*p)(0)){
        blc(0) = (*p)(0);
      }
      if(trc(1) < (*p)(1)){
        trc(1) = (*p)(1);
      }
      else if(blc(1) > (*p)(1)){
        blc(1) = (*p)(1);
      }
      if(trc(2) < (*p)(2)){
        trc(2) = (*p)(2);
      }
      else if(blc(2) > (*p)(2)){
        blc(2) = (*p)(2);
      }
    }
  }

}

void PolyMesh::buildBoundSphere(float r){

  // Assuming no predifined radius by mesh
  if(r == -1){
    std::vector<std::vector<int> *>::iterator _v;
    Vector3f * p;
    float testR = -1;

    radius = -1;
    for(_v = polyIndex->begin(); _v != polyIndex->end(); _v++){
      for(unsigned int i=0; i < (*_v)->size(); i++){
        p = points->at((*_v)->at(i));
        testR = sqrt((*p)(0)*(*p)(0) + (*p)(1)*(*p)(1) + (*p)(2)*(*p)(2));
        if(radius < testR){
          radius = testR;
        }
      }
    }
  }
  else{
    radius = r;
  }
  cout << "Bounding radius for poly: " << radius << endl;
}

void PolyMesh::saveDisplayList(){
  index = glGenLists(1);
  std::vector<std::vector<int> *>::iterator _v;
  Vector3f * p;

  glNewList(index, GL_COMPILE);
  for(_v = polyIndex->begin(); _v != polyIndex->end(); _v++){
    glBegin(GL_POLYGON);
      glColor4f(0.3f, 0.2f, 0.0f, 1.0f);
      for(unsigned int i=0; i < (*_v)->size(); i++){
        //cout << "   indexing: " << (*_v)->at(i) << endl;
        p = points->at((*_v)->at(i));
        glVertex3f((*p)(0), (*p)(1), (*p)(2));
      }
    glEnd();
  }
  glEndList();
}

void PolyMesh::drawDisplayList() const{
  glDisable(GL_TEXTURE_2D);
  glCallList(index);
  glEnable(GL_TEXTURE_2D);
}

void PolyMesh::animateMesh(Skeleton * s){
  RowVector4f t;
  Matrix4f m;
  Matrix4f m_tot;
  float w;

  for(unsigned int i = 0; i < pointsBase->size(); i++){
    t << (*pointsBase->at(i))(0), (*pointsBase->at(i))(1), (*pointsBase->at(i))(2), 1;

    // Multiply by joints and weights...
    m_tot << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;

    // Find total weight first
    w = jointWeight->at(weightIndex->at(i)->at(0));
    m = s->getJointAt(jointMap->at(jointIndex->at(i)->at(0)))->getCombined();
    m_tot = m + m_tot;
    m = *(bindShapeMatrix) *
        s->getJointAt(jointMap->at(jointIndex->at(i)->at(0)))->getInverseBind() *
        s->getJointAt(jointMap->at(jointIndex->at(i)->at(0)))->getCurTrans();
    t = t * m;
    *(points->at(i)) << t(0), t(1), t(2);
  }
}
