/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class applies the torques and forces on the joints required by the mid-motor system. 
 * 
 */

// Standard Libraries

// Custom Libraries
#include "midmotorsystem.h"

#ifndef LOWMOTORSYSTEM_H_
#define LOWMOTORSYSTEM_H_

class LowMotorSystem{

  public:
    LowMotorSystem();
    ~LowMotorSystem();

  private:
    LowMotorSystem(LowMotorSystem&);
    LowMotorSystem& operator=(LowMotorSystem&);

}

#endif
