#include "escape_from_the_island.hpp"

//
enum TextureDB_Filenames : uint64_t
{
	PLAYER_TEXTURES = 0,
	BACKGROUND_TEXTURES = 1,
	MENU_PANEL_TEXTURES = 2,
	OBSTACLES_TEXTURES = 3
};

//Function that loads the basic info from config file on how textures should be processed
static INLINE void LoadBasicTextureProps(std::string& _GraphicsDir, std::string& _AssetConfigDir, std::string& _TextureFormat, int64_t& _ScalingCoefficient)
{
	ConfigFile::ReadValue(BUNDLE_AppAssetConfig, SELECTOR_tGraphicsDir, _GraphicsDir);
	ConfigFile::ReadValue(BUNDLE_AppAssetConfig, SELECTOR_tAssetConfigDir, _AssetConfigDir);
	ConfigFile::ReadValue(BUNDLE_AppAssetConfig, SELECTOR_tTextureFormat, _TextureFormat);
	ConfigFile::ReadValue(BUNDLE_AppWindowConfig, SELECTOR_ScalingCoefficient, _ScalingCoefficient);

	return;
};

//Function that loads the info about the 4 db files with textures config
static void LoadTextureDB_FilesPropsIntoOneArray(std::array<std::string, 4>& _AllDB_Filenames)
{
	std::string Selectors[4] =
	{
		SELECTOR_tPlayerTextures,
		SELECTOR_tBackgroundTextures,
		SELECTOR_tMenuPanelTextures,
		SELECTOR_tObstaclesTextures
	};

	for (uint64_t c = NULL; c < 4; c++)
		ConfigFile::ReadValue(BUNDLE_AppAssetConfig, Selectors[c], _AllDB_Filenames[c]);
	
	return;
};
 
//Extracts PNG into surface then [optionally] scales it by a coefficient and then converts it to a render-able texture
static SDL_Texture* MakeScaledTextureFromPNG(SDL_Renderer* const _TextureRenderer, const std::string& _Filename, const uint64_t _ScalingCoefficient)
{
	SDL_IOStream* PNG_File = SDL_IOFromFile(_Filename.c_str(), "r");

	if (PNG_File == nullptr)
		ErrorHandle::ReportSDL(true);

	SDL_Surface* ExtractedData = IMG_LoadPNG_IO(PNG_File);

	if (ExtractedData == nullptr)
	{
		ErrorHandle::ReportSDL(true);
		std::exit(-1); //Temp solution for the compiler warning that was produced here, remove if possible!
	}

	SDL_Surface* ScaledExtractedData = SDL_ScaleSurface(ExtractedData, (int32_t)_ScalingCoefficient * ExtractedData->w, (int32_t)_ScalingCoefficient * ExtractedData->h, SDL_SCALEMODE_NEAREST);

	if (ScaledExtractedData == nullptr)
		ErrorHandle::ReportSDL(true);

	SDL_Texture* FinishedTexture = SDL_CreateTextureFromSurface(_TextureRenderer, ScaledExtractedData);

	if (FinishedTexture == nullptr)
		ErrorHandle::ReportSDL(true);

	if (!SDL_CloseIO(PNG_File))
		ErrorHandle::ReportSDL(true);
		
	SDL_DestroySurface(ScaledExtractedData);
	SDL_DestroySurface(ExtractedData);

	return FinishedTexture;
};

//Checks if the specified number is in the set of numbers or no
static INLINE bool NumberIsInSet(const uint64_t _Number, const std::set<uint64_t>& _Set)
{
	for (const uint64_t Element : _Set)
		if (Element == _Number) return true;

	return false;
};

//Takes string that should contain only numbers separated with commas, extracts them and puts them in a set of numbers
static std::set<uint64_t> MakeSetOfNumbersFromCommaSeparatedString(const std::string& _BaseString)
{
	std::set<uint64_t> ResultSetOfNumbers;
	std::stringstream BaseStringStream(_BaseString);

	for (std::string OneNumber; std::getline(BaseStringStream, OneNumber, ','); )
		ResultSetOfNumbers.insert(std::stoull(OneNumber));

	return ResultSetOfNumbers;
};

