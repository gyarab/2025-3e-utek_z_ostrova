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
		_Player._Textures.mark_as_active(0);
		_Player._Vector[DX] = -1 * _Player._X_Movespeed;
		_Player._IsMoving = true;
		_OptionalThreadMutex.unlock();

		return;
	}

	//Move right
	if (_AllUserEvents._D_KeyPressed)
	{
		_OptionalThreadMutex.lock();
		_Player._Textures.mark_as_active(1);
		_Player._Vector[DX] = 1 * _Player._X_Movespeed;
		_Player._IsMoving = true;
		_OptionalThreadMutex.unlock();

		return;
	}

	//Stand still
	_OptionalThreadMutex.lock();
	_Player._Textures.mark_as_active((_Player._Vector[DX] / _Player._X_Movespeed == -1 ? 2 : 3));
	_Player._IsMoving = false;
	_OptionalThreadMutex.unlock();

	return;
};

}
//GameConditions [end]