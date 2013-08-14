#include "stdafx.h"
#include "OgmoImporter.h"
#include "src\tinystr.h";
#include "src\tinyxml.h";
#include "src\game\Game.h"
#include "src\gsm\world\TiledLayer.h"
#include "src\gsm\world\SparseLayer.h"
#include "src\gsm\ai\LuaBot.h"
#include "src\gsm\ai\bots\Block.h"
#include "src\gsm\ai\bots\Goal.h"
#include "src\gsm\world\Tile.h"
#include "src\XMLFileImporter.h"

bool OgmoImporter::loadLevel(wstring initDir, wstring mapLevelFileName)
{
	dir = initDir;
	mapLevelFilePath = dir + mapLevelFileName;
	
	const char *charPath = newCharArrayFromWstring(mapLevelFilePath);
	TiXmlDocument doc(charPath);
	delete charPath;
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		TiXmlElement *pElem = doc.FirstChildElement();
		string eName;
		if (pElem)
		{
			eName = pElem->Value();
			if (strcmp(eName.c_str(), LEVEL_ELEMENT.c_str()) == 0)
			{
				this->width = extractIntAtt(pElem, WIDTH_ATT);
				this->height = extractIntAtt(pElem, HEIGHT_ATT);
				World* world = Game::getInstance()->getGSM()->getWorld();
				world->setWorldWidth(width);
				world->setWorldHeight(height);
				
				const TiXmlNode *node = pElem->FirstChild();
				while (node)
				{
					string eName = node->Value();
					if (strcmp(eName.c_str(), BACKGROUND_LAYER.c_str()) == 0)
					{
						loadBackgroundLayer(node);
					}
					else if (strcmp(eName.c_str(), COLLISION_LAYER.c_str()) == 0)
					{
						loadCollisionLayer(node);
					}
					else if (strcmp(eName.c_str(), CLUTTER_LAYER.c_str()) == 0)
					{
						loadClutterLayer(node);
					}
					else if (strcmp(eName.c_str(), ENEMIES_LAYER.c_str()) == 0)
					{
						loadEnemiesLayer(node);
					}
					else if (strcmp(eName.c_str(), STRUCTURES_LAYER.c_str()) == 0)
					{
						loadStructuresLayer(node);
					}
					node = node->NextSibling();
				}
				return true;
				
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
	
	return true;
}

void OgmoImporter::loadBackgroundLayer(const TiXmlNode *node)
{
	Game* game = Game::getInstance();
	World* world = game->getGSM()->getWorld();
	TextureManager *worldTextureManager = game->getGraphics()->getWorldTextureManager();	

	const TiXmlNode* image = node->FirstChild();

	//while (image)
	{
		Tile *imageTile = new Tile();
		imageTile->collidable = false;

		std::string source = image->Value();
		wstring imageSourceW(source.begin(), source.end());
		imageTile->textureID = worldTextureManager->loadTexture(dir + imageSourceW + L".png");

		int imageWidth = extractIntAtt(image->ToElement(), WIDTH_ATT);
		int imageHeight = extractIntAtt(image->ToElement(), HEIGHT_ATT);

		TiledLayer *imageLayerToAdd = new TiledLayer(1,1,imageWidth,imageHeight,0,false,width,width);
		world->addLayer(imageLayerToAdd);

		imageLayerToAdd->addTile(imageTile);

		image->NextSibling();
	}
}

void OgmoImporter::loadClutterLayer(const TiXmlNode *node)
{
	Game* game = Game::getInstance();
	World* world = game->getGSM()->getWorld();
	TextureManager *worldTextureManager = game->getGraphics()->getWorldTextureManager();	

	SparseLayer* clutterLayer = new SparseLayer();
	clutterLayer->setCollidableTiles(false);
	clutterLayer->setWorldWidth(width);
	clutterLayer->setWorldHeight(height);
	world->addLayer(clutterLayer);

	const TiXmlNode* object = node->FirstChild();
	while (object)
	{
		std::string name = object->Value();
		wstring imageSourceW = wstring(name.begin(), name.end());
		int id = worldTextureManager->loadTexture(dir + imageSourceW + L".png");

		int x = extractIntAtt(object->ToElement(), "x");
		int y = extractIntAtt(object->ToElement(),"y");
		int w = extractIntAtt(object->ToElement(),WIDTH_ATT);
		int h = extractIntAtt(object->ToElement(),HEIGHT_ATT);

		OverlayImage* image = new OverlayImage(id,x,y,0,255,w,h);
		clutterLayer->addTile(image);
		object = object->NextSibling();
	}
}

void OgmoImporter::loadCollisionLayer(const TiXmlNode *node)
{
	Physics* phys = Game::getInstance()->getGSM()->getPhysics();
	const TiXmlNode* rect = node->FirstChild();

	while(rect)
	{
		int x = extractIntAtt(rect->ToElement(),"x");
		int w = extractIntAtt(rect->ToElement(),"w");
		
		for (int i = w; i > 0; i -= MAX_STATIC_SIZE)
		{
			int y = extractIntAtt(rect->ToElement(),"y");
			int h = extractIntAtt(rect->ToElement(),"h");
			int adjustedWidth;
			if (i > MAX_STATIC_SIZE)
			{
				adjustedWidth = MAX_STATIC_SIZE * COLLISION_TILE_SIZE;
			}
			else
			{
				adjustedWidth = i * COLLISION_TILE_SIZE;
			}
			for (int j = h; j > 0; j -= MAX_STATIC_SIZE)
			{
				int adjustedHeight;
				if (j > MAX_STATIC_SIZE)
				{
					adjustedHeight = MAX_STATIC_SIZE * COLLISION_TILE_SIZE;
				}
				else
				{
					adjustedHeight = j * COLLISION_TILE_SIZE;
				}
				phys->addStaticRectangle(x*COLLISION_TILE_SIZE,y*COLLISION_TILE_SIZE,adjustedWidth,adjustedHeight);
				y += MAX_STATIC_SIZE;
			}
			x += MAX_STATIC_SIZE;
		}
		
		rect = rect->NextSibling();
	}
}

void OgmoImporter::loadEnemiesLayer(const TiXmlNode* node)
{
	GameStateManager* gsm = Game::getInstance()->getGSM();
	SpriteManager* spriteManager = gsm->getSpriteManager();
	Physics* phys = gsm->getPhysics();
	
	const TiXmlNode* enemy = node->FirstChild();
	while (enemy)
	{
		const TiXmlElement* enemyElem = enemy->ToElement();
		string spriteTypeName = enemyElem->Value();
		//int spriteTypeId = extractIntAtt(enemyElem,"SpriteType");
		AnimatedSpriteType* spriteType = spriteManager->getSpriteType(wstring(spriteTypeName.begin(),spriteTypeName.end()));
		int x = extractIntAtt(enemyElem,"x");
		int y = extractIntAtt(enemyElem,"y");
		std:string ai = extractCharAtt(enemyElem, "AI");
		ai = SCRIPT_PATH + ai + ".lua";
		LuaBot* bot = new LuaBot(ai.c_str());

		bot->setSpriteType(spriteType);
		bot->setCurrentState(L"LEFT");
		bot->setAlpha(255);
		spriteManager->addBot(bot);
		phys->addCollidableObject(bot, x, y, spriteType->getTextureWidth(), 
		spriteType->getTextureHeight(), 0, 1, 0.3, true, 0, false);
		bot->affixTightAABBBoundingVolume();

		enemy = enemy->NextSibling();
	}
}

void OgmoImporter::loadStructuresLayer(const TiXmlNode* node)
{
	GameStateManager* gsm = Game::getInstance()->getGSM();
	Physics* physics = gsm->getPhysics();
	SpriteManager* spriteManager = gsm->getSpriteManager();

	const TiXmlElement* structureElem = node->ToElement()->FirstChildElement();
	while (structureElem)
	{
		string blockType = structureElem->Value();
		int x = extractIntAtt(structureElem, "x");
		int y = extractIntAtt(structureElem,"y");
		int w = extractIntAtt(structureElem,WIDTH_ATT);
		int h = extractIntAtt(structureElem,HEIGHT_ATT);
		bool isGoal = extractBoolAtt(structureElem, "goal");
		float angle;
		structureElem->QueryFloatAttribute("angle", &angle);

		if (isGoal)
		{
			Goal* goal = new Goal();
			AnimatedSpriteType* spriteType = spriteManager->getSpriteType(wstring(blockType.begin(), blockType.end()));
			goal->setSpriteType(spriteType);
			goal->setCurrentState(L"IDLE");
			goal->setAlpha(255);
			physics->addDynamicRectangle(goal,x,y,w,h,angle,1,0.3f,1);
			spriteManager->addGoalObject(goal);
			AnimatedSpriteType *animationSpriteType = spriteManager->getSpriteType(13);
			AnimatedOverlay* animation = new AnimatedOverlay();
			animation->setSpriteType(animationSpriteType);
			animation->setAlpha(255);
			animation->setCurrentState(L"BOUNCING");
			animation->followCollidableObject(goal);
			animation->setZ(0);
			spriteManager->addAnimatedOverlay(animation);
		}
		else
		{
			Block* block = new Block();
			AnimatedSpriteType* spriteType = spriteManager->getSpriteType(wstring(blockType.begin(), blockType.end()));
			block->setSpriteType(spriteType);
			block->setCurrentState(L"IDLE");
			block->setAlpha(255);
			block->setCollisionType(CollisionType::BLOCK);
			physics->addDynamicRectangle(block,x,y,w,h,angle,1,0.3f,1);
			spriteManager->addBot(block);
		}


		structureElem = structureElem->NextSiblingElement();
	}
}