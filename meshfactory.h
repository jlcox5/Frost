/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the mesh list.
 *
 */

#ifndef MESHFACTORY_H_
#define MESHFACTORY_H_

// Standard Libraries
#include <map>
#include <algorithm>
#include <string>

// Custom Libraries
#include "fbxmesh.h"
#include "polymesh.h"

using std::map; using std::string;

class MeshFactory{
  public:
    MeshFactory() : meshList( map<string, Mesh*>() ) {}
    ~MeshFactory();
    Mesh * getMesh(string const &);
    bool eraseMesh(string const &);
    void pushMesh(string const &, Mesh*);

  private:
    map<string, Mesh*> meshList;

    MeshFactory(MeshFactory const &);
    MeshFactory& operator=(MeshFactory const &);
};

#endif
