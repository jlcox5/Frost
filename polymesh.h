/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the polymesh class, simliar in structure to an obj file.
 *
 */

#ifndef POLYMESH_H_
#define POLYMESH_H_

// Standard Libraries
#include <vector>

// Imported Libraries
#include "Eigen/Dense"

// Custom Libraries
#include "mesh.h"
#include "texture.h"
#include "skeleton.h"

using namespace Eigen;

class PolyMesh: public Mesh{

  public:
    PolyMesh(std::vector<Vector3f*> &, std::vector<std::vector<int> *> &, float);
    PolyMesh(std::vector<Vector3f*> &, std::vector<std::vector<int> *> &, 
             std::vector<int> *, std::vector<float> *, float);
    PolyMesh(PolyMesh const &);
    ~PolyMesh();
    PolyMesh& operator=(PolyMesh const &);

    virtual void drawMesh() const;
    virtual void assignText(Texture const &);
    virtual void buildBoundBox();
    virtual void buildBoundSphere(float);

    virtual void saveDisplayList();
    virtual void drawDisplayList() const;

  private:
    std::vector<Vector3f*> & points;
    std::vector<std::vector<int> *> & polyIndex;

    std::vector<int> * jointIndex;
    std::vector<float> * jointWeight;

    PolyMesh();
};

#endif
