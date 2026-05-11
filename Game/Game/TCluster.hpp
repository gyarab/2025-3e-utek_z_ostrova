#ifndef TCLUSTER
#define TCLUSTER

//
#include "escape_from_the_island.hpp"

//Dynamic cluster [has two dimensions] that should contain textures that are related to each other while could be splitted into multiple sub-clusters at once
struct TCluster
{
	//Main container that hold all the textures in separated arrays usually of similiar ones
	std::vector<std::vector<SDL_Texture*>> _ClusterOfTextures;
	//Pointer that holds address to one specific array of textures from the main container
	std::vector<SDL_Texture*>* _ActiveSubcluster = nullptr;

	//Number that holds the position of the texture in _ActiveSubcluster thats is active now - makes it easy to iterate though them
	uint64_t _ActiveTexturePosition = NULL;
	//Pointer that holds address to one specific texture from the _ActiveSubcluster
	SDL_Texture** _ActiveTexture = nullptr;

public:
	//
	void prepare_active_pointers(void);
	//Marks specific sub-cluster [should be textures related to the same action] of textures as active
	void mark_as_active(const uint64_t _IndexOfSubcluster);
	//
	void switch_to_next(void);
	//
	void switch_to_specific(const uint64_t _SubindexOfTexture);
#if 0
	//Animates the specified sub-cluster by changing the active texture periodically with delay usually resulting in movement
	void animate_through_textures(const std::chrono::milliseconds _TextureUpdateDelay, std::mutex* _OptionalThreadMutex = nullptr);
#endif
	//Verifies if the main container [_ClusterOfTextures] is empty => the whole TCluster should be empty
	bool is_empty(void);

	//
	std::vector<SDL_Texture*>& operator[](const uint64_t _IndexOfSubcluster);
	//
	const std::vector<SDL_Texture*>& operator[](const uint64_t _IndexOfSubcluster) const;
	//
	SDL_Texture* operator()(const uint64_t _IndexOfSubcluster, const uint64_t _SubindexOfTexture);
	//
	const SDL_Texture* operator()(const uint64_t _IndexOfSubcluster, const uint64_t _SubindexOfTexture) const;
};

#endif