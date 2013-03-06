/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles texture objects.
 *
 */

// FBX Library
#include <fbxsdk.h>

#ifndef TEXTURE_H_
#define TEXTURE_H_

class Texture{

  public:
    Texture();
    Texture(KFbxFileTexture const &);
    Texture(KFbxFileTexture const &, unsigned int);
    Texture(const Texture&);
    ~Texture();
    Texture& operator=(const Texture&);

    const KFbxFileTexture * getText() const{ return text; }
    const unsigned int getId() const{ return id; }

  private:
    const KFbxFileTexture * text;
    unsigned int id;

};

#endif
