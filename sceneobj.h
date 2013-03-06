/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This abstract class handles static world objects.
 *
 */

#ifndef SCENEOBJ_H_
#define SCENEOBJ_H_

// Standard Libraries

// Imported Libraries
#include "Eigen/Dense"

// Custom Libraries
#include "wob.h"

class SceneOBJ: public WOB{
  public:
    SceneOBJ(string const &);
    ~SceneOBJ();

    void draw() const;
    void saveDisplayList();

  private:
    
};

#endif
