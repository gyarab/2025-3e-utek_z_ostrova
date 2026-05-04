#include "escape_from_the_island.hpp"

namespace GameConditions //[start]
{

//
void PlayerMovement(entity& _Player, user_events& _AllUserEvents, std::mutex& _OptionalThreadMutex)
{
	//Move left
	if (_AllUserEvents._A_KeyPressed)
	{
		_OptionalThreadMutex.lock();
		_Player._Textures.mark_as_active(MOVING_LEFT);
		_Player.set_vector_direction_to_left();
		_Player._IsMoving = true;
		_OptionalThreadMutex.unlock();

		return;
	}

	//Move right
	if (_AllUserEvents._D_KeyPressed)
	{
		_OptionalThreadMutex.lock();
		_Player._Textures.mark_as_active(MOVING_RIGHT);
		_Player.set_vector_direction_to_right();
		_Player._IsMoving = true;
		_OptionalThreadMutex.unlock();

		return;
	}

	//Stand still
	_OptionalThreadMutex.lock();
	_Player._Textures.mark_as_active((_Player.tell_vector_x_direction() == -1 ? STANDING_LEFT : STANDING_RIGHT));
	_Player._IsMoving = false;
	_OptionalThreadMutex.unlock();

	return;
};

}
//GameConditions [end]