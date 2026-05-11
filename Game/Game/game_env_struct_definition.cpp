#include "escape_from_the_island.hpp"

//
void game_env::update_tick(void)
{
	std::this_thread::sleep_for(this->_TickDelay);
	this->_Tick == this->_TickCount ? this->_Tick = 1 : this->_Tick++;

	return;
};

//
void game_env::set_location(const GameLocationNames _NewLocation)
{
	if (_NewLocation == this->_CurrentLocation)
		return; //The same location has been already set

	this->_CurrentLocation = _NewLocation;
	this->_LocationIsReady = false;

	return;
};

//
void game_env::prepare_location(ECluster& _AllEntities)
{
	if (this->_LocationIsReady)
		return; //Already prepared

	//Temp! Redo!
	switch (this->_CurrentLocation)
	{
	case MAIN_MENU:
		_AllEntities[BACKGROUND]._Textures.mark_as_active(BACKGROUND_LEVEL_2);
		_AllEntities[MENU_PANEL]._Textures.mark_as_active(PANEL);
		_AllEntities[MENU_BUTTONS]._Textures.mark_as_active(BUTTONS);
		_AllEntities[MENU_BUTTONS]._Hitbox.x = 4 * 127;
		_AllEntities[MENU_BUTTONS]._Hitbox.y = 4 * 59;

		_AllEntities[PLAYER]._IsRenderable = false;
		_AllEntities[BACKGROUND]._IsRenderable = true;
		_AllEntities[MENU_PANEL]._IsRenderable = true;
		_AllEntities[MENU_BUTTONS]._IsRenderable = true;

		break;

	case LEVEL_1:
		_AllEntities[BACKGROUND]._Textures.mark_as_active(BACKGROUND_LEVEL_1);
		_AllEntities[PLAYER]._Textures.mark_as_active(STANDING_RIGHT);
		_AllEntities[PLAYER]._Hitbox.x = 4 * 10;
		_AllEntities[PLAYER]._Hitbox.y = 4 * 127;

		_AllEntities[PLAYER]._IsRenderable = true;
		_AllEntities[BACKGROUND]._IsRenderable = true;
		_AllEntities[MENU_PANEL]._IsRenderable = false;
		_AllEntities[MENU_BUTTONS]._IsRenderable = false;

		break;

	case LEVEL_2:
		_AllEntities[BACKGROUND]._Textures.mark_as_active(BACKGROUND_LEVEL_2);
		_AllEntities[PLAYER]._Textures.mark_as_active(STANDING_RIGHT);
		_AllEntities[PLAYER]._Hitbox.x = 4 * 10;
		_AllEntities[PLAYER]._Hitbox.y = 4 * 127;

		_AllEntities[PLAYER]._IsRenderable = true;
		_AllEntities[BACKGROUND]._IsRenderable = true;
		_AllEntities[MENU_PANEL]._IsRenderable = false;
		_AllEntities[MENU_BUTTONS]._IsRenderable = false;

		break;

	case ISLAND:
		_AllEntities[BACKGROUND]._Textures.mark_as_active(BACKGROUND_ISLAND);
		_AllEntities[PLAYER]._Textures.mark_as_active(STANDING_LEFT);
		_AllEntities[PLAYER]._Hitbox.x = 4 * (320 - 50);
		_AllEntities[PLAYER]._Hitbox.y = 4 * 127;

		_AllEntities[PLAYER]._IsRenderable = true;
		_AllEntities[BACKGROUND]._IsRenderable = true;
		_AllEntities[MENU_PANEL]._IsRenderable = false;
		_AllEntities[MENU_BUTTONS]._IsRenderable = false;

		break;

	default:
		break;
	}

	this->_LocationIsReady = true;

	return;
};