#include "escape_from_the_island.hpp"

//First game version - 2 levels, walking player, some obstacles, menu screen
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
	WRCluster MainWindow;
	//Initialize all entities, their props and textures variables
	ECluster AllEntities;
	
	//Prepare window and renderer for game loop
	WindowRenderHandle::CreateNewWindowWithRenderers(MainWindow);
	//Sets the default frame color
	WindowRenderHandle::SetFrameDefaultColorToBlack(MainWindow, TEXTURE_RENDERER);
	//Prepare entities
	EntityBulkInitHandle::PrepareECluster(AllEntities);
	//Prepare textures
	TextureHandle::PrepareForAllEntities(MainWindow, AllEntities);
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