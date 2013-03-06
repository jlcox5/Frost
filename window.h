
/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles initializing OpenGL and the display window.
 *
 */

#ifndef WINDOW_H_
#define WINDOW_H_

// SDL Libraries
#include <SDL.h>
#include <SDL_opengl.h>

// Custom Libraries
#include "parseXML.h"
#include "handleChars.h"

class Window{
  public:
    Window();
    ~Window();

    // Display Back Function
    static void runDisplay();
    int getHeight() const{ return height; }
    int getWidth() const{ return width; }

  private:
    // Init OpenGL and SDL 
    void initialize();

    int height, width;

    // Should not be able to create more than one window
    Window(const Window&){}
    Window& operator=(const Window&){ return *this; }
};

#endif
