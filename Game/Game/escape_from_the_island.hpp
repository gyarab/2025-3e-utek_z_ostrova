#ifndef ESCAPE_FROM_THE_ISLAND
#define ESCAPE_FROM_THE_ISLAND

//current game version: 1.0.0.0

//Accelerate rendering on GPU!
//Add better app/game icon!

//standard C++ lib - some yet unused
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <thread>
#include <atomic>
#include <mutex>
#include <fstream>
#include <filesystem>
#include <sstream>

//SDL3 lib + SDL3_image lib
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

//
#include "config_file_content_info.hpp"

//Allowing functions to be inlined only if the build is 'Release'
#if _DEBUG == 1
#define INLINE
#elif NDEBUG == 1
#define INLINE inline
#endif

//Allowing usage of std literal operators for strings and time
using namespace std::string_literals;
using namespace std::chrono_literals;

//Make it more dynamic!
//Default values for player's textures on screen
#define ENTITY_PLAYER_DEFAULT_SIZE 32.0f
#define ENTITY_PLAYER_DEFAULT_SCREEN_POSITION 100.0f

//All types of messages that can be logged
enum LogTypes : uint64_t
{
	INFO = 0,
	WARNING = 1,
	ERROR = 2,
	CRASH = 3
};

//Names for the indexes for all game renderers defined in 'RCluster' in 'WinMain' function
enum GameRenderersNames : uint64_t
{
	TEXTURE_RENDERER = 0
};

//Names for the indexes of 'PlayerAnimationTClusters' array defined in 'WinMain' function
enum PlayerAnimationTClustersIndexes : uint64_t
{
	RUNNING_FACING_LEFT = 0,
	RUNNING_FACING_RIGHT = 1,
	STANDING_FACING_LEFT = 2,
	STANDING_FACING_RIGHT = 3
};

//Add pointer safety!
//Dynamic cluster that should contain textures that are related to each other
typedef struct ClusterOfSimilarTextures
{
	std::vector<SDL_Texture*> _Textures;
}
TCluster;

//Add pointer safety!
//Dynamic cluster that should contain renderers that are related to each other
typedef struct ClusterOfSimilarRenderers
{
	std::vector<SDL_Renderer*> _Renderers;
}
RCluster;

//All possible direction that entity can face
enum Directions : uint64_t
{
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3
};

//Container defining a state of an entity
typedef struct EntityCurrentStatus
{
	Directions _Facing;
	bool _IsRunning;
	SDL_FRect _PositionAndSize;
}
Entity;

namespace WindowRenderHandle //[start]
{
//Initialize a specified amount of renderers for a specified window
void CreateNewRenderers(SDL_Window*& _Window, RCluster& _Renderers, const uint64_t _CountOfRenderers);
//Initialize graphical window and a specified amount of renderers
INLINE void CreateNewWindowWithRenderers(SDL_Window*& _Window, RCluster& _Renderers, const uint64_t _CountOfRenderers);
//Destroys specified renderers
void DestroyRenderers(SDL_Window*& _Window, RCluster& _Renderers);
//Destroys graphical window and renderers
INLINE void DestroyWindowWithRenderers(SDL_Window*& _Window, RCluster& _Renderers);
//Sets the color that will be used that the beginning of every frame to black - should be called only once at the beginning of program
void SetFrameDefaultColorToBlack(SDL_Renderer* const _FrameRenderer);
}
//WindowRenderHandle [end]

namespace GameLoopThread //[start]
{
//Function that takes an user event and transforms it into a player status data and animation
void AssignPlayerStatusAndAnimationToUserEvent(const SDL_Event& _UserEvent, Entity& _Player, TCluster*& _TextureToAnimate, std::array<TCluster, 4>& _PlayerAnimationTClusters, std::mutex& _MutexForMainThread);
//Loop for processing user events and rendering the game - runs on 'Main thread'
void MainLoop(SDL_Renderer* const _TextureRenderer, const uint64_t _ScalingCoeficient, std::array<TCluster, 4>& _PlayerAnimationTClusters);
}
//GameLoopThread [end]

namespace PlayerThread //[start]
{
//Function used to initialize Entity container with 'default' values - should be equal to '_ScalingCoefficient'
Entity PutDefaultValuesForPlayer(const uint64_t _ScalingCoefficient = 1);
//Thread that animates a set cluster of textures by continuously selecting them in order [first->last] with delay
void AnimatePlayerTextureClusterThreadMain(SDL_Texture** _DisplayedTexture, TCluster** const _TexturesToAnimate, const uint64_t _TextureUpdateDelay, std::atomic_bool* const _AnimationInterrupted, std::atomic_bool* const _ThreadShouldFinish);
//Thread that constantly updates the screen with an animation of player's character + it can also change the player's coords
void FrameRenderThreadMain(SDL_Renderer* const _TextureRenderer, Entity* const _Player, SDL_Texture** const _TextureToRender, std::atomic_bool* const _ThreadShouldFinish);
}
//PlayerThread [end]

namespace TextureHandle //[start]
{
//Extracts PNG into surface then [optionally] scales it by a coefficient and then converts it to a render-able texture
SDL_Texture* MakeScaledTextureFromPNG(SDL_Renderer* const _TextureRenderer, const std::string& _Filename, const uint64_t _ScalingCoefficient = 1);
//Function that safely removes from selected cluster
void SafelyRemoveTextureFromCluster(TCluster& _TextureCluster, const uint64_t _Index);
//Fills the four TClusters with textures from .png files
void PrepareAllPlayerAnimationTextures(SDL_Renderer* const _TextureRenderer, std::array<TCluster, 4>& _PlayerAnimationTClusters, const uint64_t _ScalingCoeficient);
}
//TextureHandle [end]

namespace ErrorHandle //[start]
{
//Place an error related to specified code and terminate process eventually
void Report(const std::string& _ErrorMessage, const uint64_t _ErrorCode, const bool _TerminateProcess);
//Place an error related to SDL-lib and terminate process eventually
void ReportSDL(const bool _TerminateProcess);
}
//ErrorHandle [end]

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

namespace RuntimeLog //[start]
{
//Creates a new file for log messages - needs to be called first
void CreateFile(void);
//Appends a new message to already existing log file
void Message(const LogTypes _Type, const std::string& _Message);
}
//RuntimeLog [end]

#endif //ESCAPE_FROM_THE_ISLAND