#ifndef TCLUSTER
#define TCLUSTER

//
#include "escape_from_the_island.hpp"

//Dynamic cluster [has two dimensions] that should contain textures that are related to each other while could be splitted into multiple sub-clusters at once
struct TCluster
{
	std::vector<std::vector<SDL_Texture*>> _ClusterOfTextures;
	std::vector<SDL_Texture*>* _ActiveSubcluster;
	SDL_Texture** _ActiveTexture;

public:
	//Mark specifis sub-cluster [should be textures related to same action] of textures as active
	INLINE void mark_as_active(const uint64_t _IndexofElement);
	//Animates the specified sub-cluster by changing the active texture periodically with delay usually resulting in movement
	INLINE void animate_through_textures(const std::chrono::milliseconds _TextureUpdateDelay, std::mutex* _OptionalThreadMutex = nullptr);
};

#endif