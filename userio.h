/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles user input and output.
 *
 */

#ifndef USERIO_H_
#define USERIO_H_

// SDL Libraries
#include <SDL.h>
#include <SDL_opengl.h>

// Custom Libraries

class UserIO{
  public:
    UserIO();
    ~UserIO();
    void handleKey(SDL_Event const &);

  private:
    bool keyCaught;

    // Should only have one instance to handle user input
    UserIO(UserIO const &){}
    UserIO& operator=(UserIO const &){ return *this; }
    
};

#endif
