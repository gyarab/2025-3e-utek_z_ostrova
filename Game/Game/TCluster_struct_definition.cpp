#include "escape_from_the_island.hpp"

//
void TCluster::prepare_active_pointers(void)
{
	if (this->is_empty())
		return; //Nothing to point at

	this->_ActiveSubcluster = (this->_ClusterOfTextures.data() + 0);
	this->_ActiveTexture = (this->_ActiveSubcluster->data() + 0);
	this->_ActiveTexturePosition = 0;

	return;
};

//Marks specific sub-cluster [should be textures related to same action] of textures as active
void TCluster::mark_as_active(const uint64_t _IndexOfSubcluster)
{
	if (_IndexOfSubcluster < 0 || _IndexOfSubcluster >= _ClusterOfTextures.size())
		return; //Error

	if (this->_ActiveSubcluster == (this->_ClusterOfTextures.data() + _IndexOfSubcluster))
		return; //Active subcluster is already set the one that was asked for

	//
	this->_ActiveSubcluster = (this->_ClusterOfTextures.data() + _IndexOfSubcluster);
	this->_ActiveTexture = (this->_ActiveSubcluster->data() + 0);
	this->_ActiveTexturePosition = 0;

	return;
};

//
void TCluster::switch_to_next(void)
{
	//
	this->_ActiveTexturePosition >= this->_ActiveSubcluster->size() - 1 ? 
		this->_ActiveTexturePosition = 0 : this->_ActiveTexturePosition++;

	this->_ActiveTexture = (this->_ActiveSubcluster->data() + this->_ActiveTexturePosition);

	return;
};

//
void TCluster::switch_to_specific(const uint64_t _SubindexOfTexture)
{
	if (_SubindexOfTexture < 0 || _SubindexOfTexture >= this->_ActiveSubcluster->size())
		return; //Error

	//
	this->_ActiveTexture = (this->_ActiveSubcluster->data() + _SubindexOfTexture);
	this->_ActiveTexturePosition = _SubindexOfTexture;

	return;
};

#if 0
//Animates the specified sub-cluster by changing the active texture periodically with delay usually resulting in movement
void TCluster::animate_through_textures(const std::chrono::milliseconds _TextureUpdateDelay, std::mutex* _OptionalThreadMutex)
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
#endif

//Verifies if the main container [_ClusterOfTextures] is empty => the whole TCluster should be empty
bool TCluster::is_empty(void)
{
	return (this->_ClusterOfTextures.size() == 0);
};

//
std::vector<SDL_Texture*>& TCluster::operator[](const uint64_t _IndexOfSubcluster)
{
	if (_IndexOfSubcluster < 0 || _IndexOfSubcluster >= this->_ClusterOfTextures.size())
		std::exit(-1); //Error

	return this->_ClusterOfTextures[_IndexOfSubcluster];

};

//
const std::vector<SDL_Texture*>& TCluster::operator[](const uint64_t _IndexOfSubcluster) const
{
	if (_IndexOfSubcluster < 0 || _IndexOfSubcluster >= this->_ClusterOfTextures.size())
		std::exit(-1); //Error

	return this->_ClusterOfTextures[_IndexOfSubcluster];
};

//
SDL_Texture* TCluster::operator()(const uint64_t _IndexOfSubcluster, const uint64_t _SubindexOfTexture)
{
	if (_IndexOfSubcluster < 0 || _IndexOfSubcluster >= this->_ClusterOfTextures.size())
		std::exit(-1); //Error

	if (_SubindexOfTexture < 0 || _SubindexOfTexture >= this->_ClusterOfTextures[_IndexOfSubcluster].size())
		std::exit(-1); //Error

	return this->_ClusterOfTextures[_IndexOfSubcluster][_SubindexOfTexture];
};

//
const SDL_Texture* TCluster::operator()(const uint64_t _IndexOfSubcluster, const uint64_t _SubindexOfTexture) const
{
	if (_IndexOfSubcluster < 0 || _IndexOfSubcluster >= this->_ClusterOfTextures.size())
		std::exit(-1); //Error

	if (_SubindexOfTexture < 0 || _SubindexOfTexture >= this->_ClusterOfTextures[_IndexOfSubcluster].size())
		std::exit(-1); //Error

	return this->_ClusterOfTextures[_IndexOfSubcluster][_SubindexOfTexture];
};