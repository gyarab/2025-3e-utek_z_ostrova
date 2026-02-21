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
static INLINE void RenderFrame(SDL_Renderer* const _TextureRenderer, Entity* const _Player, SDL_Texture** const _TextureToRender)
{
	if (*_TextureToRender == nullptr)
		return;

	//
	SDL_RenderClear(_TextureRenderer);
	SDL_RenderTexture(_TextureRenderer, *_TextureToRender, NULL, &_Player->_PositionAndSize);
	SDL_RenderPresent(_TextureRenderer);
	
	return;
};

//
static INLINE bool IsDifferentEvent(const SDL_Event& _FirstEvent, const SDL_Event& _SecondEvent)
{
	return
		!(_FirstEvent.type == _SecondEvent.type && _FirstEvent.key.scancode == _SecondEvent.key.scancode);
};

namespace GameLoopThread //[start]
{

//Loop for processing user events and rendering the game - runs on 'Main thread'
void MainLoop(SDL_Renderer* const _TextureRenderer, TCluster_2D& _PlayerTClusters, TCluster& _BackgroundTCluster)
{
	//Mutex to safely operate with critical sector of 'MainThread'
	std::mutex MutexForMainThread;
	//Containers holding current and previously made events
	SDL_Event CurrentUserEvent = SDL_Event();
	SDL_Event PreviousUserEvent = SDL_Event();
	//Player's status represented as individual data
	Entity Player = PlayerThread::PutDefaultValues();
	//Texture that should be rendered on screen - thread unsafe
	SDL_Texture* TextureToRender = nullptr;
	//Cluster of textures that should be animated - thread unsafe
	TCluster* TexturesToAnimate = &_PlayerTClusters._Textures[STANDING_FACING_LEFT]; //Setting a default textures to animate so something will always be on the screen
	//Variable that tells the 'PlayerThread' when to stop
	std::atomic_bool ThreadShouldFinish = false;
	//Variable that tells the 'PlayerThread' to stop animating for a while when the animation textures have to be changed
	std::atomic_bool AnimationInterrupted = false;
	
	//The 'PlayerThread' starts
	std::thread PlayerThread(&PlayerThread::Main, &TextureToRender, &TexturesToAnimate, 250, &Player, &AnimationInterrupted, &ThreadShouldFinish);
	//std::thread BackgroundThread

	while (true)
	{
		//Get event poll and process it
		SDL_PollEvent(&CurrentUserEvent);

		//User requested to close the window - shutdown the game
		if (CurrentUserEvent.type == SDL_EVENT_QUIT)
			break;

		//User pressed key A or D or released them - or didn't do anything new
		//Run this function only if the current event is different than the previous
		if (IsDifferentEvent(CurrentUserEvent, PreviousUserEvent))
		{
			//Thread has to stop animating
			AnimationInterrupted = true;
			InterpretUserEvent(CurrentUserEvent, Player, TexturesToAnimate, _PlayerTClusters, MutexForMainThread);
			//Thread can continue animating
			AnimationInterrupted = false;
		}

		//Save current event as previous to compare it later with new event
		PreviousUserEvent = CurrentUserEvent;
		//Render current frame
		RenderFrame(_TextureRenderer, &Player, &TextureToRender);
		//This ensures that only ~1000 events will be collected and frames renderered in a second <- More is not needed
		std::this_thread::sleep_for((std::chrono::milliseconds)1);
	}

	//Telling the 'PlayerThread' to stop and waiting for them to join
	ThreadShouldFinish = true;
	PlayerThread.join();
	
	return;
};

}
//GameLoopThread [end]