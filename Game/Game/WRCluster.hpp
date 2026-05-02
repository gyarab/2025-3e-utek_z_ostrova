#ifndef RCLUSTER
#define RCLUSTER

//
#include "escape_from_the_island.hpp"

//Maybe add a dynamic array of windows too!
//Dynamic cluster that should hold one window and its own corresponding renderer/s
struct WRCluster
{
	SDL_Window* _Window;
	std::vector<SDL_Renderer*> _Renderers;
};

#endif