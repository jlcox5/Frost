/*  Jonathan Cox
 *  Clemson University
 *
 *  This class handles loading the collada file and stripping out the desired data.
 */

#ifndef COLLADA_H_
#define COLLADA_H_

// Standard Libraries
#include <string>
#include <vector>

// Imported Libraries
#include "Eigen/Dense"
#include "Eigen/StdVector"

// Collada Libraries
#include <dae.h>
#include <dom/domCOLLADA.h>
#include <dom/domConstants.h>
#include <ImageMagick/Magick++.h>

// Custom Libraries
#include "polymesh.h"
#include "texture.h"
#include "animate.h"
#include "joint.h"
#include "skeleton.h"

using namespace Eigen;
using std::string;

class Collada{
  public:
    Collada();
    Collada(string const &);
    ~Collada();

    PolyMesh * buildPolyMesh();
    Texture * buildTexture();
    Animate * buildAnimate();
    Skeleton * buildSkeleton();

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:
    DAE collada_dom;
    daeElement * root;
    int PNT;             // 1 = Points, 2 = Points, Normals, 3 = Points, Normals, TextCoords
                         // 4 = Points, TextCoords

    // Model elements that will be transfered to other objects
    std::vector<Vector3f*> * points;
    std::vector<Vector3f*> * normals;
    std::vector<Vector2f*> * texCoords;
    std::vector<std::vector<int> *> * polyIndex;
    std::vector<std::vector<int> *> * normalIndex;
    std::vector<std::vector<int> *> * texIndex;
    unsigned int textID;

    // Joint Info
    Skeleton * skel;
    std::vector<float> * weightList;
    std::vector<int> * weightsPerVertex;
    std::vector<int> * weightListIndex;
    std::vector<string> * jointArrayCollada;

    Matrix4f * bindShapeMatrix;
    std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > * jointInverseBind;

    // Animation Info
    std::vector< std::vector<float> *> * jointKeyFrames;
    std::vector< std::vector<Matrix4f *, aligned_allocator<Matrix4f *> > *> * jointPoses;
    

    void loadElements();
    void loadMesh();
    void loadTexture();
    void loadJoints();
    void loadWeights();
    void loadAnimation();
    void processJoint(Joint *, daeElement *);
    void readFloatData3f(string const, string const);
    void readFloatData2f(string const, string const);
    void readVertexData(string const);
    void readMatrixData4(Joint *, daeElement *);
    bool testNode(daeElement const * const, const char *);

};

#endif
