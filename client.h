/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class represent the client for the BWS MMO Engine.  It is designed to run
 * as a singleton.  More documentation to be provided later.
 *
 */

#ifndef CLIENT_H_
#define CLIENT_H_

// SDL Libraries
#include <SDL.h>
#include <SDL_opengl.h>

// Custom Libraries
#include "window.h"
#include "userio.h"
#include "assets.h"
#include "model.h"
#include "renderer.h"
#include "clock.h"
#include "sound.h"
#include "world.h"
#include "hud.h"
#include "player.h"

class Client{

  public:
    static Client * getInstance();
    ~Client(); 

    const Window * getWindow() const{ return &display; }
    Assets&  getAssets() { return assets; }
    Assets&  getUIAssets() { return uiAssets; }
    Renderer& getRenderer() { return rend; }
    Clock& getClock() { return clock; }
    SDLSound& getSound() { return sound; }
    World& getWorld() { return world; }
    Hud & getHud() { return hud; }
    Player& getPlayer(){ return player; }

    void loadGame();
    void run();
  private:
    Client();

    // Overloaded copy and assignment
    Client(const Client&);
    Client& operator=(const Client&);

    // Instance
    static Client * inst;

    // Windowing System
    Window display;

    // User IO
    UserIO uio;

    // Assets
    Assets assets;
    Assets uiAssets;

    // Renderer
    Renderer rend;

    // Internal game clock
    Clock clock;

    // Sound system
    SDLSound sound;

    // World
    World world;

    // Hud
    Hud hud;

    // Player object
    Player player;
};

#endif

   
