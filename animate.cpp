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
#include "client.h"

 
Animate::Animate(): velocity(0.0, 0.0, 0.0),
                    jointKeyFrames(),
                    jointPoses(){
  minKeyFrame = 0;
  maxKeyFrame = 0;
  curKeyFrame = 0;
}

Animate::Animate(std::vector< std::vector<float> *> * jkf,
                 std::vector< std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > *> * jp):
                 velocity(0.0, 0.0, 0.0),
                 jointKeyFrames(jkf),
                 jointPoses(jp){

  minKeyFrame = jointKeyFrames->at(0)->at(0);
  maxKeyFrame = jointKeyFrames->at(0)->at(jointKeyFrames->at(0)->size()-1);
  curKeyFrame = 0;

  cout << "MIN KEYFRAME::: " << minKeyFrame << endl;
  cout << "MAX KEYFRAME::: " << maxKeyFrame << endl;

  // Push a matrix for each matrix set in jointPoses
  for(unsigned int i = 0; i < jointPoses->size(); i++){
    Matrix4f * m = new Matrix4f();
    currentPoses.push_back(m);
  }
}


Animate::Animate(Vector3d const &v):
    velocity(v),
    jointKeyFrames(),
    jointPoses(){
  minKeyFrame = 0;
  maxKeyFrame = 0;
  curKeyFrame = 0;
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

// This function makes the assumption that the keyframes
// are the same for each joint in the list.  If a situation
// arrises where this is not the case, this will have to be
// refactored.
void Animate::updateJoints(){
  float t = Client::getInstance()->getClock().getPhysElapsed();
  t += 0.05;
  float w = 0;
  int timeInd1, timeInd2;
  bool wSet = false;

  timeInd1 = timeInd2 = 0;
  cout << "CurKeyFrame: " << curKeyFrame << endl;
  cout << "MaxKeyFrame: " << maxKeyFrame << endl;
  curKeyFrame = fmod(curKeyFrame + t, maxKeyFrame);

  // Find weight between two matricies that should be applied
  // Check lower bound
  if(curKeyFrame < minKeyFrame){
    w = curKeyFrame/minKeyFrame;
    wSet = true;
  }

  // Find inbetween value 
  if(wSet == false){
    for(unsigned int i = 0; i < jointKeyFrames->at(0)->size()-1; i++){
      if((curKeyFrame > jointKeyFrames->at(0)->at(i)) && 
         (curKeyFrame < jointKeyFrames->at(0)->at(i+1)) ){

        float dt = jointKeyFrames->at(0)->at(i+1) -
                   jointKeyFrames->at(0)->at(i);
        float dw = curKeyFrame - jointKeyFrames->at(0)->at(i);

        w = dw/dt;
        timeInd1 = i;
        timeInd2 = i+1;

        wSet = true;
        break;
      }
    } 
  }

  // Find interpolated matrix and push to current poses
  for(unsigned int i = 0; i < jointPoses->size(); i++){
    Matrix4f * m1, * m2;

    m1 = jointPoses->at(i)->at(timeInd1);
    m2 = jointPoses->at(i)->at(timeInd2);

    *(currentPoses[i]) = ((1-w) * *(m1)) + (w * *(m2));
  }

}

