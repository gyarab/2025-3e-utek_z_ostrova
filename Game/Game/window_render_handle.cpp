#include "escape_from_the_island.hpp"

//Returns basic info about main window specified in the "config.ini" file
static INLINE void LoadBasicWindowsProps(std::string& _WindowTitle, int64_t& _WindowWidth, int64_t& _WindowLength)
{
	ConfigFile::ReadValue(BUNDLE_AppWindowConfig, SELECTOR_tWindowTitle, _WindowTitle);
	ConfigFile::ReadValue(BUNDLE_AppWindowConfig, SELECTOR_Width, _WindowWidth);
	ConfigFile::ReadValue(BUNDLE_AppWindowConfig, SELECTOR_Length, _WindowLength);

	return;
}

namespace WindowRenderHandle //[start]
{

//Initialize a specified amount of renderers for a specified window
void CreateNewRenderers(WRCluster& _WindowAndRenderers, const uint64_t _CountOfRenderers)
{
	if (_WindowAndRenderers._Renderers.size() != NULL)
	{
		//ErrorHandle...
		RuntimeLog::Message(WARNING, "some renderers already exists in the given cluster => new ones cannot be created");

		return;
	}

	_WindowAndRenderers._Renderers.clear();

	for (uint64_t c = NULL; c < _CountOfRenderers; c++)
	{
		SDL_Renderer* NewRenderer = SDL_CreateRenderer(_WindowAndRenderers._Window, NULL);

		if (NewRenderer == nullptr)
			ErrorHandle::ReportSDL(true);

		_WindowAndRenderers._Renderers.emplace_back(NewRenderer);
	}

	RuntimeLog::Message(INFO, "successfully created {" + std::to_string(_CountOfRenderers) + "} renderer/s");

	return;
};

//Initialize graphical window and a specified amount of renderers [default amount of renderers is 1] - should be called right after creating new WRCluster
void CreateNewWindowWithRenderers(WRCluster& _WindowAndRenderers, const uint64_t _CountOfRenderers)
{
	std::string WindowTitle;
	int64_t WindowWidth = NULL, WindowLength = NULL;

	if (_WindowAndRenderers._Window != nullptr)
	{
		//ErrorHandle...
		RuntimeLog::Message(WARNING, "window already exists => it wont get initialized again");
	
		return;
	}

	LoadBasicWindowsProps(WindowTitle, WindowWidth, WindowLength);
	_WindowAndRenderers._Window = SDL_CreateWindow(WindowTitle.c_str(), (int32_t)WindowWidth, (int32_t)WindowLength, NULL);

	if (_WindowAndRenderers._Window == nullptr)
		ErrorHandle::ReportSDL(true);

	RuntimeLog::Message(INFO, "successfully created main window");
	WindowRenderHandle::CreateNewRenderers(_WindowAndRenderers, _CountOfRenderers);

	return;
};

//Destroys specified renderers
void DestroyRenderers(WRCluster& _WindowAndRenderers)
{
	uint64_t CountOfRenderers = _WindowAndRenderers._Renderers.size();

	if (CountOfRenderers == NULL)
	{
		//ErrorHandle...
		RuntimeLog::Message(WARNING, "no renderers to destroy");

		return;
	}

	for (uint64_t c = NULL; c < CountOfRenderers; c++)
		SDL_DestroyRenderer(_WindowAndRenderers._Renderers[c]);

	_WindowAndRenderers._Renderers.clear();
	RuntimeLog::Message(INFO, "successfully destroyed {" + std::to_string(CountOfRenderers) + "} renderer/s");

	return;
};

//Destroys graphical window and renderers
void DestroyWindowWithRenderers(WRCluster& _WindowAndRenderers)
{
	WindowRenderHandle::DestroyRenderers(_WindowAndRenderers);
	SDL_DestroyWindow(_WindowAndRenderers._Window);
	RuntimeLog::Message(INFO, "successfully destroyed main window");

	return;
};

//Sets the color that will be used that the beginning of every frame to black - should be called only once at the beginning of program
void SetFrameDefaultColorToBlack(WRCluster& _WindowAndRenderers, const uint64_t _IndexOfFrameRenderer)
{
	SDL_SetRenderDrawColor(_WindowAndRenderers._Renderers[_IndexOfFrameRenderer], 0, 0, 0, SDL_ALPHA_OPAQUE);

	return;
};

}
//WindowRenderHandle [end]