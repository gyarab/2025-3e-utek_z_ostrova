#include "escape_from_the_island.hpp"

namespace BackgroundThread //[start]
{

//Thread that animates the water on the background by animating its textures
void Main(SDL_Texture** _DisplayedTexture, TCluster** const _TexturesToAnimate, const uint64_t _TextureUpdateDelay, std::atomic_bool* const _ThreadShouldFinish)
{
	//Mutex to safely operate with critical sector from 'BackgroundThread'
	std::mutex MutexForBackgroundThread;

	//main thread loop
	while (!*_ThreadShouldFinish)
	{
		if (*_TexturesToAnimate == nullptr)
			continue; //make as error!

		for (uint64_t c = NULL; c < (*_TexturesToAnimate)->_Textures.size(); c++)
		{
			MutexForBackgroundThread.lock();
			*_DisplayedTexture = (*_TexturesToAnimate)->_Textures[c];
			MutexForBackgroundThread.unlock();
			//This ensures that only at maximum ~3 frames will be animated in a second [max. ~3FPS], resulting in smooth animation
			std::this_thread::sleep_for((std::chrono::milliseconds)_TextureUpdateDelay);
		}
	}

	return;
};

};
//BackgroundThread [end]