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

Model::Model(): mesh(), texture(), animate(){

}

Model::Model(Mesh& m): mesh(&m), texture(), animate(new Animate()){
  cout << "Building model..." << endl;
  cout << "   mesh: " << mesh << endl;
}

Model::Model(Mesh& m, Texture& t): mesh(&m), texture(&t), animate(){
  cout << "Building model..." << endl; 
  mesh->assignText(t);
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
  //animate->runAnimation();
  mesh->drawMesh();
}
