#include "escape_from_the_island.hpp"

namespace ErrorHandle //[start]
{

//Add game-wide Error handle! <- make better soon!

//Place an error related to specified code and terminate process eventually
void Report(const std::string& _ErrorMessage, const uint64_t _ErrorCode, const bool _TerminateProcess)
{
	RuntimeLog::Message(ERROR, _ErrorMessage);

	if (!_TerminateProcess)
		return;

	RuntimeLog::Message(CRASH, "previous error was too fatal => process terminated");
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
	//Process ends here - function doesnt return
	std::exit(_ErrorCode);

	return;
};

//Place an error related to SDL-lib and terminate process eventually
void ReportSDL(const bool _TerminateProcess)
{
	ErrorHandle::Report(SDL_GetError(), -1, _TerminateProcess);

	return;
};

}
//ErrorHandle [end]