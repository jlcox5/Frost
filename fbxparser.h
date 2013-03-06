	/* BWS MMO Engine
 * Author: Jonathan Cox
 *
 * This class handles parsing fbx files and allocating the resources to the
 * appropriate data structures.
 *
 */

#ifndef FBXPARSER_H_
#define FBXPARSER_H_

// Standard Libraries
#include <string>
#include <vector>

// FBX Library
#include <fbxsdk.h>

// Custom Libraries
#include "assets.h"

using std::string;

class FBXParser{

  public:
    static FBXParser * getInstance();
    ~FBXParser();

    // Load an FBX File into the assets
    void loadFBX(string const &, Assets&);
    void traverseElements(Assets&, KFbxNode const &, string const &) const;
    void storeMesh(KFbxNode const &, Assets&, string const &) const;
    void storeTexture(KFbxNode const &, Assets&) const;

    // Functions to assist printing
    void printScene(KFbxScene const &);
    void printNode(KFbxNode const &);
    void printAttribute(KFbxNodeAttribute const &) const;
    const KString getAttributeTypeName(KFbxNodeAttribute::EAttributeType) const;
    void printTabs() const;

  private:
    static FBXParser * inst;
    static KFbxSdkManager * lManager;
    std::vector<KFbxScene*> lSceneList;
    int numTabs;

    // Do not need more than one parser the engine
    FBXParser();
    FBXParser(FBXParser const &);
    FBXParser& operator=(FBXParser const &);

};

#endif
