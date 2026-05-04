#include "escape_from_the_island.hpp"

namespace PlayerThread //[start]
{

//Thread that makes player move by changing its horizontal coords and animating its textures
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