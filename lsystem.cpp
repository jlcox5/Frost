/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class uses generates an lsystem based on a grammar provided in an xml format.
 *
 */

// Standard Libraries
#include <algorithm>

//SDL Libraries
#include <SDL_opengl.h>

// Custom Libraries
#include "lsystem.h"

extern unsigned int counter;

bool isUpper (char ch) { return (ch >= 'A' && ch <= 'Z'); }

bool isLetter (char ch) { 
  return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}

bool sortPoly(std::vector<int> * i, std::vector<int> * j){
  return i->at(0) < j->at(0);
}

LSystem::LSystem(string const & fn): handle(new HandleChars()),
                                     state(0.0, 0.0, 0.0),
                                     start(0.0, 0.0, 0.0),
                                     facing(0.0, 0.0, 0.0),
                                     angle(0.0),
                                     stepSize(0.0),
                                     levels(1),
                                     maxLevel(-1),
                                     startSymbol('F'),
                                     grammar(),
                                     sentence(),
                                     coordList(){

  loadSystem(fn);
  buildSentence();
  constructSystem();
}

LSystem::~LSystem(){
  delete handle;
}

// Parse through the xml file and load the starting values and the grammar 
// for the L-System.
void LSystem::loadSystem(string const & fn){
  // ParseXML is a singleton
  ParseXML * parser = ParseXML::getInstance();
  parser->parseXML(fn, handle);
  string data;

  start(0) = handle->getXmlFloat("startX");
  start(1) = handle->getXmlFloat("startY");
  start(2) = handle->getXmlFloat("startZ");
  facing(0) = handle->getXmlFloat("facingX");
  facing(1) = handle->getXmlFloat("facingY");
  facing(2) = handle->getXmlFloat("facingZ");
  angle = handle->getXmlFloat("angle");
  stepSize = handle->getXmlFloat("stepSize");
  levels = handle->getXmlInt("levels");
  boundRad = handle->getXmlFloat("boundRad");

  startSymbol = '\0';
  startSymbol = handle->getXmlStr("startSymbol")[0];
  if(startSymbol == '\0'){
    throw string("Unable to locate start symbol for lsystem.");
  }

  getGrammar();
}


// Build the sentence based on the grammar and the starting symbol defined in the xml file
void LSystem::buildSentence(){
  int count = 0;

  sentence = startSymbol;

  while(count < levels){
    string new_sentence;
    for(unsigned int i =0; i < sentence.size(); ++i){
      char ch = sentence[i];
      if( isUpper(ch) ){
        new_sentence += grammar.find(ch)->second;
      }
      else{
        new_sentence += ch;
      }
    }
    sentence = new_sentence;
    ++count;
  }
}

// Apply the random angle represented by rAng as a rotation around the axis defined by
// the current facing vector in the L-System, then use that rotated position as the new
// facing vector.  The rotation allows for the branching, and the rAng give the branching
// a more natural look... usually.
void LSystem::applyGrammarOp(float normDir, float rAng, float levelRatio){
  // These are used to define a coordinate system for the quaternion rotation
  Vector3f ax1, ax2, ax3;
  Vector3f a;
  Vector3f dif;
  Vector3f startPos;
  Vector4f startPosQ;
  float ratio;
  Quaternion<float> q;


  // Add rotation to facing vector
  // First figure out point off of axis to rotate
  ax1 << facing;
  ax1.normalize();

  // Make sure axis1 and a are not equal
  a << normDir, 0.0, 0.0;
  if(a == ax1){
    a << 0.0, 0.0, normDir;
  }

  // Generate coordinate system
  ax2 << a;
  ax2 = ax2.cross(ax1);
  ax2.normalize();
  ax3 << ax2;
  ax3 = ax3.cross(ax1);
  ax3.normalize();

  // Find point to rotate around facing axis
  ratio = (90.0-angle)/90.0;
  dif = (ax1 - ax2);
  startPos = (ratio * dif) + ax2;
  startPos.normalize();
  startPos = stepSize * startPos * levelRatio;
        
  q = AngleAxis<float>(static_cast<float>((rAng*M_PI)/180.0), ax1);
  startPos = q * startPos;
  startPos.normalize();
  facing = startPos;

}

