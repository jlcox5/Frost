/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class represent the client for the BWS MMO Engine.  It is designed to run
 * as a singleton.  More documentation to be provided later.
 *
 */

// SDL Libraries
#include <SDL.h>
#include <SDL_opengl.h>

// Custom Libaries
#include "client.h"

Client * Client::getInstance(){
  if(!inst){ inst = new Client(); }
  return inst;
}

// Constructor //
Client::Client(): display(), 
                  uio(), 
                  assets(), 
                  uiAssets(), 
                  rend(assets), 
                  clock(50000000),
                  sound(),
                  world(),
                  hud(),
                  player(){

  cout << "Client initialized." << endl;

}

void Client::loadGame(){
  sound.loadSound();
  world.loadWorld();
  hud.loadHud("hud/hud.xml");
  player.loadPlayer("player/player.xml");
}

Client::~Client(){
  cout << "Goodbye Client!" << endl;
  delete inst;
}

// Run Client //
void Client::run(){

  bool running = true;
  SDL_Event event;
  getClock().update();
  clock.resetPhysElapsed();
  while(running){
    SDL_PollEvent(&event);
    if(getClock().update()){

      uio.handleKey(const_cast<SDL_Event&>(event));

      if(event.type == SDL_QUIT){ running = false; }
      display.runDisplay();
      world.updateWorld(static_cast<float>(clock.getPhysElapsed()));
      player.update(static_cast<float>(clock.getPhysElapsed()));;
      clock.resetPhysElapsed();
    }
    //cout  << getClock() << endl; 
  }
}

