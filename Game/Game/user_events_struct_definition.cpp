#include "escape_from_the_island.hpp"

//Function that sets the status of the lastly triggered key to either - pressed [true] or released [false]
static INLINE void SetTheTriggeredKeysTo(user_events* _UserEventContainer, const bool _IsPressed)
{
	switch (_UserEventContainer->_CurrentUserEvents.key.scancode)
	{
	case SDL_SCANCODE_ESCAPE:
		_UserEventContainer->_ESC_KeyPressed = _IsPressed; break;

	case SDL_SCANCODE_F1:
		_UserEventContainer->_F1_KeyPressed = _IsPressed; break;

	case SDL_SCANCODE_W:
		_UserEventContainer->_W_KeyPressed = _IsPressed; break;

	case SDL_SCANCODE_A:
		_UserEventContainer->_A_KeyPressed = _IsPressed; break;

	case SDL_SCANCODE_S:
		_UserEventContainer->_S_KeyPressed = _IsPressed; break;

	case SDL_SCANCODE_D:
		_UserEventContainer->_D_KeyPressed = _IsPressed; break;

	case SDL_SCANCODE_LSHIFT:
	case SDL_SCANCODE_RSHIFT:
		_UserEventContainer->_SHIFT_KeyPressed = _IsPressed; break;

	case SDL_SCANCODE_SPACE:
		_UserEventContainer->_SPACE_KeyPressed = _IsPressed; break;

	case SDL_SCANCODE_LEFT:
		_UserEventContainer->_LEFT_ARROW_KeyPressed = _IsPressed; break;

	case SDL_SCANCODE_RIGHT:
		_UserEventContainer->_RIGHT_ARROW_KeyPressed = _IsPressed; break;

	default:
		break;
	}

	return;
};

//Polls recently made events and interprets them by conditions as mostly bools or numbers that could be used to alter the game status later on
void user_events::collect_recent_events(void)
{
	//Get event poll and process it
	SDL_PollEvent(&this->_CurrentUserEvents);

	//
	switch (this->_CurrentUserEvents.type)
	{
	case SDL_EVENT_QUIT:
		//User requested to close the window - shutdown the game
		this->_ShutdownRequested = true; return;

	case SDL_EVENT_KEY_DOWN:
		//Assinging all the new keyboard event value to the values in the struct with some conditions
		SetTheTriggeredKeysTo(this, true); break;

	case SDL_EVENT_KEY_UP:
		//Assinging all the new keyboard event value to the values in the struct with some conditions
		SetTheTriggeredKeysTo(this, false); break;

	default:
		break;
	}

	//Assinging all the new mouse event value to the values in the struct with some conditions
	//Temp!
	const float NewMouseX = this->_CurrentUserEvents.motion.x;
	const float NewMouseY = this->_CurrentUserEvents.motion.y;

	if (NewMouseX >= 0.0f && NewMouseX <= 1280.0f)
		this->_MouseX = NewMouseX;

	if (NewMouseY >= 0.0f && NewMouseY <= 720.0f)
		this->_MouseY = NewMouseY;

	this->_LeftButtonSingleClick =
		(this->_CurrentUserEvents.button.button == SDL_BUTTON_LEFT && this->_CurrentUserEvents.button.clicks == 1);

	return;
};