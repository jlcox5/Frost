/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the highlevelcontrol for a mob.
 * 
 */

#ifndef HIGHLEVELCONTROL_H_
#define HIGHLEVELCONTROL_H_

// Standard Libraries

// Custom Libraries
#include "actionplanner.h"

class HighLevelControl{

  public:
    HighLevelControl();
    ~HighLevelControl();
    HighLevelControl& operator=(HighLevelControl&);

  private:
    HighLevelControl(HighLevelControl&);


}

#endif
