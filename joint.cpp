/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the joint class.  A joint is defined as a translation, a rotation,
 * and a scale.
 *
 */


#include "joint.h"

using std::cout; using std::endl;

Joint::Joint(): pos(), rot(), scale(1.0), parent(), children(){

}

Joint::Joint(std::string & s): pos(), rot(), scale(1.0), parent(NULL), children(),
                               id(s), transform(), currentTransform(){
}

Joint::~Joint(){
  std::vector<Joint*>::iterator _j;

  for(_j = children.begin(); _j != children.end(); _j++){
    delete( (*_j) );
  }

}

void Joint::setMatrix(Matrix4f & m){
  transform << m;
  currentTransform << transform;

  //cout << "Transform set: " << endl;
  //cout << transform << endl;
}

void Joint::addChild(Joint * j){
  children.push_back(j);
}

void Joint::setParent(Joint * j){
  parent = j;
}

void Joint::setInverseBind(Matrix4f & m){
  inverseBind << m;
}

void Joint::updateJoint(Matrix4f * m){
  currentTransform << *(m);
}

void Joint::updateChildren(){
  std::vector<Joint*>::iterator _c;

  if(parent != NULL){
    //cout << "   " << id << endl;
    //cout << "      Children: " << children.size() << endl;
    //cout << "      Parent id: " << parent->getId() << endl;
    //cout << "  Parent trans: " << endl;
    //cout << parent->getCurTrans();
    //cout << endl;
    //cout << "  Finished!" << endl;
    //currentTransform = parent->getCurTrans() * currentTransform;
    currentTransform = currentTransform * parent->getCurTrans();
    //currentTransform = inverseBind *currentTransform * parent->getCurTrans();
    //currentTransform = currentTransform * inverseBind;
  }
  for(_c = children.begin(); _c != children.end(); _c++){
    (*_c)->updateChildren();
  }
  //if( parent != NULL){
  //  combined = currentTransform * inverseBind;
  //}
  //else{
    combined = currentTransform;
  //}
}
