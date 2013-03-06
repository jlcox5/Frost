/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles initializing OpenGL and the display window.
 *
 */

#include <SDL.h>
#include <SDL_opengl.h>

// Standard Libraries
#include <iostream>

// Custom Libraries
#include "window.h"
#include "client.h"

using std::cout; using std::endl;

// Constructor //
Window::Window(){

  ParseXML * parser = ParseXML::getInstance();
  HandleChars * handle = new HandleChars();
  parser->parseXML("xml/window.xml", handle);

  height = handle->getXmlInt("screen_height");
  width = handle->getXmlInt("screen_width");
  initialize();

  delete handle;
}

// Destructor //
Window::~Window(){
  cout << "Goodbye Window!" << endl;
}

// Initialize Display //
void Window::initialize(){

  // Setup SDL
  int bpp = 0;
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0){
    cout << "SDL fail at initialization." << endl;
  }
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

  const SDL_VideoInfo * info = SDL_GetVideoInfo();

  bpp = info->vfmt->BitsPerPixel;

  SDL_SetVideoMode(width, height, bpp, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL);

  glClearColor(0, 0, 0, 0);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Run! //
void Window::runDisplay(){
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Client * client = Client::getInstance();
  client->getRenderer().renderScene();

  SDL_GL_SwapBuffers();
}

