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
void game_env::prepare_location(ECluster& _AllEntities, HCluster& _AllOtherHitboxes)
{
	if (this->_LocationIsReady)
		return; //Already prepared

	//Temp! Redo!
	int64_t ScalingCoefficient = NULL;
	ConfigFile::ReadValue(BUNDLE_AppWindowConfig, SELECTOR_ScalingCoefficient, ScalingCoefficient);

	//Temp! Redo!
	switch (this->_CurrentLocation)
	{
	case MAIN_MENU:
		_AllEntities[BACKGROUND]._Textures.mark_as_active(BACKGROUND_LEVEL_2);
		_AllEntities[MENU_PANEL]._Textures.mark_as_active(PANEL);
		_AllEntities[MENU_BUTTONS]._Textures.mark_as_active(BUTTONS);
		_AllEntities[MENU_BUTTONS]._Hitbox.x = ScalingCoefficient * 127.0f;
		_AllEntities[MENU_BUTTONS]._Hitbox.y = ScalingCoefficient * 59.0f;

		_AllEntities[PLAYER]._IsRenderable = false;
		_AllEntities[BACKGROUND]._IsRenderable = true;
		_AllEntities[FLOATING_STONE]._IsRenderable = false;
		_AllEntities[SPIKE]._IsRenderable = false;
		_AllEntities[CRATE_1]._IsRenderable = false;
		_AllEntities[CRATE_2]._IsRenderable = false;
		_AllEntities[CRATE_3]._IsRenderable = false;
		_AllEntities[CRATE_4]._IsRenderable = false;
		_AllEntities[MENU_PANEL]._IsRenderable = true;
		_AllEntities[MENU_BUTTONS]._IsRenderable = true;

		_AllOtherHitboxes[LEVEL_FLOOR]._IsActive = false;
		_AllOtherHitboxes[MOUNTAIN]._IsActive = false;
		_AllOtherHitboxes[ENTRANCE_GATE]._IsActive = false;
		_AllOtherHitboxes[EXIT_GATE]._IsActive = false;
		_AllOtherHitboxes[BOAT_RUDDER]._IsActive = false;

		break;

	case LEVEL_1:
		_AllEntities[BACKGROUND]._Textures.mark_as_active(BACKGROUND_LEVEL_1);
		_AllEntities[PLAYER]._Textures.mark_as_active(STANDING_RIGHT);
		_AllEntities[FLOATING_STONE]._Textures.mark_as_active(TEXTURE_FLOATING_STONE);
		_AllEntities[PLAYER]._Hitbox.x = ScalingCoefficient * 20.0f;
		_AllEntities[PLAYER]._Hitbox.y = ScalingCoefficient * 127.0f;
		_AllEntities[FLOATING_STONE]._Hitbox.x = ScalingCoefficient * 117.0f;
		_AllEntities[FLOATING_STONE]._Hitbox.y = ScalingCoefficient * 165.0f;

		_AllEntities[PLAYER]._IsRenderable = true;
		_AllEntities[BACKGROUND]._IsRenderable = true;
		_AllEntities[FLOATING_STONE]._IsRenderable = true;
		_AllEntities[SPIKE]._IsRenderable = false;
		_AllEntities[CRATE_1]._IsRenderable = false;
		_AllEntities[CRATE_2]._IsRenderable = false;
		_AllEntities[CRATE_3]._IsRenderable = false;
		_AllEntities[CRATE_4]._IsRenderable = false;
		_AllEntities[MENU_PANEL]._IsRenderable = false;
		_AllEntities[MENU_BUTTONS]._IsRenderable = false;

		_AllOtherHitboxes[LEVEL_FLOOR]._IsActive = true;
		_AllOtherHitboxes[MOUNTAIN]._IsActive = true;
		_AllOtherHitboxes[ENTRANCE_GATE]._IsActive = true;
		_AllOtherHitboxes[EXIT_GATE]._IsActive = true;
		_AllOtherHitboxes[BOAT_RUDDER]._IsActive = false;

		break;

	case LEVEL_2:
		_AllEntities[BACKGROUND]._Textures.mark_as_active(BACKGROUND_LEVEL_2);
		_AllEntities[PLAYER]._Textures.mark_as_active(STANDING_RIGHT);
		_AllEntities[SPIKE]._Textures.mark_as_active(TEXTURE_SPIKE);
		_AllEntities[CRATE_1]._Textures.mark_as_active(TEXTURE_CRATE_1);
		_AllEntities[CRATE_2]._Textures.mark_as_active(TEXTURE_CRATE_2);
		_AllEntities[CRATE_3]._Textures.mark_as_active(TEXTURE_CRATE_1);
		_AllEntities[CRATE_4]._Textures.mark_as_active(TEXTURE_CRATE_2);
		_AllEntities[PLAYER]._Hitbox.x = ScalingCoefficient * 20.0f;
		_AllEntities[PLAYER]._Hitbox.y = ScalingCoefficient * 127.0f;
		_AllEntities[SPIKE]._Hitbox.x = ScalingCoefficient * 180.0f;
		_AllEntities[SPIKE]._Hitbox.y = ScalingCoefficient * 147.0f;
		_AllEntities[CRATE_1]._Hitbox.x = ScalingCoefficient * 140.0f;
		_AllEntities[CRATE_1]._Hitbox.y = ScalingCoefficient * 143.0f;
		_AllEntities[CRATE_2]._Hitbox.x = ScalingCoefficient * 160.0f;
		_AllEntities[CRATE_2]._Hitbox.y = ScalingCoefficient * 143.0f;
		_AllEntities[CRATE_3]._Hitbox.x = ScalingCoefficient * 151.0f;
		_AllEntities[CRATE_3]._Hitbox.y = ScalingCoefficient * 127.0f;
		_AllEntities[CRATE_4]._Hitbox.x = ScalingCoefficient * 220.0f;
		_AllEntities[CRATE_4]._Hitbox.y = ScalingCoefficient * 143.0f;

		_AllEntities[PLAYER]._IsRenderable = true;
		_AllEntities[BACKGROUND]._IsRenderable = true;
		_AllEntities[FLOATING_STONE]._IsRenderable = false;
		_AllEntities[SPIKE]._IsRenderable = true;
		_AllEntities[CRATE_1]._IsRenderable = true;
		_AllEntities[CRATE_2]._IsRenderable = true;
		_AllEntities[CRATE_3]._IsRenderable = true;
		_AllEntities[CRATE_4]._IsRenderable = true;
		_AllEntities[MENU_PANEL]._IsRenderable = false;
		_AllEntities[MENU_BUTTONS]._IsRenderable = false;

		_AllOtherHitboxes[LEVEL_FLOOR]._IsActive = true;
		_AllOtherHitboxes[MOUNTAIN]._IsActive = false;
		_AllOtherHitboxes[ENTRANCE_GATE]._IsActive = true;
		_AllOtherHitboxes[EXIT_GATE]._IsActive = true;
		_AllOtherHitboxes[BOAT_RUDDER]._IsActive = false;

		break;

	case ISLAND:
		_AllEntities[BACKGROUND]._Textures.mark_as_active(BACKGROUND_ISLAND);
		_AllEntities[PLAYER]._Textures.mark_as_active(STANDING_LEFT);
		_AllEntities[PLAYER]._Hitbox.x = ScalingCoefficient * (320.0f - 60.0f);
		_AllEntities[PLAYER]._Hitbox.y = ScalingCoefficient * 127.0f;

		_AllEntities[PLAYER]._IsRenderable = true;
		_AllEntities[BACKGROUND]._IsRenderable = true;
		_AllEntities[FLOATING_STONE]._IsRenderable = false;
		_AllEntities[SPIKE]._IsRenderable = false;
		_AllEntities[CRATE_1]._IsRenderable = false;
		_AllEntities[CRATE_2]._IsRenderable = false;
		_AllEntities[CRATE_3]._IsRenderable = false;
		_AllEntities[CRATE_4]._IsRenderable = false;
		_AllEntities[MENU_PANEL]._IsRenderable = false;
		_AllEntities[MENU_BUTTONS]._IsRenderable = false;

		_AllOtherHitboxes[LEVEL_FLOOR]._IsActive = true;
		_AllOtherHitboxes[MOUNTAIN]._IsActive = false;
		_AllOtherHitboxes[ENTRANCE_GATE]._IsActive = true;
		_AllOtherHitboxes[EXIT_GATE]._IsActive = true;
		_AllOtherHitboxes[BOAT_RUDDER]._IsActive = true;

		break;

	default:
		break;
	}

	this->_LocationIsReady = true;

	return;
};