#include "escape_from_the_island.hpp"

namespace BackgroundThread //[start]
{

//
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
			//
			std::this_thread::sleep_for((std::chrono::milliseconds)_TextureUpdateDelay);
		}
	}

	return;
};

};
//BackgroundThread [end]