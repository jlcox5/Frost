/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the polymesh class, simliar in structure to an obj file.
 *
 */

#ifndef POLYMESH_H_
#define POLYMESH_H_

// Standard Libraries
#include <string>
#include <vector>

// Imported Libraries
#include "Eigen/Dense"
#include "Eigen/StdVector"

// Custom Libraries
#include "mesh.h"
#include "texture.h"
#include "skeleton.h"

using namespace Eigen;

class PolyMesh: public Mesh{

  public:
    PolyMesh(std::vector<Vector3f*> *, std::vector<std::vector<int> *> *, float);

    PolyMesh(std::vector<Vector3f*> *, std::vector<std::vector<int> *> *, 
             std::vector<Vector3f*> *, std::vector<std::vector<int> *> *, float);

    PolyMesh(std::vector<Vector3f*> *, std::vector<std::vector<int> *> *, 
             std::vector<Vector3f*> *, std::vector<std::vector<int> *> *, 
             std::vector<Vector2f*> *, std::vector<std::vector<int> *> *, float);

    // With Joint Information
    PolyMesh(std::vector<Vector3f*> *, std::vector<std::vector<int> *> *, 
             std::vector<Vector3f*> *, std::vector<std::vector<int> *> *, 
             std::vector<Vector2f*> *, std::vector<std::vector<int> *> *,
             std::vector<std::vector<int> *> *,
             std::vector<std::vector<int> *> *,
             std::vector<float> *,
             std::vector<int> *, float);

    // With bind matricies
    PolyMesh(std::vector<Vector3f*> *, std::vector<std::vector<int> *> *, 
             std::vector<Vector3f*> *, std::vector<std::vector<int> *> *, 
             std::vector<Vector2f*> *, std::vector<std::vector<int> *> *,
             std::vector<std::vector<int> *> *,
             std::vector<std::vector<int> *> *,
             std::vector<float> *,
             std::vector<int> *,
             Matrix4f *,
             std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > *,
             float);

    //PolyMesh(std::vector<Vector3f*> &, std::vector<std::vector<int> *> &, 
    //         std::vector<std::vector<int> *> *, std::vector<float> *, float);
    PolyMesh(PolyMesh const &);
    ~PolyMesh();
    PolyMesh& operator=(PolyMesh const &);

    virtual void drawMesh() const;
    virtual void assignText(Texture const &){};
    virtual void buildBoundBox();
    virtual void buildBoundSphere(float);

    virtual void saveDisplayList();
    virtual void drawDisplayList() const;
    virtual void animateMesh(Skeleton *);

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:
    std::vector<Vector3f*> * points;
    std::vector<Vector3f*> * pointsBase;
    std::vector<std::vector<int> *> * polyIndex;

    std::vector<Vector3f*> * normals;
    std::vector<std::vector<int> *> * normalIndex;

    std::vector<Vector2f*> * texCoords;
    std::vector<std::vector<int> *> * texIndex;

    std::vector<std::vector<int> *> * jointIndex;
    std::vector<std::vector<int> *> * weightIndex;
    std::vector<float> * jointWeight;
    std::vector<int> * jointMap;

    Matrix4f * bindShapeMatrix;
    std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > * jointInverseBind;

    void pointsBaseToPoints();
    PolyMesh();
};

#endif
