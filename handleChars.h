/* BWS MMO Engine
 * Author: Jonathan Cox
 * 
 * This class is a handler for parsing XML data.
 *
 */

#ifndef HANDLECHARS_H_
#define HANDLECHARS_H_

// Standard Libraries
#include <string>
#include <map>
#include <vector>

using std::string;  using std::multimap; using std::pair;

class HandleChars{
  public:
    HandleChars() : xmlData(){}
    void operator()(string const &);
    void insertAttr(string const &, std::pair<string, string>);
    const multimap<string, string>& getData() const { return xmlData; }
    const multimap<string, pair<string, string> >& getAttr() const { return xmlAttr; }

    float getXmlFloat(string const &) const;
    int getXmlInt(string const &) const;
    string const & getXmlStr(string const &) const;
    pair<multimap<string, string>::iterator, 
         multimap<string, string>::iterator> findRangeData(string const &);
    pair<multimap<string, pair<string, string> >::iterator, 
        multimap<string, pair<string, string> >::iterator> findRangeAttr(string const &);

  private:
    multimap<string, string> xmlData;
    multimap<string, std::pair<string, string> > xmlAttr;
};

#endif

