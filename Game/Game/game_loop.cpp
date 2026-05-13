#include "escape_from_the_island.hpp"

//Thread that constantly updates the screen
static INLINE void RenderFrame(SDL_Renderer*& _TextureRenderer, ECluster& _AllEntities, user_events& _AllUserEvents, HCluster& _AllOtherHitboxes)
{
	if (_AllEntities[PLAYER]._Textures._ActiveTexture == nullptr || _AllEntities[BACKGROUND]._Textures._ActiveTexture == nullptr)
		return;

	//Firstly render a black background, the render all the given textures in order and then update the screen with the new frame
	//Only a few entities will be rendered
	SDL_RenderClear(_TextureRenderer);

	//Temp!
	_AllEntities[BACKGROUND].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[PLAYER].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[FLOATING_STONE].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[SPIKE].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[CRATE_1].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[CRATE_2].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[CRATE_3].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[CRATE_4].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[MENU_PANEL].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);
	_AllEntities[MENU_BUTTONS].render_itself(_TextureRenderer, _AllUserEvents._F1_KeyPressed);

	//Temp!
	_AllOtherHitboxes.try_highlight_all(_TextureRenderer, _AllUserEvents._F1_KeyPressed);

	//
	SDL_RenderPresent(_TextureRenderer);

	return;
};

//
static INLINE void ApplyGameConditions(ECluster& _AllEntities, user_events& _AllUserEvents, game_env& _GameState, HCluster& _AllOtherHitboxes)
{
	//
	GameConditions::LocationChange(_AllUserEvents, _GameState);
	_GameState.prepare_location(_AllEntities, _AllOtherHitboxes); //Only if the location is not ready
	//
	if (_GameState._CurrentLocation == MAIN_MENU)
	{
		GameConditions::MenuButtonHighlightAndTrigger(_AllEntities[MENU_BUTTONS], _AllUserEvents, _GameState);
	}
	else
	{
		GameConditions::PlayerMovement(_AllEntities[PLAYER], _AllUserEvents);
		GameConditions::PlayerJump(_AllEntities[PLAYER], _AllUserEvents, _GameState);
		GameConditions::PlayerCollisions(_AllEntities, _AllOtherHitboxes);
		GameConditions::FloatingStoneMovement(_AllEntities[FLOATING_STONE]);
		GameConditions::FloatingStonePlayerInteraction(_AllEntities);
		GameConditions::SpikeAndRudderPlayerInteration(_AllEntities, _GameState, _AllOtherHitboxes);
		GameConditions::LevelChange(_AllEntities, _GameState, _AllOtherHitboxes);
	}

	return;
};

//
static INLINE void AlterEntitiesPositionsAndTextures(ECluster& _AllEntities, game_env& _GameState)
{
	//Every tick!
	_AllEntities[PLAYER].make_one_movement();
	_AllEntities[FLOATING_STONE].make_one_movement();

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

	//Temp!!!!!!!
	HCluster AllOtherHitboxes;

	//Temp! Redo!
	int64_t ScalingCoefficient = NULL;
	ConfigFile::ReadValue(BUNDLE_AppWindowConfig, SELECTOR_ScalingCoefficient, ScalingCoefficient);

	AllOtherHitboxes._ClusterOfHitboxes =
	{
		hitbox(false, SDL_FRect(ScalingCoefficient * 0.0f, ScalingCoefficient * 159.0f, ScalingCoefficient * 320.0f, ScalingCoefficient * 5.0f)), //LEVEL_FLOOR
		hitbox(false, SDL_FRect(ScalingCoefficient * 156.0f, ScalingCoefficient * 80.0f, ScalingCoefficient * 41.0f, ScalingCoefficient * 79.0f)), //MOUNTAIN
		hitbox(false, SDL_FRect(ScalingCoefficient * 0.0f, ScalingCoefficient * 120.0f, ScalingCoefficient * 25.0f, ScalingCoefficient * 50.0f)), //ENTRANCE_GATE
		hitbox(false, SDL_FRect(ScalingCoefficient * 300.0f, ScalingCoefficient * 120.0f, ScalingCoefficient * 20.0f, ScalingCoefficient * 50.0f)), //EXIT_GATE
		hitbox(false, SDL_FRect(ScalingCoefficient * 95.0f, ScalingCoefficient * 125.0f, ScalingCoefficient * 15.0f, ScalingCoefficient * 35.0f)) //BOAT_RUDDER
		//
	};

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
		ApplyGameConditions(_AllEntities, _AllUserEvents, _GameState, AllOtherHitboxes);
		//
		AlterEntitiesPositionsAndTextures(_AllEntities, _GameState);
		//Render current frame
		RenderFrame(TextureRenderer, _AllEntities, _AllUserEvents, AllOtherHitboxes);
		//
		_GameState.update_tick();
	}
	//----------<GAME LOOP>----------
	
	RuntimeLog::Message(INFO, "game loop terminated => logging enabled");
	
	return;
};

}
//GameLoop [end]