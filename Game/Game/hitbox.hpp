#ifndef HITBOX
#define HITBOX

//
#include "escape_from_the_island.hpp"

//Temp! Redo!
//
struct hitbox
{
	//
	bool _IsActive = false;

	//
	SDL_FRect _Hitbox = SDL_FRect();

public:
	//
	void try_highlight_itself(SDL_Renderer*& _TextureRenderer, const bool _F1_KeyPressed);
};

//Temp! Redo!
struct HCluster
{
	std::vector<hitbox> _ClusterOfHitboxes;

public:
	//
	void try_highlight_all(SDL_Renderer*& _TextureRenderer, const bool _F1_KeyPressed);

	//
	hitbox& operator[](const uint64_t _IndexOfHitbox);
	//
	const hitbox& operator[](const uint64_t _IndexOfHitbox) const;
};


#endif