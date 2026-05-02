#ifndef ECLUSTER
#define ECLUSTER

//
#include "escape_from_the_island.hpp"

//Dynamic cluster that can hold many entities at once
struct ECluster
{
	std::vector<entity> _ClusterOfEntities;

public:
	//
	INLINE entity& operator[](const uint64_t _IndexOfEntity);
	//
	INLINE const entity& operator[](const uint64_t _IndexOfEntity) const;
};

#endif