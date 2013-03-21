/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles texture objects.
 *
 */

// Standard Libraries
#include <iostream>

// Custom Libraries
#include "texture.h"

using std::cout; using std::endl;

Texture::Texture(): loaded(false){
}

Texture::Texture(KFbxFileTexture const & t): text(&t), id(0), loaded(true){
}

Texture::Texture(KFbxFileTexture const & t, unsigned int i): text(&t), id(i), loaded(true){
}

Texture::Texture(unsigned int i): text(NULL), id(i), loaded(true){
}

Texture::~Texture(){
   cout << "Goodbye Texture!" << endl;
   delete text;
}
