#include "escape_from_the_island.hpp"

namespace TextureOperators //[start]
{

//Extracts PNG into surface then [optionally] scales it by a coefficient and then converts it to a render-able texture
SDL_Texture* MakeScaledTextureFromPNG(SDL_Renderer* const _TextureRenderer, const std::string& _Filename, const uint64_t _ScalingCoefficient)
{
	SDL_IOStream* PNG_File = SDL_IOFromFile(_Filename.c_str(), "r");
	SDL_Surface* ExtractedData = IMG_LoadPNG_IO(PNG_File);
	SDL_Surface* ScaledExtractedData = SDL_ScaleSurface(ExtractedData, (int32_t)_ScalingCoefficient * ExtractedData->w, (int32_t)_ScalingCoefficient * ExtractedData->h, SDL_SCALEMODE_NEAREST);
	SDL_Texture* FinishedTexture = SDL_CreateTextureFromSurface(_TextureRenderer, ScaledExtractedData);

	SDL_CloseIO(PNG_File);
	SDL_DestroySurface(ScaledExtractedData);
	SDL_DestroySurface(ExtractedData);

	return FinishedTexture;
};

//Function that safely removes texture from selected cluster
void SafelyRemoveTextureFromCluster(TCluster& _TextureCluster, const uint64_t _Index)
{
	SDL_DestroyTexture(_TextureCluster._Textures[_Index]);
	_TextureCluster._Textures.erase(_TextureCluster._Textures.begin() + _Index);

	return;
};

//Change to dynamic path loaded from .ini file!
struct PNG_FilePaths
{
	const std::string _AppPath = SDL_GetBasePath();

	const std::array<std::string, 12> _FilePaths =
	{
		"graphics\\character_running_facing_left_1.png",
		"graphics\\character_running_facing_left_2.png",
		"graphics\\character_running_facing_left_3.png",
		"graphics\\character_running_facing_left_4.png",
		"graphics\\character_running_facing_right_1.png",
		"graphics\\character_running_facing_right_2.png",
		"graphics\\character_running_facing_right_3.png",
		"graphics\\character_running_facing_right_4.png",
		"graphics\\character_standing_facing_left_1.png",
		"graphics\\character_standing_facing_left_2.png",
		"graphics\\character_standing_facing_right_1.png",
		"graphics\\character_standing_facing_right_2.png"
	};
};

//make better loops!
//Fills the four TClusters with textures from .png files
void PrepareAllPlayerAnimationTextures(SDL_Renderer* const _TextureRenderer, std::array<TCluster, 4>& _PlayerAnimationTClusters, const uint64_t _ScalingCoeficient)
{
	PNG_FilePaths PNGs = PNG_FilePaths();

	for (uint64_t c = 0; c <= 3; c++)
		_PlayerAnimationTClusters[RUNNING_FACING_LEFT]._Textures.emplace_back(TextureOperators::MakeScaledTextureFromPNG(_TextureRenderer, PNGs._AppPath + PNGs._FilePaths[c], _ScalingCoeficient));

	for (uint64_t c = 4; c <= 7; c++)
		_PlayerAnimationTClusters[RUNNING_FACING_RIGHT]._Textures.emplace_back(TextureOperators::MakeScaledTextureFromPNG(_TextureRenderer, PNGs._AppPath + PNGs._FilePaths[c], _ScalingCoeficient));

	for (uint64_t c = 8; c <= 9; c++)
		_PlayerAnimationTClusters[STANDING_FACING_LEFT]._Textures.emplace_back(TextureOperators::MakeScaledTextureFromPNG(_TextureRenderer, PNGs._AppPath + PNGs._FilePaths[c], _ScalingCoeficient));

	for (uint64_t c = 10; c <= 11; c++)
		_PlayerAnimationTClusters[STANDING_FACING_RIGHT]._Textures.emplace_back(TextureOperators::MakeScaledTextureFromPNG(_TextureRenderer, PNGs._AppPath + PNGs._FilePaths[c], _ScalingCoeficient));
};

}
//TextureOperators [end]