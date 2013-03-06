/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles mesh objects.
 *
 */

// Standard Libraries
#include <iostream>
#include <math.h>

// Custom Libraries
#include "mesh.h"
#include "assets.h"
#include "client.h"

using std::cout; using std::endl;


Mesh::Mesh(){

}

Mesh::~Mesh(){
  cout << "Goodbye Mesh!" << endl;
}
