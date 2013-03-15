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
                                     stateStackPos(),
                                     stateStackFacing(),
                                     sentence(),
                                     coordList(){

  cout << "Building lsystem..." << endl;
  loadSystem(fn);
  cout << "   Load system complete." << endl;
  buildSentence();
  cout << "   Sentence build complete." << endl;
  constructSystem();
  cout << "Finished lsystem..." << endl;

  cout << "Building mesh..." << endl;
  testPolyMesh = buildMesh();
  cout << "Finished." << endl;

}

LSystem::~LSystem(){
  delete handle;
}

void LSystem::loadSystem(string const & fn){
  ParseXML * parser = ParseXML::getInstance();
  parser->parseXML(fn, handle);
  string data;

  cout << "     Setting xml values..." << endl;
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

  cout << "     Finding start symbol values..." << endl;
  /*std::map<string, string>::const_iterator ptr = handle->getXmlString("startSymbol");
  if(ptr != xmlData.end()){
    data = ptr->second;
    startSymbol = data[0];
  }*/
  startSymbol = '\0';
  startSymbol = handle->getXmlStr("startSymbol")[0];
  if(startSymbol == '\0'){
    throw string("Unable to locate start symbol for lsystem.");
  }

  cout << "     Parsing grammar..." << endl;
  getGrammar();
}

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

void LSystem::constructSystem(){
  state << start;

  Vector3f v1, v2, v3;
  Vector3f a;
  Vector3f dif;
  Vector3f startPos;
  Vector4f startPosQ;
  float ratio;
  float levelRatio;
  Quaternion<float> q;
  int r;
  float t, ta, tb;
  int currentLevel = 0;
  maxLevel = currentLevel;

  cout << "sentence: " << endl;
  for(unsigned int i =0; i < sentence.size(); ++i){
    levelRatio = std::max(static_cast<float>(0.5), (static_cast<float>(levels) - static_cast<float>(currentLevel))/
                  static_cast<float>(levels));
    srand ( counter );
    counter = (counter*21)%UINT_MAX;
    r = rand() % 10000000+1;
    t = (float)r/10000000.0;

    if(t <= 0.25){ t = 90.0; }
    else if(t <= 0.5){ t = 180.0; }
    else if(t <= 0.75){ t = 270.0; }
    else { t = 0.0; }

    srand ( counter );
    counter = (counter*21)%UINT_MAX;
    r = rand() % 10000000+1;
    ta = (float)r/10000000.0;

    ta = ta*5.0; 

    srand ( counter );
    counter = (counter*21)%UINT_MAX;
    r = rand() % 10000000+1;
    tb = (float)r/10000000.0;

    if(tb <= 0.5){
      ta = -1.0*ta;
    }

    t +=ta;

   cout << sentence[i];

    switch(sentence[i]){
      case '-' :
        // Add rotation to facing vector
        // First figure out point off of axis to rotate
        v1 << facing;
        v1.normalize();

        // Make sure v1 and a are not equal
        a << -1.0, 0.0, 0.0;
        if(a == v1){
          a << 0.0, 0.0, -1.0;
        }

        // Generate local coordinate system
        v2 << a;
        v2 = v2.cross(v1);
        v2.normalize();
        v3 << v2;
        v3 = v3.cross(v1);
        v3.normalize();

        // Find point to rotate around facing axis
        ratio = (90.0-angle)/90.0;
        dif = (v1 - v2);
        startPos = (ratio * dif) + v2;
        startPos.normalize();
        startPos = stepSize * startPos* levelRatio;
        
        q = AngleAxis<float>(static_cast<float>((t*M_PI)/180.0), v1);
        startPos = q * startPos;
        startPos.normalize();
        facing = startPos;
        //cout << "  f: " << facing(0) << " " << facing(1) << " " << facing(2) << endl;
        break;

      case '+' :
        // Add rotation to facing vector
        // First figure out point off of axis to rotate
        v1 << facing;
        v1.normalize();

        // Make sure v1 and a are not equal
        a << 1.0, 0.0, 0.0;
        if(a == v1){
          a << 0.0, 0.0, 1.0;
        }

        // Generate local coordinate system
        v2 << a;
        v2 = v2.cross(v1);
        v2.normalize();
        v3 << v2;
        v3 = v3.cross(v1);
        v3.normalize();

        // Find point to rotate around facing axis
        ratio = (90.0-angle)/90.0;
        dif = (v1 - v2);
        startPos = (ratio * dif) + v2;
        startPos.normalize();
        startPos = stepSize * startPos * levelRatio;
        
        q = AngleAxis<float>((t*M_PI)/180.0, v1);
        startPos = q * startPos;
        startPos.normalize();
        facing = startPos;
        //cout << "  f: " << facing(0) << " " << facing(1) << " " << facing(2) << endl;
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
  cout << endl;
}

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

PolyMesh * LSystem::buildMesh() const{
  std::vector<Vector3f>::const_iterator _p0;
  std::vector<Vector3f>::const_iterator _p1;
  std::vector<int>::const_iterator _i;
  std::vector<Vector3f*> * points;
  std::vector<std::vector<int> *> * polyIndex;
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

  r = 0.05;
  R = 0.05;
  phi = 60.0;
  N = 360/static_cast<int>(phi);

  points = new std::vector<Vector3f*>();
  polyIndex = new std::vector<std::vector<int> *>();

  // Project points around positions in coordinate list
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
    theta = 0;
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

    // Bottom ring
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

    // Top ring
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
        //cout << "j: " << j << "  N: " << N << endl;
        intList = new std::vector<int>();
        intList->push_back(static_cast<int>(j + N*i));
        intList->push_back(static_cast<int>(j + N*(i+1)));
        intList->push_back(static_cast<int>(j + N*(i+1) + 1));
        intList->push_back(static_cast<int>(j + N*i + 1));
        polyIndex->push_back(intList);
      }
      intList = new std::vector<int>();
      intList->push_back(static_cast<int>(N-1 + N*i));
      intList->push_back(static_cast<int>(N-1 + N*(i+1)));
      intList->push_back(static_cast<int>(N*(i+1)));
      intList->push_back(static_cast<int>(N*i));
      polyIndex->push_back(intList);
    }
  }

  cout << "Polygons before cleanup: " << polyIndex->size() << endl;
  sort(polyIndex->begin(), polyIndex->end(), sortPoly);
  for(_pi = polyIndex->begin()+1; _pi != polyIndex->end(); _pi++){
    if((*_pi)->at(0) == (*(_pi-1))->at(0)){
      if((*_pi)->at(1) == (*(_pi-1))->at(1)){
        if((*_pi)->at(2) == (*(_pi-1))->at(2)){
          delete (*_pi);
          _pi = polyIndex->erase(_pi);
        }
      }
    } 
  }
  cout << "Polygons after cleanup: " << polyIndex->size() << endl;

  return new PolyMesh(*points, *polyIndex, boundRad);
   
}
