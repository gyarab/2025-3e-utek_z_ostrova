#include "escape_from_the_island.hpp"

//Function that takes an user event and transforms it into a player status data and animation
static void InterpretUserEvent(const SDL_Event& _UserEvent, Entity& _Player, TCluster*& _TextureToAnimate, TCluster_2D& _PlayerAnimationTClusters, std::mutex& _MutexForMainThread)
{
	switch (_UserEvent.type)
	{
	case SDL_EVENT_KEY_DOWN:
		switch (_UserEvent.key.scancode)
		{
		case SDL_SCANCODE_A:
			_Player._IsRunning = true;
			_Player._Facing = LEFT;
			_MutexForMainThread.lock();
			_TextureToAnimate = &_PlayerAnimationTClusters._Textures[RUNNING_FACING_LEFT];
			_MutexForMainThread.unlock();

			break;

		case SDL_SCANCODE_D:
			_Player._IsRunning = true;
			_Player._Facing = RIGHT;
			_MutexForMainThread.lock();
			_TextureToAnimate = &_PlayerAnimationTClusters._Textures[RUNNING_FACING_RIGHT];
			_MutexForMainThread.unlock();

			break;

		default:
			break;
		}

		break;

	case SDL_EVENT_KEY_UP:
		if (_UserEvent.key.scancode != SDL_SCANCODE_A && _UserEvent.key.scancode != SDL_SCANCODE_D)
			break;

		_Player._IsRunning = false;

		switch (_Player._Facing)
		{
		case LEFT:
			_MutexForMainThread.lock();
			_TextureToAnimate = &_PlayerAnimationTClusters._Textures[STANDING_FACING_LEFT];
			_MutexForMainThread.unlock();

			break;

		case RIGHT:
			_MutexForMainThread.lock();
			_TextureToAnimate = &_PlayerAnimationTClusters._Textures[STANDING_FACING_RIGHT];
			_MutexForMainThread.unlock();

			break;

		default:
			break;
		}

		break;

	default:
		break;
	}

	return;
};

//Thread that constantly updates the screen with an animation of player's character + it can also change the player's coords
static INLINE void RenderFrame(SDL_Renderer* const _TextureRenderer, Entity* const _Player, SDL_Texture** const _PlayerActiveTexture, SDL_Texture** const _BackgroundActiveTexture, SDL_FRect* const _BackgroundSize)
{
	if (*_PlayerActiveTexture == nullptr || *_BackgroundActiveTexture == nullptr)
		return;

	//Firstly render a black background, the render all the given textures in order and then update the screen with the new frame
	SDL_RenderClear(_TextureRenderer);
	SDL_RenderTexture(_TextureRenderer, *_BackgroundActiveTexture, NULL, _BackgroundSize);
	SDL_RenderTexture(_TextureRenderer, *_PlayerActiveTexture, NULL, &_Player->_PositionAndSize);
	SDL_RenderPresent(_TextureRenderer);
	
	return;
};

namespace GameLoopThread //[start]
{

//Loop for processing user events and rendering the game - runs on 'Main thread'
void MainLoop(SDL_Renderer* const _TextureRenderer, TCluster_2D& _PlayerTClusters, TCluster& _BackgroundTCluster)
{
	//Mutex to safely operate with critical sector of 'MainThread'
	std::mutex MutexForMainThread;
	//Containers holding currently made events
	SDL_Event UserEvent = SDL_Event();
	//Player's status represented as individual data
	Entity Player = PlayerThread::PutDefaultValues();
	//Temp storage for background size - will be changed!
	SDL_FRect BackgroundSize = SDL_FRect(0, 0, 1280, 720);
	//Textures that should be rendered on screen - thread unsafe
	SDL_Texture* PlayerActiveTexture = nullptr;
	SDL_Texture* BackgroundActiveTexture = nullptr;
	//Cluster of textures that should be animated - thread unsafe
	TCluster* PlayerTexturesToAnimate = &_PlayerTClusters._Textures[STANDING_FACING_LEFT]; //Setting a default textures to animate so something will always be on the screen
	TCluster* BackgroundTexturesToAnimate = &_BackgroundTCluster; //Setting a default textures to animate so something will always be on the screen
	//Variable that tells the supportive threads when to stop
	std::atomic_bool ThreadShouldFinish = false;
	
	//The supportive threads starts
	std::thread PlayerThread(&PlayerThread::Main, &PlayerActiveTexture, &PlayerTexturesToAnimate, 250, &Player, &ThreadShouldFinish);
	std::thread BackgroundThread(&BackgroundThread::Main, &BackgroundActiveTexture, &BackgroundTexturesToAnimate, 300, &ThreadShouldFinish);

	while (true)
	{
		//Get event poll and process it
		SDL_PollEvent(&UserEvent);

		//User requested to close the window - shutdown the game
		if (UserEvent.type == SDL_EVENT_QUIT)
			break;

		//User pressed key A or D or released them - or didn't do anything new
		InterpretUserEvent(UserEvent, Player, PlayerTexturesToAnimate, _PlayerTClusters, MutexForMainThread);
		//Render current frame
		RenderFrame(_TextureRenderer, &Player, &PlayerActiveTexture, &BackgroundActiveTexture, &BackgroundSize);
		//This ensures that only ~1000 events will be collected and frames renderered in a second <- More is not needed
		std::this_thread::sleep_for((std::chrono::milliseconds)1);
	}

	//Telling the supportive threads to stop and waiting for them to join
	ThreadShouldFinish = true;
	PlayerThread.join();
	BackgroundThread.join();
	
	return;
};

}
//GameLoopThread [end]