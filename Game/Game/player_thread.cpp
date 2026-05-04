#include "escape_from_the_island.hpp"

#if 0 
//Function that changes player's coords based on its current status
static INLINE void ChangePlayerCoords(Entity& _Player)
{
	if (_Player._IsRunning)
		switch (_Player._Facing)
		{
		case LEFT:
			if (_Player._PositionAndSize.x <= 0.0f)
				_Player._PositionAndSize.x = 1280.0f - 128.0f;

			_Player._PositionAndSize.x -= 20.0f;

			break;

		case RIGHT:
			if (_Player._PositionAndSize.x >= 1280.0f - 128.0f)
				_Player._PositionAndSize.x = 0.0f;

			_Player._PositionAndSize.x += 20.0f;

			break;

		default:
			break;
		}

	return;
};
#endif

namespace PlayerThread //[start]
{
#if 0
//Function used to initialize Entity container for player with 'default' values
Entity PutDefaultValues(void)
{
	int64_t ScalingCoefficient = NULL;
	ConfigFile::ReadValue(BUNDLE_AppWindowConfig, SELECTOR_TextureScalingCoefficient, ScalingCoefficient);

	return 
		Entity(LEFT, false, SDL_FRect(ENTITY_PLAYER_DEFAULT_SCREEN_POSITION, ENTITY_PLAYER_DEFAULT_SCREEN_POSITION, ScalingCoefficient * ENTITY_PLAYER_DEFAULT_SIZE, ScalingCoefficient * ENTITY_PLAYER_DEFAULT_SIZE));
};
#endif

//Thread that makes player move by changing its horizontal coords and animating its textures
#if 0
void Main(SDL_Texture** _DisplayedTexture, TCluster** const _TexturesToAnimate, const std::chrono::milliseconds _TextureUpdateDelay, Entity* const _Player, std::atomic_bool* const _ThreadShouldFinish)
#endif
void Main(entity* _Player, const std::chrono::milliseconds _TextureUpdateDelay, std::atomic_bool* const _ThreadShouldFinish)
{
	//Mutex to safely operate with critical sector from 'PlayerThread'
	std::mutex MutexForPlayerThread;

	//main thread loop
	while (!*_ThreadShouldFinish)
	{
		if (_Player->_Textures._ActiveSubcluster == nullptr)
			continue; //make as error!

		_Player->make_movement_while_animating(_TextureUpdateDelay, &MutexForPlayerThread);
	}

	return;
};

}
//PlayerThread [end]