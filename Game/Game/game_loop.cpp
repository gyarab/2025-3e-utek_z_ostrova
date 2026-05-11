#include "escape_from_the_island.hpp"

//Thread that constantly updates the screen
static INLINE void RenderFrame(SDL_Renderer*& _TextureRenderer, ECluster& _AllEntities, user_events& _AllUserEvents)
{
	if (_AllEntities[PLAYER]._Textures._ActiveTexture == nullptr || _AllEntities[BACKGROUND]._Textures._ActiveTexture == nullptr)
		return;

	//Firstly render a black background, the render all the given textures in order and then update the screen with the new frame
	//Only a few entities will be rendered
	SDL_RenderClear(_TextureRenderer);

	//Temp!
	_AllEntities[BACKGROUND].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[PLAYER].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[MENU_PANEL].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[MENU_BUTTONS].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);

	//
	SDL_RenderPresent(_TextureRenderer);

	return;
};

//
static INLINE void ApplyGameConditions(ECluster& _AllEntities, user_events& _AllUserEvents, game_env& _GameState)
{
	//
	GameConditions::LocationChange(_AllUserEvents, _GameState);
	_GameState.prepare_location(_AllEntities); //Only if the location is not ready
	//
	if (_GameState._CurrentLocation == MAIN_MENU)
	{
		GameConditions::MenuButtonHighlightAndTrigger(_AllEntities[MENU_BUTTONS], _AllUserEvents, _GameState);
	}
	else
	{
		GameConditions::PlayerMovement(_AllEntities[PLAYER], _AllUserEvents);
		GameConditions::PlayerJump(_AllEntities[PLAYER], _AllUserEvents, _GameState);
		GameConditions::PlayerCollisions(_AllEntities, _AllUserEvents, _GameState);
	}

	return;
};

//
static INLINE void AlterEntitiesPositionsAndTextures(ECluster& _AllEntities, game_env& _GameState)
{
	//Every tick!
	_AllEntities[PLAYER].make_one_movement();

	//Every fifth tick!
	if (_GameState._Tick % 5 == 0)
	{
		_AllEntities[PLAYER]._Textures.switch_to_next();
		_AllEntities[BACKGROUND]._Textures.switch_to_next();
	}

	return;
};

namespace GameLoop //[start]
{

//Loop for processing user events and rendering the game - runs on 'Main thread'
void MainLoop(WRCluster& _MainWindow, ECluster& _AllEntities, user_events& _AllUserEvents, game_env& _GameState)
{
	//Direct access to the renderer that will to all the rendering
	SDL_Renderer* TextureRenderer = _MainWindow._Renderers[TEXTURE_RENDERER];

	RuntimeLog::Message(INFO, "game loop started => logging disabled");

	//----------<GAME LOOP>----------
	while (true)
	{
		//
		_AllUserEvents.collect_recent_events();

		//_AllEntities[MENU_BUTTONS]._Textures.switch_to_next();


		//User requested to close the window - shutdown the game
		if (_AllUserEvents._ShutdownRequested || _GameState._QuitButtonPressed)
			break;

		//Apply the game conditions on the entities status
		ApplyGameConditions(_AllEntities, _AllUserEvents, _GameState);
		//
		AlterEntitiesPositionsAndTextures(_AllEntities, _GameState);
		//Render current frame
		RenderFrame(TextureRenderer, _AllEntities, _AllUserEvents);
		//
		_GameState.update_tick();
	}
	//----------<GAME LOOP>----------
	
	RuntimeLog::Message(INFO, "game loop terminated => logging enabled");
	
	return;
};

}
//GameLoop [end]