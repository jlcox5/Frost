/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles rotations, translations, and scaling of model objects.
 *
 */

//SDL Libraries
#include <SDL_opengl.h>

// Custom Libraries
#include "animate.h"

 
Animate::Animate(): velocity(0.0, 0.0, 0.0){

}

Animate::Animate(Vector3d const &v):
    velocity(v){

}

Animate::Animate(Animate& a){
  velocity << a.velocity;
}

void Animate::runAnimation(){
  /*testBounds();
  translate = translate + velocity;
  incRotate();*/
}

Animate::~Animate(){
  cout << "Goodbye Animate!" << endl;
}

