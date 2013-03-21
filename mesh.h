/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles mesh objects.
 *
 */


#ifndef MESH_H_
#define MESH_H_

// SDL Libraries
#include <SDL.h>
#include <SDL_opengl.h>

// Imported Libraries
#include <fbxsdk.h>
#include "Eigen/Dense"

// Custom Libraries
#include "texture.h"
#include "skeleton.h"

using Eigen::Vector3f; using Eigen::Vector4f;

class Mesh{

  public:
    Mesh();
    Mesh(const Mesh&);
    virtual ~Mesh();
    Mesh& operator=(Mesh const &);

    virtual void drawMesh() const = 0;
    virtual void assignText(Texture const &) = 0;
    virtual void buildBoundBox() = 0;
    virtual void buildBoundSphere(float) = 0;
    virtual void saveDisplayList() = 0;
    virtual void drawDisplayList() const = 0;
    virtual void animateMesh(Skeleton *);
    void setRadius(float f){ radius = f; }
    float const getRadius() const{ return radius; }

  protected:
    Vector3f trc;
    Vector3f blc;
    float radius;
    GLuint index;
    
};

#endif
