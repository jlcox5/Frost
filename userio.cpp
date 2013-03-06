/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles user input and output.
 *
 */

// Standard Libraries
#include <iostream>

// Custom Libraries
#include "userio.h"
#include "client.h"
#include "fbxparser.h"

using std::cout; using std::endl;

// Constructor //
UserIO::UserIO() : keyCaught(false){
}

// Destructor //
UserIO::~UserIO(){
  cout << "Goodbye UserIO!" << endl;
}

void UserIO::handleKey(SDL_Event const &event){
  Uint8 * key;
  key = SDL_GetKeyState(NULL);
  Client * client = Client::getInstance();

  if(key[SDLK_w]){
    client->getPlayer().translatePlayer(0);
    client->getRenderer().translateCamera(0);
  }
  if(key[SDLK_s]){
    client->getPlayer().translatePlayer(1);
    client->getRenderer().translateCamera(1);
  }
  if(key[SDLK_q]){
    client->getPlayer().translatePlayer(2);
    client->getRenderer().translateCamera(2);
  }
  if(key[SDLK_e]){
    client->getPlayer().translatePlayer(3);
    client->getRenderer().translateCamera(3);
  }
  if(key[SDLK_d]){
    client->getRenderer().rotateCamera(1);
    client->getPlayer().rotatePlayer(-1);
  }
  if(key[SDLK_a]){
    client->getRenderer().rotateCamera(-1);
    client->getPlayer().rotatePlayer(1);
  }
  if(key[SDLK_ESCAPE]){
    FBXParser * f = FBXParser::getInstance();
    delete f;

    exit(0);
  }

  if(event.type == SDL_KEYDOWN || event.type == SDL_KEYUP || event.type == SDL_PRESSED){
    if(event.type == SDL_KEYDOWN && keyCaught == false){

      if(key[SDLK_F1]){
        client->getHud().toggleDisplay();
        keyCaught = true;
      }
      if(key[SDLK_SPACE]){
        client->getPlayer().attack();
        keyCaught = true;
      }
      if(key[SDLK_p]){
        keyCaught = true;
      }
      else if(key[SDLK_t]){
        keyCaught = true;
        client->getAssets().rebuildTree();
      }
    } 
    if(event.type == SDL_KEYUP){
      keyCaught = false;
    }
  }
}
