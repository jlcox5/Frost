/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles texture management.
 *
 */

//SDL Libraries
#include <SDL_opengl.h>

// Standard Libraries
#include <iostream>
#include <string>

// Custom Libraries
#include "texturemanager.h"
#include "targa.h"
#include "fbxparser.h"
#include "collada.h"

using std::cout; using std::endl;
using std::string;

TextureManager::TextureManager(): textList(), lastMat(NULL), lastTextObj(0), textObjForMat(){
}

TextureManager::~TextureManager(){
  cout << "Goodbye TextureManager!" << endl;
  for(unsigned int i = 0; i < textList.size(); i++){
    delete textList[i];
  }
  delete lastMat;
}

void TextureManager::add(Texture& t){
  cout << "Pushing texture-> " << t.getID() << endl;
  textList.push_back(&t);
}

void TextureManager::loadTextureFBXNode(KFbxNode const &t){
  const int lMatCount = t.GetMaterialCount();
  for(int i=0; i < lMatCount; i++){
    const KFbxSurfaceMaterial * lMat = t.GetMaterial(i);
    if(lMat){
      const KFbxProperty lProp = lMat->FindProperty(KFbxSurfaceMaterial::sDiffuse);
      if(lProp.IsValid()){
        const int lTextCount = lProp.GetSrcObjectCount(KFbxFileTexture::ClassId);
        if(lTextCount){
          const KFbxFileTexture * lText = lProp.GetSrcObject(FBX_TYPE(KFbxFileTexture), 0);
          if(lText){
            unsigned int lTextObj;
            loadTextureFile(*lText, &lTextObj);
            textObjForMat.Insert(lMat, lTextObj);
          }
        }
      }
    }
  }
}

// Currently only supports Targa file format
void TextureManager::loadTextureFile(KFbxFileTexture const &f, unsigned int * n){
   // Need to add check for the same texture being loaded - uses n

   const KString fileName = f.GetFileName();
   unsigned int lTextObj = 0;

   cout << "Loading image: " << f.GetFileName() << endl;

   if(fileName.Right(3).Upper() == "TGA"){
      tga_image img;

      if(tga_read(&img, fileName.Buffer()) == TGA_NOERR){

         // Check image is left to right
         if(tga_is_right_to_left(&img)){
            tga_flip_horiz(&img);
         }
         // Check image is bottom to top
         if(tga_is_top_to_bottom(&img)){
            tga_flip_vert(&img);
         }

         // Mage the image BGR 24 (figure out what the hell that means)
         tga_convert_depth(&img, 24);

         // OpenGL Bind Texture
         glGenTextures(1, &lTextObj);
         glBindTexture(GL_TEXTURE_2D, lTextObj);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
         gluBuild2DMipmaps(GL_TEXTURE_2D, 3, img.width, img.height, GL_BGR,
                          GL_UNSIGNED_BYTE, img.image_data);

         // Clean up
         tga_free_buffers(&img);
         cout << "   Load complete." << endl; 
      }
      else{
        cout << "   Load failed: Texture file might be corrupt." << endl;
        exit(1);
      }
   }

   K_ASSERT(glIsTexture(lTextObj));
   Texture * t = new Texture(f, lTextObj);
   textList.push_back(t);
   if(n){
      *n = lTextObj;
   }
}

void TextureManager::setTexture(KFbxNode const & n, int i){
  const KFbxSurfaceMaterial * lSurfMat = n.GetMaterial(i);
  if(!lSurfMat){ return; }

  if(lSurfMat == lastMat){ return; }

  lastMat = lSurfMat;

  // Should set materials here... but will do later
  const TextObjForMatType::RecordType * lRec = textObjForMat.Find(lSurfMat);
  //const TextObjForMatType::RecordType * lRec = textObjForMat.Maximum();
  if(lRec){
    glBindTexture(GL_TEXTURE_2D, lRec->GetValue());
    lastTextObj = lRec->GetValue();
  }
}

void TextureManager::setTexture(unsigned int i){
  glBindTexture(GL_TEXTURE_2D, i);
  lastTextObj = i;
}
