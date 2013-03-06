/* BWS MMO Engine
 * Author: Jonathan Cox
 * 
 * This class parses XML data.
 *
 */

#ifndef PARSEXML_H_
#define PARSEXML_H_

// Standard Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <expat.h>

// Custom Libraries
#include "handleChars.h"

using std::map;
using std::multimap;

using std::cout; using std::endl;
using std::string; 
const int BUFSIZE = 8128;

class ParseXML {
public:
  static ParseXML * getInstance();
  void parseXML(string const &, HandleChars *);
  virtual ~ParseXML() { XML_ParserFree(parser); }

  const string& getLastTag() const { return lastTag; }
  const string& getNextToLastTag() const { return nextToLastTag; }

  static void wrapper4Start(void *data, const char *el, const char **attr);
  static void wrapper4End(void *data, const char *el);
  static void wrapper4Chars(void *data, const char *text, int textlen);

private:
  static ParseXML * inst;
  XML_Parser parser;
  HandleChars * handleChars;

  char buff[BUFSIZE];
  string  nextToLastTag;
  string  lastTag;

  void parseXML();
  void start(const char *el, const char *attr[]);
  void end(const char *) {}
  void chars(const char *text, int textlen);

  void stripTrailWhiteSpace(string&) const;
  void stripWhiteSpace(string&) const;

  ParseXML() : parser(NULL), handleChars(NULL), nextToLastTag(), lastTag(){ cout << "Constructed ParseXML" << endl; }
  ParseXML& operator=(const ParseXML&);
  ParseXML(const ParseXML&);
};

#endif

