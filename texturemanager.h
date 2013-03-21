/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles texture management.
 *
 */

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

// Standard Libraries
#include <vector>
#include <map>
#include <string>

// FBX Library
#include <fbxsdk.h>

// Custom Libraries
#include "texture.h"

using std::string;

class TextureManager{

  public:
    TextureManager();
    TextureManager(TextureManager&);
    ~TextureManager();
    TextureManager& operator=(TextureManager&);

    
    Texture * getText(int i) const{ return textList[i]; }

    void add(Texture&);
    void loadTextureFBXNode(KFbxNode const &);
    void loadTextureFile(KFbxFileTexture const &, unsigned int *);

    void setTexture(KFbxNode const &, int); 
    void setTexture(unsigned int);
    

  private:
    std::vector<Texture*> textList;
    KFbxSurfaceMaterial const * lastMat;
    unsigned int lastTextObj;
    typedef KMap<const KFbxSurfaceMaterial *, unsigned int> TextObjForMatType;
    TextObjForMatType textObjForMat; 

};

#endif
