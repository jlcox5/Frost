/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the models.
 *
 */

#ifndef MODELFACTORY_H_
#define MODELFACTORY_H_

// Standard Libraries
#include <map>
#include <algorithm>
#include <string>

// Custom Libraries
#include "parseXML.h"
#include "handleChars.h"
#include "model.h"
#include "fbxmesh.h"
#include "polymesh.h"

using std::map; using std::string;

class ModelFactory{
  public:
    ModelFactory();
    ~ModelFactory();
    Model * getModel(string const &);
    bool eraseModel(string const &);
    void pushModel(string const &, Model*);

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  private:
    map<string, Model*> modelList;

    ModelFactory(ModelFactory const &);
    ModelFactory& operator=(ModelFactory const &);
};

#endif
