#ifndef ESCAPE_FROM_THE_ISLAND
#define ESCAPE_FROM_THE_ISLAND

//-----<GAME PROJECT MASTER HEADER>-----
//Current game version: 1.2.2.6

//First playable version:
//The runtime is stable - working menu + all 3 lvls, but they can be just passed by walking straight

//Unsolved:
//The code is still very messy!
//Theres some yet unsolved issues!
//The code will hopefully get clean and in order by me soon!
//Some other config and asset file rearrangements might be done too!
//Also the number of headers might reduce!
//Also new code content and comments in the code may appear!
//Better error handle and runtime log should be made!
//Better data structs + split texture and entity hitbox into two!

//Optional:
//Accelerate rendering on GPU!

//Standard C++ lib - some yet unused
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <thread> //Threads may be used later when it will be actually needed! - now just for thread sleep
#if 0
#include <atomic>
#include <mutex>
#include <semaphore>
#endif
#include <fstream>
#include <filesystem>
#include <sstream>

//SDL3 lib + SDL3_image lib
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

//Allowing functions to be inlined only if the build is 'Release'
#if _DEBUG == 1
#define INLINE
#elif NDEBUG == 1
#define INLINE inline
#endif

//Too much headers it looks like!
//Other internal headers with definitions that are important for this program
#include "WRCluster.hpp"
#include "TCluster.hpp"
#include "hitbox.hpp"
#include "entity.hpp"
#include "ECluster.hpp"
#include "user_events.hpp"
#include "game_env.hpp"
#include "config_file_content_info.hpp"
#include "error_messages_codes.hpp"

//Allowing usage of std literal operators for strings and time
using namespace std::string_literals;
using namespace std::chrono_literals;

//All types of messages that can be logged
enum LogTypes : uint64_t
{
	INFO	= 0,
	WARNING = 1,
	ERROR	= 2,
	CRASH	= 3
};

//Names for the indexes for all game renderers defined in 'WRCluster' in 'WinMain' function
enum GameRenderersNames : uint64_t
{
	TEXTURE_RENDERER = 0
};

//Names of all types of entities that are present in this game - some have may have textures from the same db file
enum EntitiesNames : uint64_t
{
	PLAYER				= 0,
	BACKGROUND			= 1,
	BACKGROUND_HITBOX	= 2,
	MENU_PANEL			= 3,
	MENU_BUTTONS		= 4,
	FLOATING_STONE		= 5,
	SPIKE				= 6,
	CRATE_1				= 7,
	CRATE_2				= 8,
	CRATE_3				= 9,
	CRATE_4				= 10
};

//Names for the indexes of the subclusters of PlayerTextureCluster 
enum PlayerTextureSubclustersIndexes : uint64_t
{
	MOVING_LEFT		= 0,
	MOVING_RIGHT	= 1,
	STANDING_LEFT	= 2,
	STANDING_RIGHT	= 3
};

//
enum BackgroundTextureSubclustersIndexes : uint64_t
{
	BACKGROUND_LEVEL_1 = 0,
	BACKGROUND_LEVEL_2 = 1,
	BACKGROUND_ISLAND = 2
};

//
enum MenuPanelTextureSubclustersIndexes : uint64_t
{
	PANEL = 0
};

//
enum MenuButtonsTextureSubclustersIndexes : uint64_t
{
	BUTTONS = 1
};

//
enum MenuButtonsTextureSubindexes : uint64_t
{
	PLAY_LEVELS_QUIT = 0,
	HIGHLIGHTED_PLAY = 1,
	HIGHLIGHTED_LEVELS = 2,
	HIGHLIGHTED_QUIT = 3,
};

//
enum ObstaclesTextureSubclustersIndexes : uint64_t
{
	TEXTURE_FLOATING_STONE = 0,
	TEXTURE_SPIKE = 1,
	TEXTURE_CRATE_1 = 2,
	TEXTURE_CRATE_2 = 3
};

//
enum OtherHitboxesIndexes : uint64_t
{
	LEVEL_FLOOR = 0,
	MOUNTAIN = 1,
	ENTRANCE_GATE = 2,
	EXIT_GATE = 3,
	BOAT_RUDDER = 4
};

//problems with enum collisions!

