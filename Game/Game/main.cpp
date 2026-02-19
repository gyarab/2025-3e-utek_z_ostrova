#include "escape_from_the_island.hpp"

//First game test - character animation ; left and right movement
//'WinMain' has to be used as entry point
int WinMain(int argc, char** argv)
{
	//Creating log file
	RuntimeLog::CreateFile();

	//Initialize library - program gets terminated if this fails
	if (!SDL_Init(SDL_INIT_VIDEO))
		ErrorHandle::ReportSDL(true);
	
	RuntimeLog::Message(INFO, "SDL-lib successfully initialized"); //Add logs everywhere

	//Initialize main variables
	SDL_Window* GameWindow = nullptr;
	constexpr int32_t Width = 1280, Length = 720; //16:9 HD 720p
	RCluster GameRenderers = RCluster();
	constexpr uint64_t CountOfGameRenderers = 1;
	constexpr uint64_t PlayerTextureScalingCoefficient = 4; //Scales default 32x32 textures to 128x128 textures
	
	//Initialize texture clusters
	std::vector<TCluster> PlayerTClusters;
	std::vector<TCluster> Level1TClusters;

	//Prepare window and renderer for game loop
	WindowRenderHandle::CreateNewWindowWithRenderers(GameWindow, GameRenderers, CountOfGameRenderers);
	//Prepare textures
	TextureHandle::PrepareAllNeeded(GameRenderers._Renderers[TEXTURE_RENDERER], PlayerTClusters, Level1TClusters);
	//Game starts
	GameLoopThread::MainLoop(GameRenderers._Renderers[TEXTURE_RENDERER], PlayerTextureScalingCoefficient, PlayerTClusters);
	//Destroy game window and renderer before closing program
	WindowRenderHandle::DestroyWindowWithRenderers(GameWindow, GameRenderers);

	//Quit the library and close program
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();

	RuntimeLog::Message(INFO, "SDL-lib successfully terminated");
	RuntimeLog::Message(INFO, "Program successfully terminated");

	return EXIT_SUCCESS;
};