/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class uses generates an lsystem based on a grammar provided in an xml format.
 *
 */

#ifndef LSYSTEM_H_
#define LSYSTEM_H_

// Standard Libraries
#include <string>
#include <map>
#include <stack>
#include <vector>

// Imported Libraries
#include "Eigen/Dense"
#include "Eigen/Geometry"

// Custom Libraries
#include "parseXML.h"
#include "handleChars.h"
#include "polymesh.h"

using namespace Eigen;

class LSystem{

  public:
    LSystem(const string &);
    ~LSystem();

    void drawSkeleton() const;
    void printGrammar() const;
    PolyMesh * buildMesh() const;

  private:
    // Parser
    HandleChars * handle;

    // Building LSystem
    // These values are all defined in or are determined from the associated
    // xml file
    Vector3f state;
    Vector3f start;
    Vector3f facing;
    float angle;
    float stepSize;
    int levels;
    int maxLevel;
    char startSymbol;
    float boundRad;
    std::multimap<char, string> grammar;
    string sentence;
    
    // Results
    std::vector<Vector3f> coordList;
    std::vector<int> levelList;

    // Helper functions
    void loadSystem(string const &);
    void buildSentence();
    void applyGrammarOp(float, float, float);
    void constructSystem();
    void getGrammar(); 

    // Not sure if you should be able to copy an LSystem yet
    LSystem();
    LSystem(LSystem&);
    LSystem& operator=(LSystem&);
};

#endif
