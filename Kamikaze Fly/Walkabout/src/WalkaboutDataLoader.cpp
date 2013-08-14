#include "stdafx.h"

// Walkabout GAME INCLUDES
#include "src\WalkaboutButtonEventHandler.h"
#include "src\WalkaboutDataLoader.h"
#include "src\WalkaboutGame.h"
#include "src\WalkaboutKeyEventHandler.h"
#include "src\WalkaboutTextGenerator.h"

// GAME OBJECT INCLUDES
#include "src\game\Game.h"
#include "src\graphics\GameGraphics.h"
#include "src\gsm\ai\LuaBot.h"
#include "src\gsm\ai\bots\Block.h"
#include "src\gsm\state\GameState.h"
#include "src\gsm\world\TiledLayer.h"
#include "src\gui\Cursor.h"
#include "src\gui\GameGUI.h"
#include "src\gui\ScreenGUI.h"
#include "src\input\GameInput.h"
#include "src\os\GameOS.h"
#include "src\text\GameText.h"
#include "src\Player.h"
#include "src\audio\AudioManager.h"

// WINDOWS PLATFORM INCLUDES
#include "src\PlatformPlugins\WindowsPlugin\WindowsOS.h"
#include "src\PlatformPlugins\WindowsPlugin\WindowsInput.h"
#include "src\PlatformPlugins\WindowsPlugin\WindowsTimer.h"

// DIRECTX INCLUDES
#include "src\PlatformPlugins\DirectXPlugin\DirectXGraphics.h"
#include "src\PlatformPlugins\DirectXPlugin\DirectXTextureManager.h"

// TMX MAP LOADING
//#include "src\TMXMapImporter.h"

#include "src\OgmoImporter.h"

// ANIMATED SPRITE TYPE LOADING
#include "src\PoseurSpriteTypesImporter.h"

