#include "game.hpp"

namespace CharacterAnimation //[start]
{

//Sets the color that will be used that the beginning of every frame to black - should be called only once at the beginning of program
void SetFrameDefaultColorToBlack(SDL_Renderer* const _TextureRenderer)
{
	SDL_SetRenderDrawColor(_TextureRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

	return;
};

//Function used to initialize Entity container with 'default' values - should be equal to '_ScalingCoefficient'
Entity PutDefaultValuesForPlayer(const uint64_t _ScalingCoefficient)
{
	return Entity(LEFT, false, SDL_FRect(ENTITY_PLAYER_DEFAULT_SCREEN_POSITION, ENTITY_PLAYER_DEFAULT_SCREEN_POSITION, _ScalingCoefficient * ENTITY_PLAYER_DEFAULT_SIZE, _ScalingCoefficient * ENTITY_PLAYER_DEFAULT_SIZE));
};

//Thread that animates a set cluster of textures by continuously selecting them in order [first->last] with delay
void AnimatePlayerTextureClusterThreadMain(SDL_Texture** _DisplayedTexture, TCluster** const _TexturesToAnimate, const uint64_t _TextureUpdateDelay, std::atomic_bool* const _AnimationInterrupted, std::atomic_bool* const _ThreadShouldFinish)
{
	//Mutex to safely operate with critical sector from 'AnimatePlayerTextureClusterThread'
	std::mutex MutexForAnimatePlayerTextureClusterThread;
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
			MutexForAnimatePlayerTextureClusterThread.lock();
			*_DisplayedTexture = (*_TexturesToAnimate)->_Textures[c];
			MutexForAnimatePlayerTextureClusterThread.unlock();
			//This ensures that only at maximum ~3 frames will be animated in a second [max. ~3FPS], resulting in smooth animation
			WaitingCondition.wait_for(WaitingLock, (std::chrono::milliseconds)_TextureUpdateDelay);

			if (*_AnimationInterrupted == true)
				break;
		}
	}

	return;
};

//Thread that constantly updates the screen with an animation of player's character + it can also change the player's coords
void FrameRenderThreadMain(SDL_Renderer* const _TextureRenderer, Entity* const _Player, SDL_Texture** const _TextureToRender, std::atomic_bool* const _ThreadShouldFinish)
{
	//main thread loop
	while (!*_ThreadShouldFinish)
	{
		if (*_TextureToRender == nullptr)
			continue;

		//Transform into separate inline function!
		//Changing player's coords based on its current status
		if (_Player->_IsRunning)
			switch (_Player->_Facing)
			{
			case LEFT:
				if (_Player->_PositionAndSize.x <= 0.0f)
					_Player->_PositionAndSize.x = 1280.0f - 128.0f;

				_Player->_PositionAndSize.x -= 0.2f;

				break;

			case RIGHT:
				if (_Player->_PositionAndSize.x >= 1280.0f - 128.0f)
					_Player->_PositionAndSize.x = 0.0f;

				_Player->_PositionAndSize.x += 0.2f;

				break;

			default:
				break;
			}

		SDL_RenderClear(_TextureRenderer);
		SDL_RenderTexture(_TextureRenderer, *_TextureToRender, NULL, &_Player->_PositionAndSize);
		SDL_RenderPresent(_TextureRenderer);
		//This ensures that only at maximum ~1000 frames will be rendered in a second [max. ~1000FPS] <- More is not needed
		std::this_thread::sleep_for((std::chrono::milliseconds)1);
	}

	return;
};

}
//CharacterAnimation [end]