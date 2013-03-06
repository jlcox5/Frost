/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the player object.
 *
 */

#ifndef PLAYER_H_
#define PLAYER_H_

// Standard Libraries

// Imported Libraries
#include "Eigen/Dense"

// Custom Libraries
#include "parseXML.h"
#include "handleChars.h"
#include "mob.h"

using Eigen::Vector3f; using Eigen::Vector4f;

class Player{
  public:
    Player();
    ~Player();

    void loadPlayer(string const &);
    WOB const & getWOB() const{ return *wob; }

    void rotatePlayer(int);
    void translatePlayer(int);
    bool testMove() const;
    bool getCanMove() const{ return canMove; }
    double getCurHealth() const{ return curHealth; }
    double getMaxHealth() const{ return maxHealth; }
    void modHealth(double h);

    Vector3f const & getFacing(){ return facing; }

    void attack();
    void update(float);

  private:
    WOB * wob;
    double curHealth;
    double maxHealth;
    bool canMove;
    bool dead;
    float attackSpeed;
    float attackTimer;

    Vector3f facing;

    Player(Player const &);
    Player& operator=(Player const &);

};

#endif