/*
loadGame - This method loads the setup game data into the game and
constructs all the needed objects for the game to work.
*/
void WalkaboutDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[W_TITLE];
	wstring useFullscreenProp = (*properties)[W_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	int screenWidth, screenHeight;
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	WindowsOS *walkaboutOS = new WindowsOS(	hInstance, 
		nCmdShow,
		useFullscreen,
		titleProp,
		screenWidth, screenHeight,
		game);

	int textFontSize;
	wstring textFontSizeProp = (*properties)[W_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *walkaboutGraphics = new DirectXGraphics(game);
	walkaboutGraphics->init(screenWidth, screenHeight);
	walkaboutGraphics->initGraphics(walkaboutOS, useFullscreen);
	walkaboutGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(walkaboutGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *walkaboutInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *walkaboutTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initPlatformPlugins(	(GameGraphics*)walkaboutGraphics,
		(GameInput*)walkaboutInput,
		(GameOS*)walkaboutOS,
		(GameTimer*)walkaboutTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	WalkaboutTextGenerator *walkaboutTextGenerator = new WalkaboutTextGenerator();
	walkaboutTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(W_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)walkaboutTextGenerator);

	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);	

	// Init the audio
	initAudio(game);

	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;
}

/*
	Loads all sounds into the AudioManager class.
*/
void WalkaboutDataLoader::initAudio(Game *game)
{
	//int min = 1, max = 5000;
	AudioManager *audio = game->getAudioManager();

	// Background Music
	audio->addSound(C_MAIN_AUDIO_PATH, true);			/* Plays at the menu screen. */
	audio->addSound(C_LEVEL_1_AUDIO_PATH, true);		/* Level 1 music. */
	audio->addSound(C_LEVEL_2_AUDIO_PATH, true);		/* Level 2 music. */
	audio->addSound(C_LEVEL_3_AUDIO_PATH, true);		/* Level 3 music. */

	// Sprites
	audio->addSound(C_BEE_AUDIO_PATH, false);			/* Buzzing BEE sound. */
	audio->addSound(C_BUTTERFLY_AUDIO_PATH, false);		/* Flapping BUTTERFLY sound. */
	audio->addSound(C_FLY_LONG_AUDIO_PATH, true);		/* Option to play a continuous FLY sound throughout the level. */
	audio->addSound(C_FLY_SHORT_AUDIO_PATH, false);		/* Short-burst FLY sound. Can be played at the beginning of each level. */
	audio->addSound(C_MOSQUITO_AUDIO_PATH, false);		/* Buzzing MOSQUITO sound. */
	audio->addSound(C_VENUS_AUDIO_PATH, false);			/* Snapping VENUS FLY TRAP sound. This has to be played coincidentally with the snapping of a fly trap's mouth. */

	// Explosion
	audio->addSound(C_BLAST_SM_AUDIO_PATH, false);		/* BOOM! */
	audio->addSound(C_BLAST_MD_AUDIO_PATH, false);		/* BOOOOM!	*/
	audio->addSound(C_BLAST_LG_AUDIO_PATH, false);		/* BOOOOOOOOasdasdasdgdsjhjMMMM! */
	
	// Miscellaneous
	audio->addSound(C_BUTTON_CLICK_AUDIO_PATH, false);	/* Button clicks sound */
}

/*
initColors - this helper method loads the color key, used for loading
images, and the font color, used for rendering text.
*/
void WalkaboutDataLoader::initColors(	GameGraphics *graphics,
									 map<wstring,wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[W_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[W_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[W_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[W_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[W_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[W_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

/*
loadGUI - This method loads all the GUI assets described in the guiInitFile
argument. Note that we are loading all GUI art for all GUIs when the application
first starts. We'll learn a better technique later in the semester.
*/
void WalkaboutDataLoader::loadGUI(Game *game, wstring guiInitFile)
{
	// WE'RE JUST GOING TO IGNORE THE GUI FILE FOR NOW.
	// FOR THE MOMENT WE ARE CALLING THIS HARD-CODED GUI LOADER
	hardCodedLoadGUIExample(game);
}

/*
loadLevel - This method should load the data the level described by the
levelInitFile argument in to the Game's game state manager.
*/
void WalkaboutDataLoader::loadWorld(Game *game, wstring levelInitFile)	
{
	GameStateManager *gsm = game->getGSM();
	game->getGUI()->getScreen(GS_GAME_IN_PROGRESS)->clearScreen();
	gsm->getSpriteManager()->clearSprites();
	gsm->getWorld()->unloadWorld();
	Viewport *viewport = game->getGUI()->getViewport();
	viewport->setViewportX(0);
	viewport->setViewportY(0);
	Physics *physics = gsm->getPhysics();
	physics->resetWorld();

	// This is used to change where the restart button points to
	wstring currentLevelName = game->getCurrentLevelFileName();
	wstring command;
	if (currentLevelName.compare(W_LEVEL_1_NAME) == 0)
	{
		command = W_LEVEL_1_COMMAND;
	}
	else if (currentLevelName.compare(W_LEVEL_2_NAME) == 0)
	{
		command = W_LEVEL_2_COMMAND;
	}
	else if (currentLevelName.compare(W_LEVEL_3_NAME) == 0)
	{
		command = W_LEVEL_3_COMMAND;
	}

	ScreenGUI* pauseScreen = game->getGUI()->getScreen(GS_PAUSED);
	list<Button*>::iterator buttonIt = pauseScreen->getButtonsIterator();
	int buttonImageIndex = game->getGraphics()->getGUITextureManager()->getWStringTable()->getIndexOfWString(W_RESTART_IMAGE_PATH);
	while (buttonIt != pauseScreen->getEndButtonsIterator())
	{
		Button* button = *buttonIt;
		if (buttonImageIndex == button->getNormalTextureID())
		{
			button->setCommand(command);
		}
		buttonIt++;
	}

	OgmoImporter ogmoImporter;
	ogmoImporter.loadLevel(W_LEVEL_DIR, game->getCurrentLevelFileName());

	physics->setGravity(W_GRAVITY);
	SpriteManager *spriteManager = gsm->getSpriteManager();
	AnimatedSprite *player = new Player();

	// NOTE THAT PLAYER IS SPRITE ID 2
	AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(2);
	player->setSpriteType(playerSpriteType);
	player->setAlpha(255);
	player->setCurrentState(IDLE);
	player->setCollisionType(CollisionType::PLAYER);

	spriteManager->setPlayer(player);
	spriteManager->reserveProjectiles(100);

	physics->addCollidableObject(player, PLAYER_INIT_X, PLAYER_INIT_Y, playerSpriteType->getTextureWidth(), 
	playerSpriteType->getTextureHeight(), 0, 20.0f, 0.0f, true, 0, false);
	player->getPhysicalProperties()->setVelocity(PLAYER_SPEED, 0);

	game->setWaypoint(PLAYER_INIT_X, PLAYER_INIT_Y);
}

void WalkaboutDataLoader::initNightRunLevel(Game *game)
{
	//Physics* physics = game->getGSM()->getPhysics();
	//// LOAD THE LEVEL'S BACKGROUND TILES
	//TMXMapImporter tmxMapImporter;
	//tmxMapImporter.loadWorld(game, W_LEVEL_1_DIR, W_LEVEL_1_NAME);

	//vector<WorldLayer*>::iterator tiledLayerIterator;
	//tiledLayerIterator = gsm->getWorld()->getLayers()->begin();
	//while (tiledLayerIterator != gsm->getWorld()->getLayers()->end())
	//{
	//	physics->addCollidableTiles((TiledLayer*)(*tiledLayerIterator));
	//	tiledLayerIterator++;
	//}

	//// LOAD THE LEVEL'S SPRITE IMAGES
	//PoseurSpriteTypesImporter psti;
	//psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	//// LET'S MAKE A PLAYER SPRITE
	//// @TODO - LATER WE'LL LOAD ALL LEVEL DATA FROM A FILE
	//physics->setGravity(W_GRAVITY);
	//SpriteManager *spriteManager = gsm->getSpriteManager();
	//AnimatedSprite *player = new Player();

	//// NOTE THAT PLAYER IS SPRITE ID 2
	//AnimatedSpriteType *playerSpriteType = spriteManager->getSpriteType(2);
	//player->setSpriteType(playerSpriteType);
	//player->setAlpha(255);
	//player->setCurrentState(IDLE);
	//player->setCollisionType(CollisionType::PLAYER);

	//spriteManager->setPlayer(player);

	//physics->addCollidableObject(player, PLAYER_INIT_X, PLAYER_INIT_Y, playerSpriteType->getTextureWidth(), 
	//	playerSpriteType->getTextureHeight(), 20.0f, 0.0f, true, 0, false);
	//player->getPhysicalProperties()->setVelocity(PLAYER_SPEED, 0);

	////-----------Add blocks----------------------------
	//AnimatedSpriteType *blockSpriteType = spriteManager->getSpriteType(3);
	//AnimatedSpriteType *mediumBlockSpriteType = spriteManager->getSpriteType(10);
	//AnimatedSpriteType *longBlockSpriteType = spriteManager->getSpriteType(11);

	//addBlockTower(4500, 0, 5, game, blockSpriteType, false);
	//addBlockTower(4660, 0, 5, game, blockSpriteType, false);

	//addBlockTower(2000,  0, 1, game, mediumBlockSpriteType, false);
	//addBlockTower(2064, 0, 1, game, mediumBlockSpriteType, false);
	//int worldHeight = gsm->getWorld()->getWorldHeight();
	//addBlock(2032, worldHeight - 3*mediumBlockSpriteType->getTextureHeight(), game, mediumBlockSpriteType, true);

	//addBlockTower(2000, 2*mediumBlockSpriteType->getTextureHeight() - 30, 1, game, mediumBlockSpriteType, false);
	//addBlockTower(2064, 2*mediumBlockSpriteType->getTextureHeight() - 30, 1, game, mediumBlockSpriteType, false);
	//addBlock(2032, worldHeight - (4*mediumBlockSpriteType->getTextureHeight() + 66), game, mediumBlockSpriteType, true);

	//addBlockTower(3100, 0, 10, game, blockSpriteType, false);
	////----------------Add goals-----------------------------------
	//AnimatedSpriteType *goalSpriteType = spriteManager->getSpriteType(12);
	//addGoalObject(4596, 700, game, goalSpriteType);

	//addGoalObject(2032, 300, game, goalSpriteType);

	//addGoalObject(3100, 100, game, goalSpriteType);

	//AnimatedSpriteType *botSpriteType = spriteManager->getSpriteType(6);

	////----------------Add bots-----------------------------------
	//makeLuaBot(game, botSpriteType, "data/scripts/testbot.lua", L"LEFT", 600, 100);
	//makeLuaBot(game, spriteManager->getSpriteType(0), "data/scripts/WaveBot.lua", L"LEFT", 800, 100);
	//makeLuaBot(game, spriteManager->getSpriteType(1), "data/scripts/CirclingBot.lua", L"LEFT", 700, 100);
	//makeLuaBot(game, spriteManager->getSpriteType(6), "data/scripts/DiagonalBot.lua", L"LEFT", 900, 100);
	//makeLuaBot(game, spriteManager->getSpriteType(0), "data/scripts/ReverseDiagonalBot.lua", L"LEFT", 1000, 100);

	//spriteManager->reserveProjectiles(100);
}

void WalkaboutDataLoader::addBlockTower(float x, float yOffset, int heightInBlocks, Game *game, AnimatedSpriteType* blockType, bool rotate)
{
	int worldHeight = game->getGSM()->getWorld()->getWorldHeight();
	int blockHeight;
	if(rotate)
	{
		blockHeight = blockType->getTextureWidth();
	}
	else
	{
		blockHeight = blockType->getTextureHeight();
	}

	int offset = worldHeight - (((heightInBlocks + 1)*blockHeight) + 35);

	for(int i = 0; i < heightInBlocks; i++)
	{
		addBlock(x, offset + i*blockHeight + 1 - yOffset, game, blockType,rotate);
	}
}

void WalkaboutDataLoader::addBlock(float x, float y, Game *game, AnimatedSpriteType* blockType, bool rotate)
{
	GameStateManager *gsm = game->getGSM();
	Physics *physics = gsm->getPhysics();
	SpriteManager *spriteManager = gsm->getSpriteManager();

	Block* block = new Block();
	block->setSpriteType(blockType);
	block->setCurrentState(L"IDLE");
	block->setAlpha(255);
	block->setCollisionType(CollisionType::BLOCK);
	spriteManager->addBot(block);
	//physics->addCollidableObject(block,x,y,blockType->getTextureWidth(), blockType->getTextureHeight(), 10.0f, 0.3f,false,1,false);
	if(rotate)
	{
		block->getPhysicalProperties()->setRotation(90);
	}
}

void WalkaboutDataLoader::addGoalObject(float x, float y, Game *game, AnimatedSpriteType* goalType)
{
	//GameStateManager *gsm = game->getGSM();
	//Physics *physics = gsm->getPhysics();

	//AnimatedSprite* goalObject = new AnimatedSprite();
	//SpriteManager *spriteManager = gsm->getSpriteManager();
	//goalObject->setSpriteType(goalType);
	//goalObject->setAlpha(255);
	//goalObject->setCurrentState(L"IDLE");
	//goalObject->setCollisionType(CollisionType::BLOCK);
	//spriteManager->addGoalObject(goalObject);
	////physics->addCollidableObject(goalObject,x,y,goalType->getTextureWidth(), goalType->getTextureHeight(), 10.0f, 0.3f,false,1,false);

	//AnimatedSpriteType *animationSpriteType = spriteManager->getSpriteType(13);
	//AnimatedOverlay* animation = new AnimatedOverlay();
	//animation->setSpriteType(animationSpriteType);
	//animation->setAlpha(255);
	//animation->setCurrentState(L"BOUNCING");
	//animation->followCollidableObject(goalObject);
	//animation->setZ(0);
	//spriteManager->addAnimatedOverlay(animation);
}

void WalkaboutDataLoader::makeLuaBot(Game *game, AnimatedSpriteType *botType, const char* luaScriptFile, wstring initialState, float initX, float initY)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Physics *physics = game->getGSM()->getPhysics();

	LuaBot* bot = new LuaBot(luaScriptFile);
	bot->setCollisionType(CollisionType::BOT);

	bot->setSpriteType(botType);
	bot->setCurrentState(initialState);
	bot->setAlpha(255);
	spriteManager->addBot(bot);
	//physics->addCollidableObject(bot, initX, initY, botType->getTextureWidth(), 
		//botType->getTextureHeight(), 1, 0.3, true, 0, false);
	bot->affixTightAABBBoundingVolume();
}

/*
initWalkaboutGUI - This method builds a GUI for the Walkabout Game application.
Note that we load all the GUI components from this method, including
the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void WalkaboutDataLoader::hardCodedLoadGUIExample(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);

	// SCREENS
	initSplashScreen(game, gui, guiTextureManager);
	initMainMenu(game, gui, guiTextureManager);
	initInGameGUI(gui, guiTextureManager);
	initWinScreen(game, gui, guiTextureManager);
	initLoseScreen(game, gui, guiTextureManager);
	
	initStoryScreen(game, gui, guiTextureManager);
	initCreditsScreen(game, gui, guiTextureManager);
	initLevelChooserScreen(game, gui, guiTextureManager);
	initControlsScreen(game, gui, guiTextureManager);
	initPauseScreen(game, gui, guiTextureManager);
	
}

/*
initCursor - initializes a simple little cursor for the gui.
*/
void WalkaboutDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	//imageID = guiTextureManager->loadTexture(W_GREEN_CURSOR_PATH);
	//imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	//imageID = guiTextureManager->loadTexture(W_RED_CURSOR_PATH);
	//imageIDs->push_back(imageID);

	// LOAD CURSOR IMAGES
	imageID = guiTextureManager->loadTexture(W_CROSSHAIR_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
		*(imageIDs->begin()),
		0,
		0,
		0,
		255,
		32,
		32);
	gui->setCursor(cursor);
}

/*
initSplashScreen - initializes the game's splash screen gui.
*/
void WalkaboutDataLoader::initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();
	unsigned int imageID= guiTextureManager->loadTexture(W_SPLASH_SCREEN_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = game->getGraphics()->getScreenHeight();
	imageToAdd->width = game->getGraphics()->getScreenWidth();
	imageToAdd->imageID = imageID;
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	splashScreenGUI->addOverlayImage(imageToAdd);

	// IT HAS ONE BUTTON WHICH STARTS THE GAME
	Button *buttonToAdd = new Button();
	int normalTextureID = guiTextureManager->loadTexture(W_START_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_START_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		(game->getGraphics()->getScreenWidth() / 2) - 125,
		400,
		0,
		255,
		250,
		100,
		false,
		W_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

/*
initMainMenu - initializes the game's main menu gui.
*/
void WalkaboutDataLoader::initMainMenu(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = game->getGraphics()->getScreenHeight();
	imageToAdd->width = game->getGraphics()->getScreenWidth();
	imageToAdd->imageID = imageID;
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	mainMenuGUI->addOverlayImage(imageToAdd);

	// ADD THE NEW GAME BUTTON
	Button *buttonToAdd = new Button();
	int normalTextureID = guiTextureManager->loadTexture(W_NEW_GAME_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_NEW_GAME_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		317,
		120,
		0,
		255,
		417,
		100,
		false,
		W_LEVEL_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// ADD THE HIGH SCORES BUTTON
	/*buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_HIGH_SCORES_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_HIGH_SCORES_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		317,
		230,
		0,
		255,
		417,
		100,
		false,
		W_EXIT_COMMAND);	// @TODO for now it exits the game
	mainMenuGUI->addButton(buttonToAdd);*/

	// ADD THE STORY BUTTON
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_STORY_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_STORY_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		317,
		230,
		0,
		255,
		417,
		100,
		false,
		W_STORY_1_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// ADD THE CREDITS BUTTON
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_CREDITS_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_CREDITS_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		317,
		340,
		0,
		255,
		417,
		100,
		false,
		W_CREDITS_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// ADD THE EXIT BUTTON
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		317,
		560,
		0,
		255,
		417,
		100,
		false,
		W_PROMPT_COMMAND);
	mainMenuGUI->addButton(buttonToAdd);

	// Initialize the prompt when the user presses the exit game button in the main menu
	initPromptScreen(game, gui, guiTextureManager, W_EXIT_COMMAND, W_GO_TO_MM_COMMAND, GS_MAIN_MENU);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU,		mainMenuGUI);
}

void WalkaboutDataLoader::initWinScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	ScreenGUI *winGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_BASE_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = game->getGraphics()->getScreenHeight();
	imageToAdd->width = game->getGraphics()->getScreenWidth();
	imageToAdd->imageID = imageID;
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	winGUI->addOverlayImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(W_EPIC_WIN_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = 298;
	imageToAdd->width = 468;
	imageToAdd->imageID = imageID;
	imageToAdd->x = (game->getGraphics()->getScreenWidth()/2) - 234;
	imageToAdd->y = 200;
	imageToAdd->z = 0;
	winGUI->addOverlayImage(imageToAdd);


	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);
	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		game->getGraphics()->getScreenWidth()/2 - 208,
		game->getGraphics()->getScreenHeight() - 150,
		0,
		255,
		417,
		100,
		false,
		W_QUIT_COMMAND);
	winGUI->addButton(buttonToAdd);

	gui->addScreenGUI(GS_WIN, winGUI);
}

void WalkaboutDataLoader::initLoseScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	ScreenGUI *winGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_BASE_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = game->getGraphics()->getScreenHeight();
	imageToAdd->width = game->getGraphics()->getScreenWidth();
	imageToAdd->imageID = imageID;
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	winGUI->addOverlayImage(imageToAdd);

	imageID = guiTextureManager->loadTexture(W_LOSE_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = 464;
	imageToAdd->width = 389;
	imageToAdd->imageID = imageID;
	imageToAdd->x = (game->getGraphics()->getScreenWidth()/2) - 194;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	winGUI->addOverlayImage(imageToAdd);


	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);
	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		game->getGraphics()->getScreenWidth()/2 - 208,
		game->getGraphics()->getScreenHeight() - 150,
		0,
		255,
		417,
		100,
		false,
		W_QUIT_COMMAND);
	winGUI->addButton(buttonToAdd);

	gui->addScreenGUI(GS_GAME_OVER, winGUI);
}

/*
initInGameGUI - initializes the game's in-game gui.
*/
void WalkaboutDataLoader::initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	/*unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
	mouseOverTextureID,
	0,
	0,
	0,
	255,
	200,
	100,
	false,
	W_QUIT_COMMAND);
	inGameGUI->addButton(buttonToAdd);*/
	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS,	inGameGUI);
}

void WalkaboutDataLoader::initPauseScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *pauseGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_PAUSE_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = 718;
	imageToAdd->width = 571;
	imageToAdd->imageID = imageID;
	imageToAdd->x = (game->getGraphics()->getScreenWidth()/2) - 285; // 285 = center of image in x-axis
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	pauseGUI->addOverlayImage(imageToAdd);

	// ADD THE RESTART BUTTON
	Button *buttonToAdd = new Button();
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_RESTART_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_RESTART_IMAGE_MO_PATH);

	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		293,
		250,
		0,
		255,
		417,
		100,
		false,
		L"");
	pauseGUI->addButton(buttonToAdd);

	// ADD THE CONTROLS BUTTON
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_CONTROLS_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_CONTROLS_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		293,
		370,
		0,
		255,
		417,
		100,
		false,
		W_CONTROLS_COMMAND);
	pauseGUI->addButton(buttonToAdd);

	// ADD THE BACK TO MAIN MENU BUTTON
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		293,
		490,
		0,
		255,
		417,
		100,
		false,
		W_PROMPT_COMMAND);
	pauseGUI->addButton(buttonToAdd);

	initPromptScreen(game, gui, guiTextureManager, W_QUIT_COMMAND, W_PAUSE_COMMAND, GS_PAUSED);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_PAUSED,	pauseGUI);
}

