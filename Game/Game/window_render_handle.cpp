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
void CreateNewRenderers(SDL_Window*& _Window, RCluster& _Renderers, const uint64_t _CountOfRenderers)
{
	if (_Renderers._Renderers.size() != NULL)
	{
		//ErrorHandle...
		RuntimeLog::Message(WARNING, "some renderers already exists in the given cluster => new ones cannot be created");

		return;
	}

	_Renderers._Renderers.clear();

	for (uint64_t c = NULL; c < _CountOfRenderers; c++)
	{
		SDL_Renderer* NewRenderer = SDL_CreateRenderer(_Window, NULL);

		if (NewRenderer == nullptr)
			ErrorHandle::ReportSDL(true);

		_Renderers._Renderers.emplace_back(NewRenderer);
	}

	RuntimeLog::Message(INFO, "successfully created {" + std::to_string(_CountOfRenderers) + "} renderer/s");

	return;
};

//Initialize graphical window and a specified amount of renderers
INLINE void CreateNewWindowWithRenderers(SDL_Window*& _Window, RCluster& _Renderers, const uint64_t _CountOfRenderers)
{
	std::string WindowTitle;
	int64_t WindowWidth = NULL, WindowLength = NULL;

	if (_Window != nullptr)
	{
		//ErrorHandle...
		RuntimeLog::Message(WARNING, "window already exists => it wont get initialized again");
	
		return;
	}

	LoadBasicWindowsProps(WindowTitle, WindowWidth, WindowLength);
	_Window = SDL_CreateWindow(WindowTitle.c_str(), WindowWidth, WindowLength, NULL);

	if (_Window == nullptr)
		ErrorHandle::ReportSDL(true);

	RuntimeLog::Message(INFO, "successfully created main window");
	WindowRenderHandle::CreateNewRenderers(_Window, _Renderers, _CountOfRenderers);

	return;
};

//Destroys specified renderers
void DestroyRenderers(SDL_Window*& _Window, RCluster& _Renderers)
{
	uint64_t CountOfRenderers = _Renderers._Renderers.size();

	if (CountOfRenderers == NULL)
	{
		//ErrorHandle...
		RuntimeLog::Message(WARNING, "no renderers to destroy");

		return;
	}

	for (uint64_t c = NULL; c < CountOfRenderers; c++)
		SDL_DestroyRenderer(_Renderers._Renderers[c]);

	_Renderers._Renderers.clear();
	RuntimeLog::Message(INFO, "successfully destroyed {" + std::to_string(CountOfRenderers) + "} renderer/s");

	return;
};

//Destroys graphical window and renderers
INLINE void DestroyWindowWithRenderers(SDL_Window*& _Window, RCluster& _Renderers)
{
	WindowRenderHandle::DestroyRenderers(_Window, _Renderers);
	SDL_DestroyWindow(_Window);
	RuntimeLog::Message(INFO, "successfully destroyed main window");

	return;
};

//Sets the color that will be used that the beginning of every frame to black - should be called only once at the beginning of program
void SetFrameDefaultColorToBlack(SDL_Renderer* const _FrameRenderer)
{
	SDL_SetRenderDrawColor(_FrameRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

	return;
};

}
//WindowRenderHandle [end]