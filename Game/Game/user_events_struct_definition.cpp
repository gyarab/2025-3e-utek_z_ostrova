#include "escape_from_the_island.hpp"

//Function that sets the status of the lastly triggered key to either - pressed [true] or released [false]
static INLINE void SetTheTriggeredKeysTo(user_events* _UserEventContainer, const bool _IsPressed)
{
	if (_UserEventContainer->_CurrentUserEvents.key.scancode == SDL_SCANCODE_ESCAPE)
	{
		_UserEventContainer->_ESC_KeyPressed = _IsPressed;

		return;
	}

	if (_UserEventContainer->_CurrentUserEvents.key.scancode == SDL_SCANCODE_W)
	{
		_UserEventContainer->_W_KeyPressed = _IsPressed;

		return;
	}

	if (_UserEventContainer->_CurrentUserEvents.key.scancode == SDL_SCANCODE_A)
	{
		_UserEventContainer->_A_KeyPressed = _IsPressed;

		return;
	}

	if (_UserEventContainer->_CurrentUserEvents.key.scancode == SDL_SCANCODE_S)
	{
		_UserEventContainer->_S_KeyPressed = _IsPressed;

		return;
	}

	if (_UserEventContainer->_CurrentUserEvents.key.scancode == SDL_SCANCODE_D)
	{
		_UserEventContainer->_D_KeyPressed = _IsPressed;

		return;
	}

	return;
};

//Polls recently made events and interprets them by conditions as mostly bools or numbers that could be used to alter the game status later on
INLINE void user_events::collect_recent_events(void)
{
	//Get event poll and process it
	SDL_PollEvent(&this->_CurrentUserEvents);

	//User requested to close the window - shutdown the game
	if (_CurrentUserEvents.type == SDL_EVENT_QUIT)
	{
		this->_ShutdownRequested = true;

		return;
	}

	//Assinging all the new event value to the values in the struct with some conditions
	this->_MouseX = this->_CurrentUserEvents.motion.x;
	this->_MouseY = this->_CurrentUserEvents.motion.y;

	this->_LeftButtonSingleClick =
		(this->_CurrentUserEvents.button.button == SDL_BUTTON_LEFT && this->_CurrentUserEvents.button.clicks == 1);

	if (this->_CurrentUserEvents.type == SDL_EVENT_KEY_DOWN)
	{
		SetTheTriggeredKeysTo(this, true);

		return;
	}

	if (this->_CurrentUserEvents.type == SDL_EVENT_KEY_UP)
	{
		SetTheTriggeredKeysTo(this, false);

		return;
	}

	return;
};