void WalkaboutDataLoader::initStoryScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	/***********FIRST STORY SCREEN *************/

	ScreenGUI *storyGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_STORY_1_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = game->getGraphics()->getScreenHeight();
	imageToAdd->width = game->getGraphics()->getScreenWidth();
	imageToAdd->imageID = imageID;
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	storyGUI->addOverlayImage(imageToAdd);

	// ADD THE RIGHT ARROW BUTTON
	Button *buttonToAdd = new Button();
	int normalTextureID = guiTextureManager->loadTexture(W_RIGHT_ARROW_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_RIGHT_ARROW_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		800,
		600,
		0,
		255,
		160,
		96,
		false,
		W_STORY_2_COMMAND);
	storyGUI->addButton(buttonToAdd);

	// ADD A DISABLED YELLOW ARROW
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_LEFT_ARROW_DISABLED_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_LEFT_ARROW_DISABLED_IMAGE_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		50,
		600,
		0,
		255,
		160,
		96,
		false,
		L"");
	storyGUI->addButton(buttonToAdd);

	// ADD THE BACK TO MAIN MENU BUTTON
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		293,
		600,
		0,
		255,
		417,
		100,
		false,
		W_QUIT_COMMAND);
	storyGUI->addButton(buttonToAdd);

	gui->addScreenGUI(GS_MENU_STORY_1_MENU, storyGUI);


	/*******SECOND STORY SCREEN*************/

	storyGUI = new ScreenGUI();
	imageID = guiTextureManager->loadTexture(W_STORY_2_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = game->getGraphics()->getScreenHeight();
	imageToAdd->width = game->getGraphics()->getScreenWidth();
	imageToAdd->imageID = imageID;
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	storyGUI->addOverlayImage(imageToAdd);

	// ADD THE DISABLED RIGHT ARROW BUTTON
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_RIGHT_ARROW_DISABLED_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_RIGHT_ARROW_DISABLED_IMAGE_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		800,
		600,
		0,
		255,
		160,
		96,
		false,
		L"");
	storyGUI->addButton(buttonToAdd);

	// ADD A YELLOW ARROW
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_LEFT_ARROW_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_LEFT_ARROW_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		50,
		600,
		0,
		255,
		160,
		96,
		false,
		W_STORY_1_COMMAND);
	storyGUI->addButton(buttonToAdd);

	// ADD THE BACK TO MAIN MENU BUTTON
	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		293,
		600,
		0,
		255,
		417,
		100,
		false,
		W_QUIT_COMMAND);
	storyGUI->addButton(buttonToAdd);

	gui->addScreenGUI(GS_MENU_STORY_2_MENU, storyGUI);
}

