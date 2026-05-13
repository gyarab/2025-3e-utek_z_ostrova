#include "escape_from_the_island.hpp"

//
void hitbox::try_highlight_itself(SDL_Renderer*& _TextureRenderer, const bool _F1_KeyPressed)
{
	if (!this->_IsActive)
		return; //

	//Temp!
	if (!_F1_KeyPressed)
		return;

	//Hitbox rectangle
	SDL_SetRenderDrawColor(_TextureRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderRect(_TextureRenderer, &this->_Hitbox);
	//Hitbox diagonals
	SDL_SetRenderDrawColor(_TextureRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderLine(_TextureRenderer, this->_Hitbox.x, this->_Hitbox.y, this->_Hitbox.x + this->_Hitbox.w, this->_Hitbox.y + this->_Hitbox.h);
	SDL_RenderLine(_TextureRenderer, this->_Hitbox.x + this->_Hitbox.w, this->_Hitbox.y, this->_Hitbox.x, this->_Hitbox.y + this->_Hitbox.h);

	//Hitbox vector - doesnt exist there!!
	//
	SDL_SetRenderDrawColor(_TextureRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

	return;
};

//
void HCluster::try_highlight_all(SDL_Renderer*& _TextureRenderer, const bool _F1_KeyPressed)
{
	//
	for (hitbox& OneHitbox : this->_ClusterOfHitboxes)
		OneHitbox.try_highlight_itself(_TextureRenderer, _F1_KeyPressed);

	return;
};

//
hitbox& HCluster::operator[](const uint64_t _IndexOfHitbox)
{
	if (_IndexOfHitbox < 0 || _IndexOfHitbox >= this->_ClusterOfHitboxes.size())
		std::exit(-1); //Error

	return this->_ClusterOfHitboxes[_IndexOfHitbox];
};

//
const hitbox& HCluster::operator[](const uint64_t _IndexOfHitbox) const
{
	if (_IndexOfHitbox < 0 || _IndexOfHitbox >= this->_ClusterOfHitboxes.size())
		std::exit(-1); //Error

	return this->_ClusterOfHitboxes[_IndexOfHitbox];
};