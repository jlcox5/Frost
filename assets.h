/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles user input and output.
 *
 */

#ifndef ASSETS_H_
#define ASSETS_H_

// Standard Libraries
#include <vector>
#include <string>

// FBX Library
#include <fbxsdk.h>

// Custom Libraries
#include "mesh.h"
#include "fbxmesh.h"
#include "texturemanager.h"
#include "parseXML.h"
#include "handleChars.h"
#include "meshfactory.h"
#include "modelfactory.h"
#include "model.h"
#include "particlefactory.h"

using std::string;

class Assets{

  public:
    Assets();
    ~Assets();

    // Load assets specified in an FBX file
    void loadAssets(string const &);
    void addTexture(Texture&);

    void loadTextureFBXNode(KFbxNode const &);

    Texture * getText(int i) const{ return textMan.getText(i); }
    MeshFactory& getMeshFactory(){ return meshFact; }
    ModelFactory& getModelFactory(){ return modelFact; }
    ParticleFactory& getParticleFactory(){ return particleFact; }
    

    void setTexture(KFbxNode const &, int);
    void rebuildTree();
    void rebuildTreeModel();

  private:
    MeshFactory meshFact;
    ModelFactory modelFact;
    TextureManager textMan;

    ParticleFactory particleFact;

    // Should not be able to copy assets as of now 
    Assets(const Assets& a): meshFact(), textMan(){}
    Assets& operator=(const Assets&){ return *this; }

};

#endif
