#include "escape_from_the_island.hpp"

//Function that loads the info about the 8 cfg files with entities config
static void LoadTextureCFG_FilesPropsIntoOneArray(std::array<std::string, 8> _AllCFG_Filenames)
{
	std::string Selectors[8] =
	{
		SELECTOR_tPlayer,
		SELECTOR_tBackground,
		SELECTOR_tBackgroundHitbox,
		SELECTOR_tMenuPanel,
		SELECTOR_tMenuButtons,
		SELECTOR_tFloatingStone,
		SELECTOR_tPoisonedArrow,
		SELECTOR_tStone
	};

	for (uint64_t c = NULL; c < 8; c++)
		ConfigFile::ReadValue(BUNDLE_AppEntityConfig, Selectors[c], _AllCFG_Filenames[c]);

	return;
};

namespace EntityBulkInitHandle //[start]
{

//Prepares all entites and puts them into the specified ECluster based on the config in the "config.ini" file
void PrepareECluster(ECluster& _Entities)
{
	std::array<std::string, 8> AllCFG_Filenames;
	LoadTextureCFG_FilesPropsIntoOneArray(AllCFG_Filenames);

	_Entities._ClusterOfEntities.resize(AllCFG_Filenames.size());

	for (uint64_t c = NULL; c < AllCFG_Filenames.size(); c++)
	{
		entity NewEntity;

		NewEntity.load_basic_info(AllCFG_Filenames[c]);
		_Entities._ClusterOfEntities.push_back(NewEntity);
	}

	return;
};

}
//EntityBulkInitHandle [end]