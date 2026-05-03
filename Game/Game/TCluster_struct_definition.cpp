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

//Verifies if the main container [_ClusterOfTextures] is empty => the whole TCluster should be empty
INLINE bool TCluster::is_empty(void)
{
	return (this->_ClusterOfTextures.size() == 0);
};

//
INLINE std::vector<SDL_Texture*>& TCluster::operator[](const uint64_t _IndexOfSubcluster)
{
	if (_IndexOfSubcluster < 0 || _IndexOfSubcluster >= this->_ClusterOfTextures.size())
		return; //Error

	return this->_ClusterOfTextures[_IndexOfSubcluster];

};

//
INLINE const std::vector<SDL_Texture*>& TCluster::operator[](const uint64_t _IndexOfSubcluster) const
{
	if (_IndexOfSubcluster < 0 || _IndexOfSubcluster >= this->_ClusterOfTextures.size())
		return; //Error

	return this->_ClusterOfTextures[_IndexOfSubcluster];
};

//
INLINE SDL_Texture* TCluster::operator()(const uint64_t _IndexOfSubcluster, const uint64_t _SubindexOfTexture)
{
	if (_IndexOfSubcluster < 0 || _IndexOfSubcluster >= this->_ClusterOfTextures.size())
		return; //Error

	if (_SubindexOfTexture < 0 || _SubindexOfTexture >= this->_ClusterOfTextures[_IndexOfSubcluster].size())
		return; //Error

	return this->_ClusterOfTextures[_IndexOfSubcluster][_SubindexOfTexture];
};

//
INLINE const SDL_Texture* TCluster::operator()(const uint64_t _IndexOfSubcluster, const uint64_t _SubindexOfTexture) const
{
	if (_IndexOfSubcluster < 0 || _IndexOfSubcluster >= this->_ClusterOfTextures.size())
		return; //Error

	if (_SubindexOfTexture < 0 || _SubindexOfTexture >= this->_ClusterOfTextures[_IndexOfSubcluster].size())
		return; //Error

	return this->_ClusterOfTextures[_IndexOfSubcluster][_SubindexOfTexture];
};