#include "escape_from_the_island.hpp"

//
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
		std::exit(-1); //
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

//
static INLINE bool NumberIsInSet(const uint64_t _Number, const std::set<uint64_t>& _Set)
{
	for (const uint64_t Element : _Set)
		if (Element == _Number) return true;

	return false;
};

namespace TextureHandle //[start]
{

//
void PutDefaultTexture(SDL_Renderer* const _TextureRenderer, SDL_Texture*& _Texture)
{
	
};

//
TCluster_2D LoadFromFiles(SDL_Renderer* const _TextureRenderer, const std::string& _TexturesDB_Filename, const uint64_t _TextureCount, const std::set<uint64_t>& _WhenCreateNewCluster)
{
	TCluster_2D ResultTextureClusters;
	std::string GraphicsDir, TextureFormat, TextureFilename;
	int64_t TextureScalingCoefficient = NULL;

	LoadBasicTextureProps(GraphicsDir, TextureFormat, TextureScalingCoefficient);

	const std::string FullGraphicsDir = SDL_GetBasePath() + GraphicsDir + "\\";
	std::fstream TexturesDB(FullGraphicsDir + _TexturesDB_Filename, std::ios::in);

	if (TexturesDB.fail())
		RuntimeLog::Message(WARNING, "could not find/open texture file {\"" + _TexturesDB_Filename + "\"} => loading default textures instead");

	for (uint64_t c = NULL; c < _TextureCount /*&& !TexturesDB.eof()*/; c++)
	{
		//
		if (NumberIsInSet(c, _WhenCreateNewCluster))
			ResultTextureClusters._Textures.push_back(TCluster());

		//
		std::getline(TexturesDB, TextureFilename);
		//
		ResultTextureClusters._Textures[ResultTextureClusters._Textures.size() - 1]._Textures.push_back(
			MakeScaledTextureFromPNG(_TextureRenderer, FullGraphicsDir + TextureFilename + "." + TextureFormat, TextureScalingCoefficient)
		);
	}

	RuntimeLog::Message(INFO, "all {" + std::to_string(_TextureCount) + "} textures from {" + _TexturesDB_Filename + "} loaded successfully");

	return ResultTextureClusters;
};

//
void PrepareAllNeeded(SDL_Renderer* const _TextureRenderer, TCluster_2D& _PlayerTextures, TCluster_2D& _Level1Textures)
{
	std::string PlayerTextures, Level1Textures;

	ConfigFile::ReadValue(BUNDLE_AppAssetConfig, SELECTOR_tPlayerTextures, PlayerTextures);
	ConfigFile::ReadValue(BUNDLE_AppAssetConfig, SELECTOR_tLevel1Textures, Level1Textures);
	//
	_PlayerTextures = TextureHandle::LoadFromFiles(_TextureRenderer, PlayerTextures, 12, { 0, 4, 8, 10 }); //move the numeric values to the db file too!
	_Level1Textures = TextureHandle::LoadFromFiles(_TextureRenderer, Level1Textures, 3, { 0 }); //move the numeric values to the db file too!

	return;
};

//Function that safely removes texture from selected cluster
void SafelyRemoveTextureFromCluster(TCluster& _TextureCluster, const uint64_t _Index)
{
	SDL_DestroyTexture(_TextureCluster._Textures[_Index]);
	_TextureCluster._Textures.erase(_TextureCluster._Textures.begin() + _Index);

	return;
};

}
//TextureHandle [end]