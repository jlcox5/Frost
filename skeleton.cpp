/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the skeleton class.  A skeleton is a set of defined joints.
 *
 */

// Custom Libraries
#include "skeleton.h"

Skeleton::Skeleton(): rootJointList(), fullJointList(), jointCount(0){}

Skeleton::~Skeleton(){
  std::vector<Joint*>::iterator _j;

  for(_j = rootJointList.begin(); _j != rootJointList.end(); _j++){
    delete *(_j);
  }

  rootJointList.clear();
}

void Skeleton::pushRoot(Joint * j){
  rootJointList.push_back(j);
  fullJointList.push_back(j);
  jointCount++;
}

void Skeleton::addJoint(Joint * parent, Joint * child){
  std::vector<Joint*>::iterator _j;

  for(_j = fullJointList.begin(); _j != fullJointList.end(); _j++){
    if((*_j) == parent){
      fullJointList.push_back(child);
      jointCount++;
      break;
    }
  }
}

void Skeleton::updateJointsAnim( std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > & jl){
  std::vector<Joint *>::iterator _fjl;
  std::vector<Joint *>::iterator _rjl;
  std::vector<Matrix4f *, aligned_allocator<Matrix4f *> >::iterator _jl;

  // Size check
  if(jl.size() != fullJointList.size()){
    cout << "Error-> joint list size does not match update " <<
             "list size. Exited from skeleton::updateJoints." << endl;
    cout << "    jl size: " << jl.size() << "   fjl size: " << fullJointList.size() << endl;
    exit(1);
  }

  _jl = jl.begin();
  for(_fjl = fullJointList.begin(); _fjl != fullJointList.end(); _fjl++){
    (*_fjl)->updateJoint((*_jl));
    _jl++;
  }

  // Update each joint based on the parent joint
  for(_rjl = rootJointList.begin(); _rjl != rootJointList.end(); _rjl++){
    updateJointMatrix((*_rjl));
  }
}

void Skeleton::updateJointMatrix(Joint * j){
  j->updateChildren();
}

void Skeleton::mapJointInverseBind(std::vector<string> * s,
               std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > * ib){
  std::vector<string>::iterator _s;
  std::vector<Joint*>::iterator _fjl;
  int set = 0;

  cout << "Id string from Collada: " << endl;
  for(_s = s->begin(); _s != s->end(); _s++){
    cout << (*_s) << " ";
  }
  cout << endl;

  //Make sure number of inverse bind matricies and elements in string vector match
  if(ib->size() != s->size()){
    cout << "Error: skeleton(mapJointInverseBind)-> " << endl;
    cout << "   Number of id's does match number of inverse bind matricies." << endl;
    exit(1);
  }

  for(_fjl = fullJointList.begin(); _fjl != fullJointList.end(); _fjl++){
    for(unsigned int i = 0; i < s->size(); i++){
      if((*_fjl)->getId() == s->at(i)){
        cout << "Setting inverse for : " << (*_fjl)->getId() << endl;
        cout << *(ib->at(i)) << endl;
        (*_fjl)->setInverseBind(*(ib->at(i)));
        set++;
      }
    }
  }

  if(jointCount != set){
    cout << "Warning: skeleton(mapJointInverseBind)-> " << endl;
    cout << "   Joint Count: " << jointCount << " Inverse Matricies Set: " << set << endl;
  }
}
