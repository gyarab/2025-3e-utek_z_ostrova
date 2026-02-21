#ifndef TCLUSTER
#define TCLUSTER

//
#include "escape_from_the_island.hpp"

//Add pointer safety!
//Dynamic cluster that should contain textures that are related to each other
typedef struct ClusterOfSimilarTextures
{
	std::vector<SDL_Texture*> _Textures;
}
TCluster;

//2D Dynamic cluster that should contain textures that are related to each other but could be splitted into multiple clusters at once
typedef struct MultipleClustersOfSimilarTextures
{
	std::vector<TCluster> _Textures;
}
TCluster_2D;

#endif