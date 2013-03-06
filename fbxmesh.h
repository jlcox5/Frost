/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles mesh objects.
 *
 */

#ifndef FBXMESH_H_
#define FBXMESH_H_

// SDL Libraries
#include <SDL.h>
#include <SDL_opengl.h>

// FBX Library
#include <fbxsdk.h>

// Custom Libraries
#include "mesh.h"
#include "texture.h"

class FBXMesh: public Mesh{

  public:
    FBXMesh();
    FBXMesh(KFbxMesh&);
    FBXMesh(const FBXMesh&);
    ~FBXMesh();
    FBXMesh& operator=(FBXMesh const &);

    virtual void drawMesh() const;
    virtual void assignText(Texture const &);
    virtual void buildBoundBox();
    virtual void buildBoundSphere(float);

    virtual void saveDisplayList();
    virtual void drawDisplayList() const;

  private:
    KFbxMesh * mesh;
};

#endif
