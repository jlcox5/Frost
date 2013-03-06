/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles all rendering issues.
 *
 */

#ifndef RENDERER_H_
#define RENDERER_H_

// Standard Libraries
#include <vector>

// Custom Libraries
#include "assets.h"
#include "camera.h"
#include "model.h"
#include "texture.h"

class Renderer{

  public:
    Renderer(Assets&);
    ~Renderer();

    void renderScene() const;
    void rotateCamera(int);
    void translateCamera(int);
    void renderCamera() const;
    //void rebuildTree();

    Camera const & getCamera() const{ return camera; }

  private:
    Camera camera;
    Assets &assets;

    // Should only have one renderer
    Renderer(Renderer&);
    Renderer& operator=(Renderer&);
};

#endif
