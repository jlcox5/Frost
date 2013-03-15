/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the model class.
 * 
 */

#ifndef MODEL_H_
#define MODEL_H_

// Custom Libraries
#include "mesh.h"
#include "texture.h"
#include "animate.h"

class Model{

  public:
    Model();
    Model(Mesh&);
    Model(Mesh&, Texture&);
    Model& operator=(Model&);
    ~Model();

    void drawModel() const;
    Animate * getAnimate(){ return animate; }
    void setMesh(Mesh & m){ mesh = &m; }
    Mesh & getMesh() const{ return *mesh; } 

  private:
    Mesh * mesh;
    Texture * texture;
    Animate * animate;

    Model(Model&);
};

#endif
