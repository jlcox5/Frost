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

Texture::Texture(){
}

Texture::Texture(KFbxFileTexture const & t): text(&t), id(0){
}

Texture::Texture(KFbxFileTexture const & t, unsigned int i): text(&t), id(i){
}

Texture::~Texture(){
   cout << "Goodbye Texture!" << endl;
   delete text;
}