namespace TextureHandle //[start]
{

//Maybe remove!
//Puts a pixels of default texture into a specified texture
void PutDefaultTexture(SDL_Renderer* const _TextureRenderer, SDL_Texture*& _Texture)
{
	
};

//Loads and prepares of the textures from image files based on the filenames specified in a db file and makes a TCluster from them
TCluster LoadFromFiles(SDL_Renderer* const _TextureRenderer, const std::string& _TexturesDB_Filename)
{
	TCluster ResultTextures;
	std::string GraphicsDir, AssetConfigDir, TextureFormat, TextureFilename, OneLine;
	int64_t ScalingCoefficient = NULL;

	LoadBasicTextureProps(GraphicsDir, AssetConfigDir, TextureFormat, ScalingCoefficient);

	const std::string FullGraphicsDir = SDL_GetBasePath() + GraphicsDir + "\\";
	const std::string FullAssetConfigDir = SDL_GetBasePath() + AssetConfigDir + "\\";
	std::fstream TexturesDB(FullAssetConfigDir + _TexturesDB_Filename, std::ios::in);

	if (TexturesDB.fail())
		RuntimeLog::Message(WARNING, "could not find/open texture file {\"" + _TexturesDB_Filename + "\"} => loading default textures instead");

	//First two lines has to hold TextureCount && WhenCreateNewCluster info only
	std::getline(TexturesDB, OneLine);
	const uint64_t TextureCount = std::stoull(OneLine);
	std::getline(TexturesDB, OneLine);
	const std::set<uint64_t> WhenCreateNewCluster = MakeSetOfNumbersFromCommaSeparatedString(OneLine);
	
	for (uint64_t c = NULL; c < TextureCount /*&& !TexturesDB.eof()*/; c++)
	{
		//Checks if new TCluster need to be created on this index and does it if needed
		if (NumberIsInSet(c, WhenCreateNewCluster))
			ResultTextures._ClusterOfTextures.push_back(std::vector<SDL_Texture*>());

		//Loads one file from the db file - there should be one valid texture filename
		std::getline(TexturesDB, TextureFilename);
		//makes texture from that file and adds to the TCluster - it may fail theres an invalid filename or the file is corrupted
		ResultTextures[ResultTextures._ClusterOfTextures.size() - 1].push_back(
			MakeScaledTextureFromPNG(_TextureRenderer, FullGraphicsDir + TextureFilename + "." + TextureFormat, ScalingCoefficient)
		);
	}

	RuntimeLog::Message(INFO, "all {" + std::to_string(TextureCount) + "} textures from {" + _TexturesDB_Filename + "} loaded successfully");

	return ResultTextures;
};

//Function that loas the texture filenames from db files and call the function that loads them - this function may be removed soon!
void PrepareForAllEntities(WRCluster& _MainWindow, ECluster& _AllEntities)
{
	SDL_Renderer* TextureRenderer = _MainWindow._Renderers[TEXTURE_RENDERER];
	std::array<std::string, 4> AllDB_Filenames;
	LoadTextureDB_FilesPropsIntoOneArray(AllDB_Filenames);

	//Loads the textures in order based on the db files
	_AllEntities[PLAYER]._Textures = TextureHandle::LoadFromFiles(TextureRenderer, AllDB_Filenames[PLAYER_TEXTURES]);
	_AllEntities[BACKGROUND]._Textures = TextureHandle::LoadFromFiles(TextureRenderer, AllDB_Filenames[BACKGROUND_TEXTURES]);
	_AllEntities[MENU_PANEL]._Textures = TextureHandle::LoadFromFiles(TextureRenderer, AllDB_Filenames[MENU_PANEL_TEXTURES]);
	_AllEntities[FLOATING_STONE]._Textures = TextureHandle::LoadFromFiles(TextureRenderer, AllDB_Filenames[OBSTACLES_TEXTURES]);

	//Some entities share the same base texture - theyll be just copied
	_AllEntities[MENU_PANEL].copy_textures_to(_AllEntities[MENU_BUTTONS]);
	_AllEntities[FLOATING_STONE].copy_textures_to(_AllEntities[POISONED_ARROW]);
	_AllEntities[FLOATING_STONE].copy_textures_to(_AllEntities[STONE]);

	//BackgroundHitbox has no textures
	_AllEntities[BACKGROUND_HITBOX]._Textures = TCluster();

	return;
};

//Function that safely removes texture from selected cluster
void SafelyRemoveTextureFromCluster(TCluster& _TextureCluster, const uint64_t _Index, const uint64_t _Subindex)
{
	SDL_DestroyTexture(_TextureCluster(_Index, _Subindex));
	_TextureCluster[_Index].erase(_TextureCluster[_Index].begin() + _Index);

	return;
};

}
//TextureHandle [end]