#pragma once
#include "stdafx.h"

/*
	WalkaboutGame.h

	This file contains setup data for making the Walkabout game. Note that nearly
	all of this data would better serve us if loaded from files.
*/

// HERE IS THE Walkabout GAME SETUP/INIT FILE. EVERYTHING WOULD START FROM HERE
static const wstring	W_LEVEL_DIR				= L"data/levels/";
//static const wstring	W_LEVEL_NAME			= L"NightRun.oel";

static const wstring	W_LEVEL_1_DIR			= L"";
static const wstring	W_LEVEL_1_NAME			= L"Tutorial.oel"; // Dinia's Level
static const wstring	W_LEVEL_1_PATH			= W_LEVEL_1_DIR + W_LEVEL_1_NAME;

static const wstring	W_LEVEL_2_DIR			= L"";
static const wstring	W_LEVEL_2_NAME			= L"NightRun.oel";	//Nick's Level
static const wstring	W_LEVEL_2_PATH			= W_LEVEL_2_DIR + W_LEVEL_2_NAME;

static const wstring	W_LEVEL_3_DIR			= L"";
static const wstring	W_LEVEL_3_NAME			= L"TestLevel.oel"; // Andrew's Level
static const wstring	W_LEVEL_3_PATH			= W_LEVEL_3_DIR + W_LEVEL_3_NAME;

static const wstring	W_INIT_FILE				= L"data/KamikazeFlyInit.txt";
static const wstring	W_GUI_INIT_FILE			= L"data/gui/KamikazeFlyGUI.txt";
static const string		W_DEBUG_FILE			= "DebugOutput.txt";

// HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
static const wstring	W_TITLE					= L"TITLE";
static const wstring	W_SCREEN_WIDTH			= L"SCREEN_WIDTH";
static const wstring	W_SCREEN_HEIGHT			= L"SCREEN_HEIGHT";
static const wstring	W_USE_FULLSCREEN_MODE	= L"USE_FULLSCREEN_MODE";
static const wstring	W_TEXT_FONT_SIZE		= L"TEXT_FONT_SIZE";
static const wstring	W_VIEWPORT_OFFSET_X		= L"VIEWPORT_OFFSET_X";
static const wstring	W_VIEWPORT_OFFSET_Y		= L"VIEWPORT_OFFSET_Y";
static const wstring	W_FONT_COLOR_RED		= L"FONT_COLOR_RED";
static const wstring	W_FONT_COLOR_GREEN		= L"FONT_COLOR_GREEN";
static const wstring	W_FONT_COLOR_BLUE		= L"FONT_COLOR_BLUE";
static const wstring	W_COLOR_KEY_RED			= L"COLOR_KEY_RED";
static const wstring	W_COLOR_KEY_GREEN		= L"COLOR_KEY_GREEN";
static const wstring	W_COLOR_KEY_BLUE		= L"COLOR_KEY_BLUE";

// DIRECTORIES THAT WE MIGHT NEED
static const wstring	W_OVERLAYS_DIR			= L"data/gui/overlays/";
static const wstring	W_BUTTONS_DIR			= L"data/gui/buttons/";
static const wstring	W_CURSOR_DIR			= L"data/gui/cursor/";

// BUTTON COMMANDS
static const wstring	W_GO_TO_MM_COMMAND		= L"Go To Main Menu";
static const wstring	W_EXIT_COMMAND			= L"Exit";
static const wstring	W_QUIT_COMMAND			= L"Quit";
static const wstring	W_STORY_1_COMMAND		= L"Story 1";
static const wstring	W_STORY_2_COMMAND		= L"Story 2";
static const wstring	W_CREDITS_COMMAND		= L"Credits";
static const wstring	W_CONTROLS_COMMAND		= L"Controls";
static const wstring	W_LEVEL_COMMAND			= L"Level";
static const wstring	W_LEVEL_1_COMMAND		= L"Level 1";
static const wstring	W_LEVEL_2_COMMAND		= L"Level 2";
static const wstring	W_LEVEL_3_COMMAND		= L"Level 3";
static const wstring	W_PAUSE_COMMAND			= L"Pause";
static const wstring	W_PROMPT_COMMAND		= L"Prompt";

// CONSTANTS FOR ARTWORK - NOTE, THIS SHOULD REALLY BE LOADED FROM A FILE
static const wstring	W_CROSSHAIR_CURSOR_PATH	= W_CURSOR_DIR + L"crosshair_cursor.png";
static const wstring	W_BULLET_PATH			= L"data/gui/overlays/projectile.png";

