#include "escape_from_the_island.hpp"

//Thread that constantly updates the screen with an animation of player's character + it can also change the player's coords
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

		//Apply the game conditions on the entities status
		GameConditions::PlayerMovement(_AllEntities[PLAYER], _AllUserEvents, MutexForMainThread);

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