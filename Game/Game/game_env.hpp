#ifndef GAME_ENV
#define GAME_ENV

//
#include "escape_from_the_island.hpp"

//
enum GameLocationNames : uint64_t
{
	MAIN_MENU	= 0,
	LEVEL_1		= 1,
	LEVEL_2		= 2,
	ISLAND		= 3
};

//
struct game_env
{
	//
	//Load from config file!
	uint64_t _TickCount = 20;
	std::chrono::milliseconds _TickDelay = (std::chrono::milliseconds)50;
	uint64_t _Tick = 1;

	//
	GameLocationNames _CurrentLocation = (GameLocationNames)NULL;
	//
	bool _LocationIsReady = false;

	//
	bool _Level_1_Completed = false;
	//
	bool _Level_2_Completed = false;
	
	//
	bool _F1_KeyLock = false;
	//
	bool _SPACE_KeyLock = false;

	//
	bool _PlayButtonPressed = false;
	//
	bool _QuitButtonPressed = false;

public:
	//
	void update_tick(void);
	//
	void set_location(const GameLocationNames _CurrentLocation);
	//
	void prepare_location(ECluster& _AllEntities, HCluster& _AllOtherHitboxes);
};

#endif