/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the model class.
 *
 */

//SDL Libraries
#include <SDL_opengl.h>

// Standard Libraries
#include <iostream>

// Custom Libraries
#include "model.h"

using std::cout; using std::endl;

Model::Model(): mesh(), texture(), animate(), isAnimated(false){}

Model::Model(Mesh& m): mesh(&m), texture(), animate(), skel(){
  cout << "Building model..." << endl;
  cout << "   mesh: " << mesh << endl;
}

Model::Model(Mesh& m, Texture& t): mesh(&m), texture(&t), animate(), skel(), isAnimated(false){
  cout << "Building model..." << endl; 
}

Model::Model(Mesh& m, Texture& t, Animate& a): mesh(&m), texture(&t), animate(&a), skel(),
             isAnimated(true){
  cout << "Building model..." << endl; 
}

Model::Model(Mesh& m, Texture& t, Animate& a, Skeleton& s): 
             mesh(&m), texture(&t), animate(&a), skel(&s), isAnimated(true){
  cout << "Building model..." << endl; 
}

// Note that shallow copies are done on purpose as actual data is handles by Texture Mangager,
// Model Factory, etc.
Model& Model::operator=(Model& m){
  cout << "assigning attributes: " << endl;
  m.mesh = mesh;
  m.texture = texture;
  m.animate = new Animate();

  cout << "   m mesh: " << m.mesh << "      this mesh: " << mesh << endl;
  return m;
}

Model::~Model(){
  // Model and texture are deleted as a part of the assets class
  delete animate;
}

void Model::drawModel() const{
  if(texture != NULL){
    if(texture->isLoaded()){
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texture->getID());
    }
  }
  mesh->drawMesh();
  if(texture != NULL){
    if(texture->isLoaded()){
      glDisable(GL_TEXTURE_2D);
    }
  }
}

void Model::animateModel(){
  if(isAnimated){
    // Find keyframe joint manipulations
    animate->updateJoints();

    // Apply keyframe joint manipulations to inverse bind matricies
    skel->updateJointsAnim(animate->getPoses());

    // Apply to each vertex
    mesh->animateMesh(skel);
  }
}
