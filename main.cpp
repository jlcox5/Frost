/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * Almighty main. Initialize the client, which gets everything rolling. 
 *
 */

//SDL Libraries
#include <SDL_opengl.h>

// Standard Libraries
#include <iostream>

// FBS Library
#include <fbxsdk.h>

// Custom Libraries
#include "client.h"
#include "fbxparser.h"
#include "parseXML.h"
#include "world.h"

// Initialization of all static class variables
Client * Client::inst = NULL;
FBXParser * FBXParser::inst = NULL;
KFbxSdkManager * FBXParser::lManager = NULL;
ParseXML * ParseXML::inst = NULL;

unsigned int counter = time(NULL);

using std::cout; using std::endl;

int main(int argc, char ** argv){

  // Build client, load resources
  cout << "Client..." << endl;
  Client * client = Client::getInstance();

  cout << "Loading game.... " << endl;
  client->loadGame();

  cout << "Running client.... " << endl;
  // Run client
  client->run();

  return 0;
}
