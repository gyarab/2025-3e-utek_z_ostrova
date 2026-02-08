#include "escape_from_the_island.hpp"

//First game test - character animation ; left and right movement
//'WinMain' has to be used as entry point
int WinMain(int argc, char** argv)
{
	//Initialize library
	SDL_Init(SDL_INIT_VIDEO);
	
	//Initialize main variables
	SDL_Window* GameWindow = nullptr;
	constexpr int32_t Width = 1280, Length = 720; //16:9 HD 720p
	RCluster GameRenderers = RCluster();
	constexpr uint64_t CountOfGameRenderers = 1;
	constexpr uint64_t PlayerTextureScalingCoefficient = 4; //Scales default 32x32 textures to 128x128 textures
	
	//Initialize texture clusters
	std::array<TCluster, 4> PlayerAnimationTClusters =
	{
		TCluster(),
		TCluster(),
		TCluster(),
		TCluster()
	};

	//Prepare window and renderer for game loop
	GameLoop::PrepareGameWindowsAndRenderers(GameWindow, GameRenderers, Width, Length, CountOfGameRenderers);
	//Prepare textures
	TextureOperators::PrepareAllPlayerAnimationTextures(GameRenderers._Renderers[TEXTURE_RENDERER], PlayerAnimationTClusters, PlayerTextureScalingCoefficient);
	//Game starts
	GameLoop::MainLoop(GameRenderers._Renderers[TEXTURE_RENDERER], PlayerTextureScalingCoefficient, PlayerAnimationTClusters);
	//Destroy game window and renderer before closing program
	GameLoop::DestroyGameWindowAndRenderers(GameWindow, GameRenderers);

	//Quit the library and close program
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();

	return EXIT_SUCCESS;
};