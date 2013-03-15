
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

PolyMesh::PolyMesh(std::vector<Vector3f*> & p, std::vector<std::vector<int> *> & pi, float r):
                   points(p),
                   polyIndex(pi){

  buildBoundSphere(r);
  buildBoundBox();
  cout << "Bounding volumes complete" << endl;
  saveDisplayList();
}

PolyMesh::~PolyMesh(){
  std::vector<std::vector<int> *>::iterator _p;

  for(_p = polyIndex.begin(); _p != polyIndex.end(); _p++){
    delete (*_p);
  }

  glDeleteLists(index, 1);
}

void PolyMesh::drawMesh() const{
  std::vector<std::vector<int> *>::iterator _v;
  Vector3f * p;

  glDisable(GL_TEXTURE_2D);
  for(_v = polyIndex.begin(); _v != polyIndex.end(); _v++){
    glBegin(GL_POLYGON);
      glColor4f(0.3f, 0.2f, 0.0f, 1.0f);
      for(unsigned int i=0; i < (*_v)->size(); i++){
        p = points[(*_v)->at(i)];
        glVertex3f((*p)(0), (*p)(1), (*p)(2));
      }
    glEnd();
  }
  glEnable(GL_TEXTURE_2D);
}

void PolyMesh::assignText(Texture const & t){
  // Need to implement
}

void PolyMesh::buildBoundBox(){
  std::vector<std::vector<int> *>::iterator _v;
  Vector3f * p;
  trc << 0, 0, 0;
  blc << 0, 0, 0;

  for(_v = polyIndex.begin(); _v != polyIndex.end(); _v++){
    for(unsigned int i=0; i < (*_v)->size(); i++){
      p = points[(*_v)->at(i)];
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
    for(_v = polyIndex.begin(); _v != polyIndex.end(); _v++){
      for(unsigned int i=0; i < (*_v)->size(); i++){
        p = points[(*_v)->at(i)];
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
  for(_v = polyIndex.begin(); _v != polyIndex.end(); _v++){
    glBegin(GL_POLYGON);
      glColor4f(0.3f, 0.2f, 0.0f, 1.0f);
      for(unsigned int i=0; i < (*_v)->size(); i++){
        //cout << "   indexing: " << (*_v)->at(i) << endl;
        p = points[(*_v)->at(i)];
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
