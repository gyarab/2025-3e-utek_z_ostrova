#ifndef USER_EVENTS
#define USER_EVENTS

//
#include "escape_from_the_island.hpp"

//Container holding currently made events that are interpreted as bools and numbers to made it easier to work with later on
struct user_events
{
	//Container holding currently made events - should not be used outside the struct
	SDL_Event _CurrentUserEvents = SDL_Event();

	//User pressed the X button on the game windows
	bool _ShutdownRequested = false;
	//Mouse events
	float _MouseX = 0.0f;
	float _MouseY = 0.0f;
	bool _LeftButtonSingleClick = false;
	//Keyboard events
	bool _ESC_KeyPressed = false;
	bool _W_KeyPressed = false;
	bool _A_KeyPressed = false;
	bool _S_KeyPressed = false;
	bool _D_KeyPressed = false;

public:
	//Polls recently made events and interprets them by conditions as mostly bools or numbers that could be used to alter the game status later on
	INLINE void collect_recent_events(void);
};

#endif