void WalkaboutDataLoader::initCreditsScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	ScreenGUI *creditsGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_CREDITS_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = game->getGraphics()->getScreenHeight();
	imageToAdd->width = game->getGraphics()->getScreenWidth();
	imageToAdd->imageID = imageID;
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	creditsGUI->addOverlayImage(imageToAdd);

	// ADD THE BACK TO MENU BUTTON
	Button *buttonToAdd = new Button();
	int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		400,
		600,
		0,
		255,
		417,
		100,
		false,
		W_QUIT_COMMAND);
	creditsGUI->addButton(buttonToAdd);

	// Add the screen to our gui
	gui->addScreenGUI(GS_CREDITS, creditsGUI);
}

void WalkaboutDataLoader::initLevelChooserScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	ScreenGUI *levelMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_MAIN_MENU_PATH);	// We're just using the main menu overlay here
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = game->getGraphics()->getScreenHeight();
	imageToAdd->width = game->getGraphics()->getScreenWidth();
	imageToAdd->imageID = imageID;
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	levelMenuGUI->addOverlayImage(imageToAdd);

	// ADD THE LEVEL BUTTONS
	Button *buttonToAdd = new Button();
	int normalTextureID = guiTextureManager->loadTexture(W_LEVEL_1_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_LEVEL_1_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		317,
		230,
		0,
		255,
		417,
		100,
		false,
		W_LEVEL_1_COMMAND);
	levelMenuGUI->addButton(buttonToAdd);

	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_LEVEL_2_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_LEVEL_2_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		317,
		340,
		0,
		255,
		417,
		100,
		false,
		W_LEVEL_2_COMMAND);
	levelMenuGUI->addButton(buttonToAdd);

	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_LEVEL_3_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_LEVEL_3_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		317,
		450,
		0,
		255,
		417,
		100,
		false,
		W_LEVEL_3_COMMAND);
	levelMenuGUI->addButton(buttonToAdd);

	gui->addScreenGUI(GS_LEVEL_MENU, levelMenuGUI);
}

