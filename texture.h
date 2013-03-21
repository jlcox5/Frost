/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles texture objects.
 *
 */

// Imported Libraries
#include <fbxsdk.h>
#include <ImageMagick/Magick++.h>


#ifndef TEXTURE_H_
#define TEXTURE_H_

class Texture{

  public:
    Texture();
    Texture(KFbxFileTexture const &);
    Texture(KFbxFileTexture const &, unsigned int);
    Texture(unsigned int);
    Texture(const Texture&);
    ~Texture();
    Texture& operator=(const Texture&);

    const KFbxFileTexture * getText() const{ return text; }
    const unsigned int getID() const{ return id; }
    const bool isLoaded() const{ return loaded; }

  private:
    const KFbxFileTexture * text;
    unsigned int id;
    bool loaded;

};

#endif
