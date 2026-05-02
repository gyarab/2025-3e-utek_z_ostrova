#include "escape_from_the_island.hpp"

//Function that loads the basic info from config file on how textures should be processed
static INLINE void LoadBasicTextureProps(std::string& _GraphicsDir, std::string& _TextureFormat, int64_t& _TextureScalingCoefficient)
{
	ConfigFile::ReadValue(BUNDLE_AppAssetConfig, SELECTOR_tGraphicsDir, _GraphicsDir);
	ConfigFile::ReadValue(BUNDLE_AppAssetConfig, SELECTOR_tTextureFormat, _TextureFormat);
	ConfigFile::ReadValue(BUNDLE_AppWindowConfig, SELECTOR_TextureScalingCoefficient, _TextureScalingCoefficient);

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

namespace TextureHandle //[start]
{

//Puts a pixels of default texture into a specified texture
void PutDefaultTexture(SDL_Renderer* const _TextureRenderer, SDL_Texture*& _Texture)
{
	
};

//Loads and prepares of the textures from image files based on the filenames specified in a db file and makes a 2D TCluster from them
TCluster LoadFromFiles(SDL_Renderer* const _TextureRenderer, const std::string& _TexturesDB_Filename, const uint64_t _TextureCount, const std::set<uint64_t>& _WhenCreateNewCluster)
{
	TCluster ResultTextures;
	std::string GraphicsDir, TextureFormat, TextureFilename;
	int64_t TextureScalingCoefficient = NULL;

	LoadBasicTextureProps(GraphicsDir, TextureFormat, TextureScalingCoefficient);

	const std::string FullGraphicsDir = SDL_GetBasePath() + GraphicsDir + "\\";
	std::fstream TexturesDB(FullGraphicsDir + _TexturesDB_Filename, std::ios::in);

	if (TexturesDB.fail())
		RuntimeLog::Message(WARNING, "could not find/open texture file {\"" + _TexturesDB_Filename + "\"} => loading default textures instead");



	for (uint64_t c = NULL; c < _TextureCount /*&& !TexturesDB.eof()*/; c++)
	{
		//Checks if new TCluster need to be created on this index and does it if needed
		if (NumberIsInSet(c, _WhenCreateNewCluster))
			ResultTextures._ClusterOfTextures.push_back(std::vector<SDL_Texture*>());

		//Loads one file from the db file - there should be one valid texture filename
		std::getline(TexturesDB, TextureFilename);
		//makes texture from that file and adds to the TCluster - it may fail theres an invalid filename or the file is corrupted
		ResultTextures[ResultTextures._ClusterOfTextures.size() - 1].push_back(
			MakeScaledTextureFromPNG(_TextureRenderer, FullGraphicsDir + TextureFilename + "." + TextureFormat, TextureScalingCoefficient)
		);
	}

	RuntimeLog::Message(INFO, "all {" + std::to_string(_TextureCount) + "} textures from {" + _TexturesDB_Filename + "} loaded successfully");

	return ResultTextures;
};

//Function that loas the texture filenames from db files and call the function that loads them - this function may be removed soon!
void PrepareAllNeeded(SDL_Renderer* const _TextureRenderer, TCluster& _PlayerTextures, TCluster& _Level1Textures)
{
	std::string PlayerTextures, Level1Textures;

	ConfigFile::ReadValue(BUNDLE_AppAssetConfig, SELECTOR_tPlayerTextures, PlayerTextures);
	ConfigFile::ReadValue(BUNDLE_AppAssetConfig, SELECTOR_tLevel1Textures, Level1Textures);
	//Loads the textures in order based on the db files
	_PlayerTextures = TextureHandle::LoadFromFiles(_TextureRenderer, PlayerTextures, 12, { 0, 4, 8, 10 }); //move the numeric values to the db file too!
	_Level1Textures = TextureHandle::LoadFromFiles(_TextureRenderer, Level1Textures, 3, { 0 }); //move the numeric values to the db file too!

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