void WalkaboutDataLoader::initControlsScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *controlsGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_CONTROLS_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = 480;
	imageToAdd->width = 704;
	imageToAdd->imageID = imageID;
	imageToAdd->x = (game->getGraphics()->getScreenWidth()/2) - 352; // 352 = center of image in x-axis
	imageToAdd->y = (game->getGraphics()->getScreenHeight()/2) - 240; // 240 = center of image in y-axis
	imageToAdd->z = 0;
	controlsGUI->addOverlayImage(imageToAdd);

	// ADD A BACK BUTTON
	Button *buttonToAdd = new Button();
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_LEFT_ARROW_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_LEFT_ARROW_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		630,
		460,
		0,
		255,
		160,
		96,
		false,
		W_PAUSE_COMMAND);
	controlsGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_IN_GAME_CONTROLS,	controlsGUI);
}

void WalkaboutDataLoader::initPromptScreen(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager, wstring yesCommand, wstring noCommand, GameState callingState)
{
	ScreenGUI *promptGUI = new ScreenGUI();

	if (callingState == GS_MAIN_MENU)
	{
		unsigned int imageID = guiTextureManager->loadTexture(W_BASE_PATH);
		OverlayImage *imageToAdd = new OverlayImage();
		imageToAdd->alpha = 255;
		imageToAdd->height = game->getGraphics()->getScreenHeight();
		imageToAdd->width = game->getGraphics()->getScreenWidth();
		imageToAdd->imageID = imageID;
		imageToAdd->x = 0;
		imageToAdd->y = 0;
		imageToAdd->z = 0;
		promptGUI->addOverlayImage(imageToAdd);
	}
	
	unsigned int imageID = guiTextureManager->loadTexture(W_PROMPT_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->height = 192;
	imageToAdd->width = 384;
	imageToAdd->imageID = imageID;
	imageToAdd->x = (game->getGraphics()->getScreenWidth()/2) - 192; // 192 = center of image in x-axis
	imageToAdd->y = (game->getGraphics()->getScreenHeight()/2) - 96; // 96 = center of image in y-axis
	imageToAdd->z = 0;
	promptGUI->addOverlayImage(imageToAdd);

	// ADD THE YES-NO BUTTONS
	Button *buttonToAdd = new Button();
	int normalTextureID = guiTextureManager->loadTexture(W_YES_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_YES_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		400,
		375,
		0,
		255,
		96,
		64,
		false,
		yesCommand);
	promptGUI->addButton(buttonToAdd);

	buttonToAdd = new Button();
	normalTextureID = guiTextureManager->loadTexture(W_NO_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_NO_IMAGE_MO_PATH);
	buttonToAdd->initButton(normalTextureID, 
		mouseOverTextureID,
		525,
		375,
		0,
		255,
		96,
		64,
		false,
		noCommand);
	promptGUI->addButton(buttonToAdd);

	if (callingState == GS_MAIN_MENU)
		gui->addScreenGUI(GS_PROMPT, promptGUI);
	else if (callingState == GS_PAUSED)
		gui->addScreenGUI(GS_IN_GAME_PROMPT, promptGUI);
}

/*
initViewport - initializes the game's viewport.
*/
void WalkaboutDataLoader::initViewport(GameGUI *gui, map<wstring,wstring> *properties)
{
	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[W_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[W_VIEWPORT_OFFSET_Y];
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(viewportOffsetY);
}