static const wstring	W_SPLASH_SCREEN_PATH	= W_OVERLAYS_DIR + L"kamikaze_fly_splash_screen_overlay.png";
static const wstring	W_MAIN_MENU_PATH		= W_OVERLAYS_DIR + L"kamikaze_fly_main_menu_overlay.png";
static const wstring	W_EPIC_WIN_PATH			= W_OVERLAYS_DIR + L"EpicWin.jpg";
static const wstring	W_LOSE_PATH				= W_OVERLAYS_DIR + L"you-lose.jpg";
static const wstring	W_PAUSE_PATH			= W_OVERLAYS_DIR + L"kamikaze_fly_game_menu_overlay.png";
static const wstring	W_STORY_1_PATH			= W_OVERLAYS_DIR + L"kamikaze_fly_story_overlay_1.png";
static const wstring	W_STORY_2_PATH			= W_OVERLAYS_DIR + L"kamikaze_fly_story_overlay_2.png";
static const wstring	W_CREDITS_PATH			= W_OVERLAYS_DIR + L"kamikaze_fly_credits_overlay.png";
static const wstring	W_CONTROLS_PATH			= W_OVERLAYS_DIR + L"kamikaze_fly_controls_overlay.png";
static const wstring	W_LIFE_PATH				= W_OVERLAYS_DIR + L"life.png";
static const wstring	W_HEART_PATH			= W_OVERLAYS_DIR + L"heart.png";
static const wstring	W_PROMPT_PATH			= W_OVERLAYS_DIR + L"yes_no_prompt_overlay.png";
static const wstring	W_BASE_PATH				= W_OVERLAYS_DIR + L"kamikaze_fly_base_overlay.png";

static const wstring	W_START_IMAGE_PATH					= W_BUTTONS_DIR + L"start_game.png";
static const wstring	W_START_IMAGE_MO_PATH				= W_BUTTONS_DIR + L"start_game_mo.png";
static const wstring	W_NEW_GAME_IMAGE_PATH				= W_BUTTONS_DIR + L"new_game.png";
static const wstring	W_NEW_GAME_IMAGE_MO_PATH			= W_BUTTONS_DIR + L"new_game_mo.png";
//static const wstring	W_HIGH_SCORES_IMAGE_PATH			= W_BUTTONS_DIR + L"high_score.png";
//static const wstring	W_HIGH_SCORES_IMAGE_MO_PATH			= W_BUTTONS_DIR + L"high_score_mo.png";
static const wstring	W_STORY_IMAGE_PATH					= W_BUTTONS_DIR + L"story.png";
static const wstring	W_STORY_IMAGE_MO_PATH				= W_BUTTONS_DIR + L"story_mo.png";
static const wstring	W_CREDITS_IMAGE_PATH				= W_BUTTONS_DIR + L"credits.png";
static const wstring	W_CREDITS_IMAGE_MO_PATH				= W_BUTTONS_DIR + L"credits_mo.png";
static const wstring	W_EXIT_IMAGE_PATH					= W_BUTTONS_DIR + L"exit_game.png";
static const wstring	W_EXIT_IMAGE_MO_PATH				= W_BUTTONS_DIR + L"exit_game_mo.png";
static const wstring	W_QUIT_IMAGE_PATH					= W_BUTTONS_DIR + L"quit.png";
static const wstring	W_QUIT_IMAGE_MO_PATH				= W_BUTTONS_DIR + L"quit_mo.png";
static const wstring	W_RESTART_IMAGE_PATH				= W_BUTTONS_DIR + L"restart.png";
static const wstring	W_RESTART_IMAGE_MO_PATH				= W_BUTTONS_DIR + L"restart_mo.png";
static const wstring	W_CONTROLS_IMAGE_PATH				= W_BUTTONS_DIR + L"controls.png";
static const wstring	W_CONTROLS_IMAGE_MO_PATH			= W_BUTTONS_DIR + L"controls_mo.png";
static const wstring	W_RIGHT_ARROW_IMAGE_PATH			= W_BUTTONS_DIR + L"right_arrow.png";
static const wstring	W_RIGHT_ARROW_IMAGE_MO_PATH			= W_BUTTONS_DIR + L"right_arrow_mo.png";
static const wstring	W_LEFT_ARROW_IMAGE_PATH				= W_BUTTONS_DIR + L"left_arrow.png";
static const wstring	W_LEFT_ARROW_IMAGE_MO_PATH			= W_BUTTONS_DIR + L"left_arrow_mo.png";
static const wstring	W_LEFT_ARROW_DISABLED_IMAGE_PATH	= W_BUTTONS_DIR + L"left_arrow_disabled.png";
static const wstring	W_RIGHT_ARROW_DISABLED_IMAGE_PATH	= W_BUTTONS_DIR + L"right_arrow_disabled.png";
static const wstring	W_LEVEL_1_IMAGE_PATH				= W_BUTTONS_DIR + L"level_1.png";
static const wstring	W_LEVEL_1_IMAGE_MO_PATH				= W_BUTTONS_DIR + L"level_1_mo.png";
static const wstring	W_LEVEL_2_IMAGE_PATH				= W_BUTTONS_DIR + L"level_2.png";
static const wstring	W_LEVEL_2_IMAGE_MO_PATH				= W_BUTTONS_DIR + L"level_2_mo.png";
static const wstring	W_LEVEL_3_IMAGE_PATH				= W_BUTTONS_DIR + L"level_3.png";
static const wstring	W_LEVEL_3_IMAGE_MO_PATH				= W_BUTTONS_DIR + L"level_3_mo.png";
static const wstring	W_BACK_IMAGE_PATH					= W_BUTTONS_DIR + L"back.png";
static const wstring	W_BACK_IMAGE_MO_PATH				= W_BUTTONS_DIR + L"back_mo.png";
static const wstring	W_YES_IMAGE_PATH					= W_BUTTONS_DIR + L"yes.png";
static const wstring	W_YES_IMAGE_MO_PATH					= W_BUTTONS_DIR + L"yes_mo.png";
static const wstring	W_NO_IMAGE_PATH						= W_BUTTONS_DIR + L"no.png";
static const wstring	W_NO_IMAGE_MO_PATH					= W_BUTTONS_DIR + L"no_mo.png";

