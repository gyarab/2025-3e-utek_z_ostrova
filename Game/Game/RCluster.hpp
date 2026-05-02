#ifndef RCLUSTER
#define RCLUSTER

//
#include "escape_from_the_island.hpp"

//Add pointer safety!
//Dynamic cluster that should contain renderers that are related to each other
struct RCluster
{
	std::vector<SDL_Renderer*> _Renderers;
};

#endif