#include <cstring>
#include <sstream>
#include "parseXML.h"

ParseXML * ParseXML::getInstance(){
  if(!inst){
    inst = new ParseXML();
  }
  return inst;
}

void ParseXML::parseXML(string const & filename, HandleChars * hand) {
  parser = XML_ParserCreate(NULL);
  if (!parser) {
    throw string("Couldn't allocate memory for parser");
  }

  handleChars = hand;
  nextToLastTag = "";
  lastTag = "";

  XML_SetElementHandler(parser, wrapper4Start, wrapper4End);
  XML_SetCharacterDataHandler(parser, wrapper4Chars);
  std::fstream in;
  in.open(filename.c_str(), std::ios::in);
  if (!in) { 
    throw string("Cannot open xml file: ")+filename;
  }

  int length = 0;
  in.getline(buff, BUFSIZE);
  while ( true ) {

    if (! XML_Parse(parser, buff, strlen(buff), length)) {
      cout << "Parse error at line "
	         << XML_GetCurrentLineNumber(parser)
	         << XML_ErrorString(XML_GetErrorCode(parser))
           << endl;
      exit(-1);
    }

    if ( in.eof() ) break;
    else in.getline(buff, BUFSIZE);
  }
}

void ParseXML::start(const char *el, const char *attr[]) {
  nextToLastTag = lastTag;
  lastTag = el;
  //cout << el << " ";
  for(int i = 0; attr[i]; i += 2){
    handleChars->insertAttr(el, std::pair<string, string>(attr[i], attr[i+1]));
  }
}

void ParseXML::stripTrailWhiteSpace(std::string& str) const {
  int length = str.size();   
  int i = length-1;
  while (i >= 0) { 
    if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t') {
      break;
    }
    else if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
      str.erase(i, 1);
    }
    --i;
  }
}

void ParseXML::stripWhiteSpace(std::string& str) const {
  int length = str.size();   
  int i = 0;
  while (i < length) { 
    if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
      str.erase(i, 1);
      i--;
    }
    ++i;
  }
  str[i]='\0';
}

void ParseXML::chars(const char *text, int textlen) {
  // The text is not zero terminated; thus we need the  length:
  string str(text, textlen);
  // The text is likely to have trailing white space, e.g. newline, etc
  stripTrailWhiteSpace(str);
  if ( str.size() ) {
    if( handleChars) (*handleChars)(str);
  }
}

void ParseXML::wrapper4Start(void *data, const char *el, const char **attr) {
  ParseXML::getInstance()->start(el, attr);
}

void ParseXML::wrapper4End(void *data, const char *el) {
  ParseXML::getInstance()->end(el);
}

void ParseXML::wrapper4Chars(void *data, const char *text, int textlen) {
  ParseXML::getInstance()->chars(text, textlen);
}
