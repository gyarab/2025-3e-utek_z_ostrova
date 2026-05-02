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
	
	RuntimeLog::Message(INFO, "SDL-lib successfully initialized"); //Add logs everywhere! and comments!

	//Initialize window and renderers variables
	WRCluster MainWindow = {};
	SDL_Window* GameWindow = nullptr;
	constexpr uint64_t CountOfGameRenderers = 1;
	
	//Initialize texture clusters
	TCluster PlayerTClusters;
	TCluster Level1TClusters;

	entity Player;
	entity Background;
	
	//Prepare window and renderer for game loop
	WindowRenderHandle::CreateNewWindowWithRenderers(MainWindow, CountOfGameRenderers);
	//Sets the default frame color
	WindowRenderHandle::SetFrameDefaultColorToBlack(MainWindow._Renderers[TEXTURE_RENDERER]);
	//Prepare textures
	TextureHandle::PrepareAllNeeded(MainWindow._Renderers[TEXTURE_RENDERER], PlayerTClusters, Level1TClusters);
	//Game starts
	GameLoopThread::MainLoop(MainWindow._Renderers[TEXTURE_RENDERER], PlayerTClusters, Level1TClusters._Textures[0]);
	//Destroy game window and renderer before closing program
	WindowRenderHandle::DestroyWindowWithRenderers(MainWindow);

	//Quit the library and close program
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();

	RuntimeLog::Message(INFO, "SDL-lib successfully terminated");
	RuntimeLog::Message(INFO, "Program successfully terminated");

	return EXIT_SUCCESS;
};