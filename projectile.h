/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles objects that are thrown from players or mobs.  It is derived from 
 * WOB.
 *
 */

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

// Standard Libraries

// Imported Libraries
#include "Eigen/Dense"

// Custom Libraries
#include "wob.h"

using Eigen::Vector3f; using Eigen::Vector4f;
using std::cout; using std::endl;

class Projectile: public WOB{
  public:
    Projectile();
    Projectile(string const &, Vector3f const &);
    Projectile(Projectile const &);
    ~Projectile();

    void update(float);
    void setDamage(float d) { damage = d; }
    void modDamage(float d) { damage += d; }
    float getDamage() const{ return damage; }

    void setDirection(Vector3f const &);
    void setPosition(Vector3f const &);

    bool getDead() const{ return dead; }
    void setDead(bool b) { dead = b; }

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:
    Vector3f velocity;
    Vector3f acceleration;

    float damage;
    float velocityMag;
    bool dead;
};

#endif

