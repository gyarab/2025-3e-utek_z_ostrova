#include "escape_from_the_island.hpp"

namespace GameConditions //[start]
{

//
static bool temp = false;

//
void LocationChange(user_events& _AllUserEvents, game_env& _GameState)
{
	//Change to level 1
	if (_GameState._CurrentLocation == MAIN_MENU && _GameState._PlayButtonPressed)
	{
		_GameState.set_location(LEVEL_1);
		_GameState._PlayButtonPressed = false;

		return;
	}

	//Change to level 2
	if (_GameState._CurrentLocation == LEVEL_1 && _GameState._Level_1_Completed)
	{
		_GameState.set_location(LEVEL_2);

		return;
	}

	//Change to island
	if (_GameState._CurrentLocation == LEVEL_2 && _GameState._Level_2_Completed)
	{
		_GameState.set_location(ISLAND);

		return;
	}
	
	//Change to main menu
	if (_AllUserEvents._ESC_KeyPressed || temp)
	{
		_GameState.set_location(MAIN_MENU);
		temp = false;
		_GameState._Level_1_Completed = false;
		_GameState._Level_2_Completed = false;
	}

	return;
};

//
void MenuButtonHighlightAndTrigger(entity& _MenuButtons, user_events& _AllUserEvents, game_env& _GameState)
{
	//Temp!
	if (!(_AllUserEvents._MouseX >= _MenuButtons._Hitbox.x && _AllUserEvents._MouseX <= _MenuButtons._Hitbox.x + _MenuButtons._Hitbox.w))
		return;

	//Temp!
	if (_AllUserEvents._MouseY >= _MenuButtons._Hitbox.y && _AllUserEvents._MouseY <= _MenuButtons._Hitbox.y + (_MenuButtons._Hitbox.h / 3))
	{
		_MenuButtons._Textures.switch_to_specific(HIGHLIGHTED_PLAY);

		if (_AllUserEvents._LeftButtonSingleClick)
			_GameState._PlayButtonPressed = true;
	}
	else if (_AllUserEvents._MouseY >= _MenuButtons._Hitbox.y + (_MenuButtons._Hitbox.h / 3) && _AllUserEvents._MouseY <= _MenuButtons._Hitbox.y + (2 * _MenuButtons._Hitbox.h / 3))
	{
		_MenuButtons._Textures.switch_to_specific(HIGHLIGHTED_LEVELS);
	}
	else if (_AllUserEvents._MouseY >= _MenuButtons._Hitbox.y + (2 * _MenuButtons._Hitbox.h / 3) && _AllUserEvents._MouseY <= _MenuButtons._Hitbox.y + _MenuButtons._Hitbox.h)
	{
		_MenuButtons._Textures.switch_to_specific(HIGHLIGHTED_QUIT);

		if (_AllUserEvents._LeftButtonSingleClick)
			_GameState._QuitButtonPressed = true;
	}
	else
	{
		_MenuButtons._Textures.switch_to_specific(PLAY_LEVELS_QUIT);
	}

	return;
};

//
void PlayerMovement(entity& _Player, user_events& _AllUserEvents)
{
	//Move left
	if (_AllUserEvents._A_KeyPressed || _AllUserEvents._LEFT_ARROW_KeyPressed)
	{
		_Player._Textures.mark_as_active(MOVING_LEFT);
		_Player.set_vector_direction_to_left();

		return;
	}

	//Move right
	if (_AllUserEvents._D_KeyPressed || _AllUserEvents._RIGHT_ARROW_KeyPressed)
	{
		_Player._Textures.mark_as_active(MOVING_RIGHT);
		_Player.set_vector_direction_to_right();

		return;
	}

	//Stand still
	const int64_t PlayerVectorX_Direction = _Player.tell_vector_x_direction();

	if (PlayerVectorX_Direction != 0)
		_Player._Textures.mark_as_active((PlayerVectorX_Direction == -1 ? STANDING_LEFT : STANDING_RIGHT));

	_Player.nullify_vector_x_direction();

	return;
};

//Temp!!!
static uint64_t tick_wait;

//
void PlayerJump(entity& _Player, user_events& _AllUserEvents, game_env& _GameState)
{
	//Temp!
	if (tick_wait > 0)
	{
		tick_wait--;

		return;
	}

	if (!_AllUserEvents._SPACE_KeyPressed)
		_GameState._SPACE_KeyLock = false;

	//Jump up
	//Temp!
	if (_AllUserEvents._SPACE_KeyPressed && !_GameState._SPACE_KeyLock && _Player.tell_vector_y_direction() == 0)
	{
		_Player.set_vector_direction_to_up();
		//Temp!
		_GameState._SPACE_KeyLock = true;
		tick_wait = 5;

		return;
	}

	//Stand still - auto gravity
	//Temp!
	_Player.set_vector_direction_to_down();

	return;
};

//
void PlayerCollisions(ECluster& _AllEntities, HCluster& _AllOtherHitboxes)
{
	//
	entity* Player = (_AllEntities._ClusterOfEntities.data() + PLAYER);
	int64_t PlayerX_Direction = Player->tell_vector_x_direction();
	int64_t PlayerY_Direction = Player->tell_vector_y_direction();

	//Temp!
	for (hitbox OneHitbox : _AllOtherHitboxes._ClusterOfHitboxes)
		if ((Player->hitbox_is_touching_hitbox_on_left(OneHitbox) && PlayerX_Direction == -1) || (Player->hitbox_is_touching_hitbox_on_right(OneHitbox) && PlayerX_Direction == 1))
		{
			Player->_Textures.mark_as_active((Player->_Textures._ActiveSubcluster == &Player->_Textures[MOVING_LEFT] ? STANDING_LEFT : STANDING_RIGHT));
			Player->nullify_vector_x_direction(); break;
		}

	for (hitbox OneHitbox : _AllOtherHitboxes._ClusterOfHitboxes)
		if ((Player->hitbox_is_touching_hitbox_on_up(OneHitbox) && PlayerY_Direction == -1) || (Player->hitbox_is_touching_hitbox_on_down(OneHitbox) && PlayerY_Direction == 1))
		{
			Player->nullify_vector_y_direction(); break;
		}

	for (uint64_t c = CRATE_1; c < _AllEntities._ClusterOfEntities.size(); c++)
		//Temp! - additional for crate
		if ((Player->hitbox_is_touching_hitbox_on_left(_AllEntities[c]) && PlayerX_Direction == -1) || (Player->hitbox_is_touching_hitbox_on_right(_AllEntities[c]) && PlayerX_Direction == 1))
		{
			Player->_Textures.mark_as_active((Player->_Textures._ActiveSubcluster == &Player->_Textures[MOVING_LEFT] ? STANDING_LEFT : STANDING_RIGHT));
			Player->nullify_vector_x_direction();
		}

	for (uint64_t c = CRATE_1; c < _AllEntities._ClusterOfEntities.size(); c++)
		if ((Player->hitbox_is_touching_hitbox_on_up(_AllEntities[c]) && PlayerY_Direction == -1) || (Player->hitbox_is_touching_hitbox_on_down(_AllEntities[c]) && PlayerY_Direction == 1))
		{
			Player->nullify_vector_y_direction();
		}

	return;
};

//
void FloatingStoneMovement(entity& _FloatingStone)
{
	if (_FloatingStone._Hitbox.y >= 0.0f)
		_FloatingStone.set_vector_direction_to_up();
	else
	{
		_FloatingStone._Hitbox.y = 4 * 165.0f;
		_FloatingStone.nullify_vector_y_direction();
	}

	return;
};

//
void FloatingStonePlayerInteraction(ECluster& _AllEntities)
{
	if (_AllEntities[PLAYER].hitbox_is_touching_hitbox_on_down(_AllEntities[FLOATING_STONE]))
	{
		if (_AllEntities[PLAYER].tell_vector_y_direction() == 1)
			_AllEntities[PLAYER].nullify_vector_y_direction();

		_AllEntities[PLAYER]._Vector[DY] += _AllEntities[FLOATING_STONE]._Vector[DY];
	}

	return;
};

//
void SpikeAndRudderPlayerInteration(ECluster& _AllEntities, game_env& _GameState, HCluster& _AllOtherHitboxes)
{
	if (_AllEntities[PLAYER].hitbox_is_touching_hitbox_of(_AllEntities[SPIKE]) || _AllEntities[PLAYER].hitbox_is_touching_hitbox_of(_AllOtherHitboxes[BOAT_RUDDER]))
		temp = true;

	return;
};

//
void LevelChange(ECluster& _AllEntities, game_env& _GameState, HCluster& _AllOtherHitboxes)
{
	if (_AllEntities[PLAYER].hitbox_is_touching_hitbox_of(_AllOtherHitboxes[EXIT_GATE]))
		if (_GameState._CurrentLocation == LEVEL_1)
			_GameState._Level_1_Completed = true;
		else if (_GameState._CurrentLocation == LEVEL_2)
			_GameState._Level_2_Completed = true;

	return;
};

}
//GameConditions [end]