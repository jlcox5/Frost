/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class is a handler for parsing XML data.
 *
 */

// Standard Libraries
#include <sstream>
#include <string>
#include <map>
using std::multimap;  using std::string;

// Custom Libraries
#include "parseXML.h"

void HandleChars::operator()(string const & str){
  string lastTag = ParseXML::getInstance()->getLastTag();
  xmlData.insert(std::pair<string,string>(lastTag, str));
}

void HandleChars::insertAttr(string const & str, std::pair<string, string> attr){
  xmlAttr.insert(pair<string, pair<string, string> >(str, attr));
}

int HandleChars::getXmlInt(string const & tag) const {
  std::multimap<string, string>::const_iterator ptr = xmlData.find(tag);
  if ( ptr == xmlData.end() )
    throw string("Didn't find integer tag in xml");
  else {
    std::stringstream strm;
    strm << ptr->second;
    int data;
    strm >> data;
    return data;
  }
}

float HandleChars::getXmlFloat(string const & tag) const {
  std::multimap<string, string>::const_iterator ptr = xmlData.find(tag);
  if ( ptr == xmlData.end() )
    throw string("Didn't find float tag in xml");
  else {
    std::stringstream strm;
    strm << ptr->second;
    float data;
    strm >> data;
    return data;
  }
}

string const & HandleChars::getXmlStr(const string& tag) const {
  std::multimap<string, string>::const_iterator ptr = xmlData.find(tag);
  if ( ptr == xmlData.end() )
    throw string("Didn't find string tag in xml");
  else return ptr->second;
}

pair<multimap<string, string>::iterator, multimap<string, string>::iterator> 
 HandleChars::findRangeData(string const & str){
  return xmlData.equal_range(str);
}

pair<multimap<string, pair<string, string> >::iterator, 
     multimap<string, pair<string, string> >::iterator>
     HandleChars::findRangeAttr(string const & str){
  return xmlAttr.equal_range(str);
}

