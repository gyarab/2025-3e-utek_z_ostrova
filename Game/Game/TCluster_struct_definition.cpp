#include "escape_from_the_island.hpp"

//Mark specifis sub-cluster [should be textures related to same action] of textures as active
INLINE void TCluster::mark_as_active(const uint64_t _IndexofElement)
{
	if (_IndexofElement < 0 || _IndexofElement >= _ClusterOfTextures.size())
		return; //Error

	_ActiveSubcluster = (_ClusterOfTextures.data() + _IndexofElement);

	return;
};

//Animates the specified sub-cluster by changing the active texture periodically with delay usually resulting in movement
INLINE void TCluster::animate_through_textures(const std::chrono::milliseconds _TextureUpdateDelay, std::mutex* _OptionalThreadMutex = nullptr)
{
	for (uint64_t c = NULL; c < this->_ActiveSubcluster->size(); c++)
	{
		_OptionalThreadMutex->lock();
		this->_ActiveTexture = (this->_ActiveSubcluster->data() + c);
		_OptionalThreadMutex->unlock();
		std::this_thread::sleep_for(_TextureUpdateDelay);
	}

	return;
};