// Construct the skeleton of the L-System
void LSystem::constructSystem(){
  state << start;

  // Maintain the state of the system during construction
  std::stack<Vector3f> stateStackPos;
  std::stack<Vector3f> stateStackFacing;
  std::stack<int> stateStackLevel;

  float levelRatio;
  int r;
  float rAng, modAng1, modAng2;
  int currentLevel = 0;
  maxLevel = currentLevel;

  cout << "sentence: " << endl;
  for(unsigned int i =0; i < sentence.size(); ++i){
    levelRatio = std::max(static_cast<float>(0.5), 
                          (static_cast<float>(levels) - static_cast<float>(currentLevel))
                           /static_cast<float>(levels));

    // Apply a pseudo random angle change to the branch
    srand ( counter );
    counter = (counter*21)%UINT_MAX;
    r = rand() % 10000000+1;
    rAng = (float)r/10000000.0;

    if(rAng <= 0.25){ rAng = 90.0; }
    else if(rAng <= 0.5){ rAng = 180.0; }
    else if(rAng <= 0.75){ rAng = 270.0; }
    else { rAng = 0.0; }

    r = rand() % 10000000+1;
    modAng1 = (float)r/10000000.0;

    modAng1 = modAng1*5.0; 

    r = rand() % 10000000+1;
    modAng2 = (float)r/10000000.0;

    if(modAng2 <= 0.5){
      modAng2 = -1.0*modAng2;
    }

    // Add or subtract to rAng.  The constraints on ta and tb were determined through
    // some experimentation to generate branches with "natural" looking angles.
    rAng += modAng2;

    cout << sentence[i];

    // Parse through the sentence, generating the points of the L-System skeleton,
    // and pushing or popping the state of the L-System to walk up and down branches
    // as dictated by the grammar.
    switch(sentence[i]){
      case '-' :
        applyGrammarOp(-1.0, rAng, levelRatio);
        break;

      case '+' :
        applyGrammarOp(1.0, rAng, levelRatio);
        break;

      case '[' :
        stateStackPos.push(state);
        stateStackFacing.push(facing);
        stateStackLevel.push(currentLevel);
        break;

      case ']' :
        state = stateStackPos.top();
        facing = stateStackFacing.top();
        currentLevel = stateStackLevel.top();
        stateStackPos.pop();
        stateStackFacing.pop();
        stateStackLevel.pop();
        break;

      case 'X' : case 'Y' : case 'f' :
        state = state + facing*stepSize*levelRatio;
        break;

      case 'F' : case 'A' : case 'B' : case 'C':
        coordList.push_back(state);
        levelList.push_back(currentLevel);
        state = state + facing*stepSize*levelRatio;
        coordList.push_back(state);
        levelList.push_back(currentLevel);
        currentLevel++;
        if(maxLevel < currentLevel){ maxLevel = currentLevel; }
        break;

      default:
        cout << "Sentence size is: " << sentence.size() << endl;
        cout << "   i: " << i << endl;
        throw string("Unhandled switch/case: " )+sentence[i];
    }
  }
}

// Pull the grammar from the xml file
void LSystem::getGrammar(){
  multimap<string, string>::iterator _i; 
  multimap<string, pair<string, string> >::iterator _j;

  pair<multimap<string, string>::iterator,
       multimap<string, string>::iterator> _p0; 
  pair<multimap<string, pair<string, string> >::iterator, 
      multimap<string, pair<string, string> >::iterator> _p1; 

  _p0 = handle->findRangeData("rule");
  _p1 = handle->findRangeAttr("rule");

  _j = _p1.first;
  for(_i = _p0.first; _i != _p0.second; _i++){
    grammar.insert(pair<char, string>( (*_j).second.second[0], (*_i).second));
    _j++;
  }
  printGrammar(); 
}

// Just draw the wire skeleton of the L-System
void LSystem::drawSkeleton() const{
  std::vector<Vector3f>::const_iterator _p0;
  std::vector<Vector3f>::const_iterator _p1;

  glColor4f(0.0, 1.0, 0.0, 1.0);
  glLineWidth(2.0);
  glBegin(GL_LINES);
  for(_p0 = coordList.begin(); _p0 != coordList.end()-1; _p0++){
    _p1 = _p0+1;
    glVertex3f((*_p0)(0), (*_p0)(1), (*_p0)(2));
  }
  glEnd();

  // Y-axis
  glColor4f(0.0, 1.0, 0.0, 1.0);
  glBegin(GL_LINES);
    glVertex3f(100, -100, 0);
    glVertex3f(100, -75, 0);
  glEnd();
  // X-axis
  glColor4f(1.0, 0.0, 0.0, 1.0);
  glBegin(GL_LINES);
    glVertex3f(100, -100, 0);
    glVertex3f(125, -100, 0);
  glEnd();
  // Z-axis
  glColor4f(0.0, 0.0, 1.0, 1.0);
  glBegin(GL_LINES);
    glVertex3f(100, -100, 0);
    glVertex3f(100, -100, 25);
  glEnd();

}

void LSystem::printGrammar() const{
  std::map<char, string>::const_iterator _it;

  cout << endl << "LSystem Grammar: " << endl;
  cout << "Non-T" << '\t' << "RHS" << endl;

  for(_it = grammar.begin(); _it != grammar.end(); _it++){
    cout << _it->first << '\t' << _it->second << endl;
  }
  cout << endl;
}

