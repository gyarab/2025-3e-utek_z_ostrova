#include "escape_from_the_island.hpp"

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
};

namespace PlayerThread //[start]
{

//Function used to initialize Entity container with 'default' values - should be equal to '_ScalingCoefficient'
Entity PutDefaultValues(void)
{
	int64_t ScalingCoefficient = NULL;
	ConfigFile::ReadValue(BUNDLE_AppWindowConfig, SELECTOR_TextureScalingCoefficient, ScalingCoefficient);

	return 
		Entity(LEFT, false, SDL_FRect(ENTITY_PLAYER_DEFAULT_SCREEN_POSITION, ENTITY_PLAYER_DEFAULT_SCREEN_POSITION, ScalingCoefficient * ENTITY_PLAYER_DEFAULT_SIZE, ScalingCoefficient * ENTITY_PLAYER_DEFAULT_SIZE));
};

//Thread that animates a set cluster of textures by continuously selecting them in order [first->last] with delay
void Main(SDL_Texture** _DisplayedTexture, TCluster** const _TexturesToAnimate, const uint64_t _TextureUpdateDelay, Entity* const _Player, std::atomic_bool* const _AnimationInterrupted, std::atomic_bool* const _ThreadShouldFinish)
{
	//Mutex to safely operate with critical sector from 'AnimatePlayerTextureClusterThread'
	std::mutex MutexForPlayerThread;
	//Dynamic thread waiting method using mutex-lock with condition variable - waiting can be interrupted even before it ends
	std::mutex WaitingMutex;
	std::unique_lock<std::mutex> WaitingLock(WaitingMutex);
	std::condition_variable WaitingCondition;

	//main thread loop
	while (!*_ThreadShouldFinish)
	{
		if (*_TexturesToAnimate == nullptr || *_AnimationInterrupted == true)
			continue;

		for (uint64_t c = NULL; c < (*_TexturesToAnimate)->_Textures.size(); c++)
		{
			ChangePlayerCoords(*_Player);
			MutexForPlayerThread.lock();
			*_DisplayedTexture = (*_TexturesToAnimate)->_Textures[c];
			MutexForPlayerThread.unlock();
			//This ensures that only at maximum ~3 frames will be animated in a second [max. ~3FPS], resulting in smooth animation
			WaitingCondition.wait_for(WaitingLock, (std::chrono::milliseconds)_TextureUpdateDelay);
			
			if (*_AnimationInterrupted == true)
				break;
		}
	}

	return;
};

}
//PlayerThread [end]