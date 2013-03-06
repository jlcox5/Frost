/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class translates the action requested by the action planner to an action list to be
 * completed. 
 * 
 */

// Standard Libraries

// Custom Libraries
#include "actionplanner.h"

#ifndef MIDMOTORSYSTEM_H_
#define MIDMOTORSYSTEM_H_

class MidMotorSystem{

  public:
    MidMotorSystem();
    ~MidMotorSystem();

  private:
    MidMotorSystem(MidMotorSytem&);
    MidMotorSystem& operator=(MidMotorSystem&);

}

#endif
