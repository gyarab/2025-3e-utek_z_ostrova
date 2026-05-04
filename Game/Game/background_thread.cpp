#include "escape_from_the_island.hpp"

namespace BackgroundThread //[start]
{

//Thread that animates the water on the background by animating its textures
void Main(entity* _Background, const std::chrono::milliseconds _TextureUpdateDelay, std::atomic_bool* const _ThreadShouldFinish)
{
	//Mutex to safely operate with critical sector from 'BackgroundThread'
	std::mutex MutexForBackgroundThread;

	//main thread loop
	while (!*_ThreadShouldFinish)
	{
		if (_Background->_Textures._ActiveSubcluster == nullptr)
			continue; //make as error!

		_Background->_Textures.animate_through_textures(_TextureUpdateDelay, &MutexForBackgroundThread);
	}

	return;
};

};
//BackgroundThread [end]