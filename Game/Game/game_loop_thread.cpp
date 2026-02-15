#include "escape_from_the_island.hpp"

namespace GameLoopThread //[start]
{

//Function that takes an user event and transforms it into a player status data and animation
void AssignPlayerStatusAndAnimationToUserEvent(const SDL_Event& _UserEvent, Entity& _Player, TCluster*& _TextureToAnimate, std::array<TCluster, 4>& _PlayerAnimationTClusters, std::mutex& _MutexForMainThread)
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
			_TextureToAnimate = &_PlayerAnimationTClusters[RUNNING_FACING_LEFT];
			_MutexForMainThread.unlock();

			break;

		case SDL_SCANCODE_D:
			_Player._IsRunning = true;
			_Player._Facing = RIGHT;
			_MutexForMainThread.lock();
			_TextureToAnimate = &_PlayerAnimationTClusters[RUNNING_FACING_RIGHT];
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

		switch(_Player._Facing)
		{
		case LEFT:
			_MutexForMainThread.lock();
			_TextureToAnimate = &_PlayerAnimationTClusters[STANDING_FACING_LEFT];
			_MutexForMainThread.unlock();

			break;

		case RIGHT:
			_MutexForMainThread.lock();
			_TextureToAnimate = &_PlayerAnimationTClusters[STANDING_FACING_RIGHT];
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

//Loop for processing user events and rendering the game - runs on 'Main thread'
void MainLoop(SDL_Renderer* const _TextureRenderer, const uint64_t _ScalingCoeficient, std::array<TCluster, 4>& _PlayerAnimationTClusters)
{
	//Mutex to safely operate with critical sector of 'MainThread'
	std::mutex MutexForMainThread;
	//Containers holding current and previously made events
	SDL_Event CurrentUserEvent = SDL_Event();
	SDL_Event PreviousUserEvent = SDL_Event();
	//Player's status represented as individual data
	Entity Player = PlayerThread::PutDefaultValuesForPlayer(_ScalingCoeficient);
	//Texture that should be rendered on screen - thread unsafe
	SDL_Texture* TextureToRender = nullptr;
	//Cluster of textures that should be animated - thread unsafe
	TCluster* TexturesToAnimate = &_PlayerAnimationTClusters[STANDING_FACING_LEFT]; //Setting a default textures to animate so something will always be on the screen
	//Variable that tells the 'AnimatePlayerTextureClusterThread' and 'FrameRenderThread' when to stop
	std::atomic_bool ThreadShouldFinish = false;
	//Variable that tells the 'AnimatePlayerTextureClusterThread' to stop animating for a while when the animation textures has to be changed
	std::atomic_bool AnimationInterrupted = false;
	
	//Sets the default frame color
	WindowRenderHandle::SetFrameDefaultColorToBlack(_TextureRenderer);
	//The 'AnimatePlayerTextureClusterThread' and 'FrameRenderThread' starts
	std::thread AnimatePlayerTextureClusterThread(&PlayerThread::AnimatePlayerTextureClusterThreadMain, &TextureToRender, &TexturesToAnimate, 300, &AnimationInterrupted, &ThreadShouldFinish);
	std::thread FrameRenderThread(&PlayerThread::FrameRenderThreadMain, _TextureRenderer, &Player, &TextureToRender, &ThreadShouldFinish);
	
	while (true)
	{
		//Get event poll and process it
		SDL_PollEvent(&CurrentUserEvent);

		//User requested to close the window - shutdown the game
		if (CurrentUserEvent.type == SDL_EVENT_QUIT)
			break;

		//User pressed key A or D or released them - or didn't do anything new
		//Run this function only if the current event is different than the previous
		if (CurrentUserEvent.type != PreviousUserEvent.type || CurrentUserEvent.key.scancode != PreviousUserEvent.key.scancode)
		{
			//Thread has to stop animating
			AnimationInterrupted = true;
			GameLoopThread::AssignPlayerStatusAndAnimationToUserEvent(CurrentUserEvent, Player, TexturesToAnimate, _PlayerAnimationTClusters, MutexForMainThread);
			//Thread can continue animating
			AnimationInterrupted = false;
		}

		//Save current event as previous to compare it later with new event
		PreviousUserEvent = CurrentUserEvent;
		//This ensures that only ~1000 events will be collected in a second <- More is not needed
		std::this_thread::sleep_for((std::chrono::milliseconds)1);
	}

	//Telling the 'AnimatePlayerTextureClusterThread' and 'FrameRenderThread' to stop and waiting for them to join
	ThreadShouldFinish = true;
	AnimatePlayerTextureClusterThread.join();
	FrameRenderThread.join();

	return;
};

}
//GameLoopThread [end]