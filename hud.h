/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles the hud.
 *
 */

#ifndef HUD_H_
#define HUD_H_

// Standard Libraries

// Imported Libraries
#include "Eigen/Dense"

// Custom Libraries
#include "parseXML.h"
#include "handleChars.h"
#include "model.h"
#include "sceneobj.h"
#include "assets.h"

using Eigen::Vector3f; using Eigen::Vector4f;

class Hud{
  public:
    Hud();
    ~Hud();

    std::vector<string> const & getModelList() const{ return modelList; }
    std::vector<SceneOBJ*> const & getSceneObjList() const{ return sceneObjList; }
    void loadHud(string const &);
    void loadModels(string const &);
    void loadSceneObj(string const &);

    bool displayHud() const{ return display; }
    void toggleDisplay();

    void displayHealthBar() const;

  private:
    std::vector<string> modelList;
    std::vector<SceneOBJ*> sceneObjList;
    bool display;

    Hud(Hud const &);
    Hud& operator=(Hud const &);

};

#endif
