#include "escape_from_the_island.hpp"

#if 0
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
#endif

//Thread that constantly updates the screen with an animation of player's character + it can also change the player's coords
#if 0
static INLINE void RenderFrame(SDL_Renderer* const _TextureRenderer, Entity* const _Player, SDL_Texture** const _PlayerActiveTexture, SDL_Texture** const _BackgroundActiveTexture, SDL_FRect* const _BackgroundSize)
#endif
static INLINE void RenderFrame(SDL_Renderer*& _TextureRenderer, ECluster& _AllEntities)
{

	if (_AllEntities[PLAYER]._Textures._ActiveTexture == nullptr || _AllEntities[BACKGROUND]._Textures._ActiveTexture == nullptr)
		return;

	//Firstly render a black background, the render all the given textures in order and then update the screen with the new frame
	//Only a few entities will be rendered
	SDL_RenderClear(_TextureRenderer);
	SDL_RenderTexture(_TextureRenderer, *_AllEntities[BACKGROUND]._Textures._ActiveTexture, NULL, &_AllEntities[BACKGROUND]._Hitbox);
	SDL_RenderTexture(_TextureRenderer, *_AllEntities[PLAYER]._Textures._ActiveTexture, NULL, &_AllEntities[PLAYER]._Hitbox);
	SDL_RenderPresent(_TextureRenderer);
	
	return;
};

namespace GameLoopThread //[start]
{

//Loop for processing user events and rendering the game - runs on 'Main thread'
void MainLoop(WRCluster& _MainWindow, ECluster& _AllEntities, user_events& _AllUserEvents)
{
	//Mutex to safely operate with critical sector of 'MainThread'
	std::mutex MutexForMainThread;
	//Direct access to the renderer that will to all the rendering
	SDL_Renderer* TextureRenderer = _MainWindow._Renderers[TEXTURE_RENDERER];
#if 0
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
#endif
	//Variable that tells the supportive threads when to stop
	std::atomic_bool ThreadShouldFinish = false;
	
	//The supportive threads starts
	std::thread PlayerThread(&PlayerThread::Main, &_AllEntities[PLAYER], 250ms, &ThreadShouldFinish);
	std::thread BackgroundThread(&BackgroundThread::Main, &_AllEntities[BACKGROUND], 300ms, &ThreadShouldFinish);

	RuntimeLog::Message(INFO, "game loop started => logging disabled");

	//Temp!
	_AllEntities[PLAYER]._Textures.mark_as_active(0);
	_AllEntities[BACKGROUND]._Textures.mark_as_active(0);

	//Temp!
	_AllEntities[PLAYER]._Hitbox.y = 4 * 127;
	
	while (true)
	{
		_AllUserEvents.collect_recent_events();

		//User requested to close the window - shutdown the game
		if (_AllUserEvents._ShutdownRequested)
			break;

		GameConditions::PlayerMovement(_AllEntities[PLAYER], _AllUserEvents, MutexForMainThread);

		//Change this!
#if 0
		//User pressed key A or D or released them - or didn't do anything new
		InterpretUserEvent(UserEvent, Player, PlayerTexturesToAnimate, _PlayerTClusters, MutexForMainThread);
#endif
		//Render current frame
		RenderFrame(TextureRenderer, _AllEntities);
		//This ensures that only ~1000 events will be collected and frames renderered in a second <- More is not needed
		std::this_thread::sleep_for(1ms);
	}

	RuntimeLog::Message(INFO, "game loop terminated => logging enabled");

	//Telling the supportive threads to stop and waiting for them to join
	ThreadShouldFinish = true;
	PlayerThread.join();
	BackgroundThread.join();
	
	return;
};

}
//GameLoopThread [end]