// This method builds the mesh around the L-System skeleton.  We start by walking down
// the skeleton and generating points to form a top and bottom cylinder for each skeleton
// segment.  We then generate the indices for the polygons using these points.  The
// result is a cylinder around each segment of the section of the L-System. 
PolyMesh * LSystem::buildMesh() const{
  std::vector<Vector3f>::const_iterator _p0;
  std::vector<Vector3f>::const_iterator _p1;
  std::vector<int>::const_iterator _i;

  std::vector<Vector3f*> * points;
  std::list<std::vector<int> *> * polyIndex;
  std::vector<std::vector<int> *>::iterator _pi;
  std::vector<int> * intList;

  Vector3f uX, uY, uZ;
  Vector3f v1, v2;
  Vector3f * newP;
  Matrix3f m;
  float r, R;
  float phi, theta, ratio0, ratio1;
  int N;

  uX << 0,0,0;
  uY << 0,0,0;
  uZ << 0,0,0;

  float modLevels = static_cast<float>(maxLevel);

  r = 0.05; // Radius of the bottom ring of the cylinder
  R = 0.05; // Radios of the top ring of the cylinder
  phi = 60.0;
  N = 360/static_cast<int>(phi); // Number of divisions around the base of the cylinder

  points = new std::vector<Vector3f*>();
  polyIndex = new std::list<std::vector<int> *>();

  // Project points around positions in coordinate list

  // Go ahead and set aside enough space for the vector to hold all of the points
  points->reserve(coordList.size()*(N*2));
  _i = levelList.begin();
  for(_p0 = coordList.begin(); _p0 != coordList.end(); _p0 = _p0+2){
    _p1  = _p0+1;
    ratio0 = std::max(static_cast<float>(0.01), 
                      (modLevels-static_cast<float>((*_i)))/modLevels);
    if(modLevels > (*_i)+1){
      ratio1 = std::max(static_cast<float>(0.01), 
                        (modLevels-static_cast<float>((*_i)))/modLevels);
    }
    else{
      ratio1 = 0.005;
    }
    uY << (*_p1) - (*_p0);
    uY.normalize();
    uX << 1.0, 0.0, 0.0;

    if(uY == uZ){
      uX << 0.0, 0.0, 1.0;
    }
    
    uZ = uX.cross(uY);
    uZ.normalize();
    uX = uZ.cross(uY);
    uX.normalize();

    // Generate points around the bottom ring of the cylinder
    for(theta = 0.0; theta < 360.0; theta = theta + phi){
      newP = new Vector3f();
      m = AngleAxisf((0.0*M_PI)/180.0, uX)
          * AngleAxisf((theta*M_PI)/180.0, uY)
          * AngleAxisf((0.0*M_PI)/180.0, uZ);

      (*newP) << uX*(r * ratio0);
      (*newP) << m * (*newP);
      (*newP) << (*_p0) + (*newP);
      points->push_back(newP); 
    }

    // Generate points around the top ring of the cylinder
    for(theta = 0; theta < 360; theta = theta + phi){
      newP = new Vector3f();
      m = AngleAxisf((0.0*M_PI)/180.0, uX)
          * AngleAxisf((theta*M_PI)/180.0, uY)
          * AngleAxisf((0.0*M_PI)/180.0, uZ);

      (*newP) << uX*(R * ratio1);
      (*newP) << m* (*newP);
      (*newP) << (*_p1) + (*newP);
      points->push_back(newP); 
    }
    _i = _i+2;
  }

  // Build polygons from projected points
  for(unsigned int i = 0; i < coordList.size()-1; i++){
    if(levelList[i] == levelList[i+1] || levelList[i] == levelList[i+1]-1){
      // Build individual polygons
      for(int j = 0; j < N-1; j++){
        intList = new std::vector<int>();
        intList->reserve(4);
        intList->push_back(static_cast<int>(j + N*i));
        intList->push_back(static_cast<int>(j + N*(i+1)));
        intList->push_back(static_cast<int>(j + N*(i+1) + 1));
        intList->push_back(static_cast<int>(j + N*i + 1));
        polyIndex->push_back(intList);
      }
      intList = new std::vector<int>();
      intList->reserve(4);
      intList->push_back(static_cast<int>(N-1 + N*i));
      intList->push_back(static_cast<int>(N-1 + N*(i+1)));
      intList->push_back(static_cast<int>(N*(i+1)));
      intList->push_back(static_cast<int>(N*i));
      polyIndex->push_back(intList);
    }
  }

  return new PolyMesh(points, polyIndex, boundRad);
   
}
