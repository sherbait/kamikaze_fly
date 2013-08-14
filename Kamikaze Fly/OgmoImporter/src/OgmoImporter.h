#pragma once

#include "stdafx.h"
#include "src\tinystr.h"
#include "src\tinyxml.h"
#include "src\game\Game.h"
#include "src\gsm\state\GameStateManager.h"
#include "src\XMLFileImporter.h"

// XML TEXT
static const string		LEVEL_ELEMENT			= "level";
static const string		WIDTH_ATT				= "width";
static const string		HEIGHT_ATT				= "height";
static const string		BACKGROUND_LAYER		= "Background";
static const string		COLLISION_LAYER			= "Collision";
static const string		CLUTTER_LAYER			= "Clutter";
static const string		ENEMIES_LAYER			= "Enemies";
static const string		STRUCTURES_LAYER		= "Structures";
static const int		COLLISION_TILE_SIZE		= 32;
static const int		MAX_STATIC_SIZE			= 100;
static const char*		SCRIPT_PATH				= "data/scripts/";

class OgmoImporter : public XMLFileImporter
{
protected:
	int width;
	int height;
	wstring dir;
	wstring mapLevelFilePath;

public:
	OgmoImporter() {}
	~OgmoImporter(){}
	bool loadLevel(wstring initDir, wstring mapLevelFileName);

	// THESE ARE ALL THE HELPER METHODS
private:
	void loadBackgroundLayer(const TiXmlNode *node);
	void loadCollisionLayer(const TiXmlNode *node);
	void loadClutterLayer(const TiXmlNode *node);
	void loadStructuresLayer(const TiXmlNode *node);
	void loadEnemiesLayer(const TiXmlNode *node);
};