// Audio file. Using char* here to make it simpler to create a Sound using fmod (fmod likes char*).
static const char*		C_BEE_AUDIO_PATH			= "data/audio/bee.mp3";
static const char*		C_BLAST_LG_AUDIO_PATH		= "data/audio/blast_large.mp3";
static const char*		C_BLAST_MD_AUDIO_PATH		= "data/audio/blast_medium.mp3";
static const char*		C_BLAST_SM_AUDIO_PATH		= "data/audio/blast_small.mp3";
static const char*		C_BUTTERFLY_AUDIO_PATH		= "data/audio/butterfly.mp3";
static const char*		C_BUTTON_CLICK_AUDIO_PATH	= "data/audio/button_click.wav";
static const char*		C_FLY_LONG_AUDIO_PATH		= "data/audio/fly_long.mp3";
static const char*		C_FLY_SHORT_AUDIO_PATH		= "data/audio/fly_short.mp3";
static const char*		C_LEVEL_1_AUDIO_PATH		= "data/audio/level1.mp3";
static const char*		C_LEVEL_2_AUDIO_PATH		= "data/audio/level2.mp3";
static const char*		C_LEVEL_3_AUDIO_PATH		= "data/audio/level3.mp3";
static const char*		C_MAIN_AUDIO_PATH			= "data/audio/main.mp3";
static const char*		C_MOSQUITO_AUDIO_PATH		= "data/audio/mosquito.mp3";
static const char*		C_VENUS_AUDIO_PATH			= "data/audio/venus.mp3";

// FOR OUR SPRITE
static const wstring	SPRITE_TYPES_DIR = L"data/sprite_types/";
static const wstring	SPRITE_TYPES_LIST = SPRITE_TYPES_DIR + L"sprite_type_list.xml";
static const wstring	JUMPING = L"JUMPING";

// FOR THE GAME WORLD
static const float		MAX_VIEWPORT_AXIS_VELOCITY = 20.0f;
static const float		W_GRAVITY = 0.8f;
static const float		JUMPING_BOT_MIN_CYCLES = 30;
static const float		JUMPING_BOT_MAX_CYCLES = 120;
static const float		JUMPING_BOT_MAX_VELOCITY = 40;
static const float		PLAYER_INIT_X = 80.0f;
static const float		PLAYER_INIT_Y = 100.0f;
static const float		JUMP_SPEED = -24.0f;
static const float		PLAYER_SPEED = 10.0f;
static const int		MIN_FPS = 5;
static const int		MAX_FPS = 100;
static const int		FPS_INC = 1;
static const wstring	IDLE(L"IDLE");

// USED FOR MOVEMENT
const unsigned int W_KEY = (unsigned int)'W';
const unsigned int A_KEY = (unsigned int)'A';
const unsigned int S_KEY = (unsigned int)'S';
const unsigned int D_KEY = (unsigned int)'D';
//const unsigned int P_KEY = (unsigned int)'P';
//const unsigned int T_KEY = (unsigned int)'T';
//const unsigned int UP_KEY = VK_UP;
//const unsigned int DOWN_KEY = VK_DOWN;
//const unsigned int LEFT_KEY = VK_LEFT;
//const unsigned int RIGHT_KEY = VK_RIGHT;
const unsigned int SPACE_KEY = VK_SPACE;
const unsigned int ESC_KEY = VK_ESCAPE;

// THIS IS JUST FOR SHOWING HOW THE CURSOR CAN BE CHANGED
const unsigned int C_KEY = (unsigned int)'C';