namespace WindowRenderHandle //[start]
{
//Initialize a specified amount of renderers for a specified window
void CreateNewRenderers(WRCluster& _WindowAndRenderers, const uint64_t _CountOfRenderers);
//Initialize graphical window and a specified amount of renderers [default amount of renderers is 1]
void CreateNewWindowWithRenderers(WRCluster& _WindowAndRenderers, const uint64_t _CountOfRenderers = 1);
//Destroys specified renderers
void DestroyRenderers(WRCluster& _WindowAndRenderers);
//Destroys graphical window and renderers
void DestroyWindowWithRenderers(WRCluster& _WindowAndRenderers);
//Sets the color that will be used that the beginning of every frame to black - should be called only once at the beginning of program
void SetFrameDefaultColorToBlack(WRCluster& _WindowAndRenderers, const uint64_t _IndexOfFrameRenderer);
}
//WindowRenderHandle [end]

namespace TextureHandle //[start]
{
//Loads and prepares of the textures from image files based on the filenames specified in a db file and makes a 2D TCluster from them
TCluster LoadFromFiles(SDL_Renderer* const _TextureRenderer, const std::string& _TexturesDB_Filename);
//Function that loas the texture filenames from db files and call the function that loads them - this function may be removed soon!
void PrepareForAllEntities(WRCluster& _MainWindow, ECluster& _AllEntities);
//Function that safely removes from selected cluster
void SafelyRemoveTextureFromCluster(TCluster& _TextureCluster, const uint64_t _Index, const uint64_t _Subindex);
}
//TextureHandle [end]

namespace EntityBulkInitHandle //[start]
{
//Prepares all entites and puts them into the specified ECluster based on the config in the "config.ini" file
void PrepareECluster(ECluster& _Entities);
}
//EntityBulkInitHandle [end]

namespace GameConditions //[start]
{
//
void LocationChange(user_events& _AllUserEvents, game_env& _GameState);
//
void MenuButtonHighlightAndTrigger(entity& _MenuButtons, user_events& _AllUserEvents, game_env& _GameState);
//
void PlayerMovement(entity& _Player, user_events& _AllUserEvents);
//
void PlayerJump(entity& _Player, user_events& _AllUserEvents, game_env& _GameState);
//
void PlayerCollisions(ECluster& _AllEntities, HCluster& _AllOtherHitboxes);
//
void FloatingStoneMovement(entity& _FloatingStone);
//
void FloatingStonePlayerInteraction(ECluster& _AllEntities);
//
void SpikeAndRudderPlayerInteration(ECluster& _AllEntities, game_env& _GameState, HCluster& _AllOtherHitboxes);
//
void LevelChange(ECluster& _AllEntities, game_env& _GameState, HCluster& _AllOtherHitboxes);
}
//GameConditions [end]

namespace GameLoop //[start]
{
//Loop for processing user events and rendering the game - runs on 'Main thread'
void MainLoop(WRCluster& _MainWindow, ECluster& _AllEntities, user_events& _AllUserEvents, game_env& _GameState);
}
//GameLoop [end]

namespace ConfigFile //[start]
{
//Finds the value based on specific Bundle and Selector and extracts it as Number type
void ReadValue(const std::string& _Bundle, const std::string& _Selector, int64_t& _ExtractedValue);
//Finds the value based on specific Bundle and Selector and extracts it as Bool type
void ReadValue(const std::string& _Bundle, const std::string& _Selector, bool& _ExtractedValue);
//Finds the value based on specific Bundle and Selector and extracts it as Text type
void ReadValue(const std::string& _Bundle, const std::string& _Selector, std::string& _ExtractedValue);
//Finds the old value based on specific Bundle and Selector and changes it to the new Number type value
void UpdateValue(const std::string& _Bundle, const std::string& _Selector, const int64_t& _NewValue);
//Finds the old value based on specific Bundle and Selector and changes it to the new Bool type value
void UpdateValue(const std::string& _Bundle, const std::string& _Selector, const bool& _NewValue);
//Finds the old value based on specific Bundle and Selector and changes it to the new Text type value
void UpdateValue(const std::string& _Bundle, const std::string& _Selector, const std::string& _NewValue);
}
//ConfigFile [end]

namespace ErrorHandle //[start]
{
//Place an error related to specified code and terminate process eventually
void Report(const std::string& _ErrorMessage, const uint64_t _ErrorCode, const bool _TerminateProcess);
//Place an error related to SDL-lib and terminate process eventually
void ReportSDL(const bool _TerminateProcess);
}
//ErrorHandle [end]

namespace RuntimeLog //[start]
{
//Creates a new file for log messages - needs to be called first
void CreateFile(void);
//Appends a new message to already existing log file
void Message(const LogTypes _Type, const std::string& _Message);
//Makes one message out of any amount of other messages separated by the '=>' arrow then it puts it in the log file as one message - usage without existing log file may be unsafe
void MultiMessage(const LogTypes _Type, const std::initializer_list<const std::string> _MultipleMessages);
}
//RuntimeLog [end]

#endif //ESCAPE_FROM_THE_ISLAND