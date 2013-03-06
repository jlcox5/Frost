/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class mobile objects, such as monsters, NPC's, etc.
 *
 */


#ifndef MOB_H_
#define MOB_H_

// Standard Libraries
#include <vector>

// Imported Libraries
#include "Eigen/Dense"

// Custom Libraries
#include "wob.h"

using Eigen::Vector3f; using Eigen::Vector4f;
using std::cout; using std::endl;

class MOB: public WOB{
  public:
    MOB(){}
    MOB(string const &);
    ~MOB();

    // Need to add -> animations from fbx file
    // Particle effects
    // AI
    void update(float);
    void modHealth(float);
    bool getDead() const{ return dead; }

    Vector3f const & getFacing(){ return facing; }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:
    std::vector<float> degList;
    std::vector<float> distList;

    Vector3f facing;

    int posIt;
    float speed;
    float damage;
    float curHealth;
    float maxHealth;
    float curDeg;
    float curDist;
    int aggressive;
    bool dead;
    bool chasingPlayer;
    bool coolingDown;
    float chaseDistance;
    float maxChase;
    float aggrDist;
    float attackSpeed;
    float attackTimer;

    void turnMOB(float);

};

#endif

