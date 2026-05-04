#include "escape_from_the_island.hpp"

//polls recently made events and interprets them by conditions as mostly bools or numbers that could be used to alter the game status later on
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
		this->_CurrentUserEvents.key.scancode == SDL_SCANCODE_ESCAPE ?	this->_ESC_KeyPressed = true :
			this->_CurrentUserEvents.key.scancode == SDL_SCANCODE_W ?		this->_W_KeyPressed = true :
				this->_CurrentUserEvents.key.scancode == SDL_SCANCODE_A ?		this->_A_KeyPressed = true :
					this->_CurrentUserEvents.key.scancode == SDL_SCANCODE_S ?		this->_S_KeyPressed = true :
						this->_CurrentUserEvents.key.scancode == SDL_SCANCODE_D ?		this->_D_KeyPressed = true : NULL;
			
		return;
	}

	if (this->_CurrentUserEvents.type == SDL_EVENT_KEY_UP)
	{
		this->_CurrentUserEvents.key.scancode == SDL_SCANCODE_ESCAPE ?	this->_ESC_KeyPressed = false :
			this->_CurrentUserEvents.key.scancode == SDL_SCANCODE_W ?		this->_W_KeyPressed = false :
				this->_CurrentUserEvents.key.scancode == SDL_SCANCODE_A ?		this->_A_KeyPressed = false :
					this->_CurrentUserEvents.key.scancode == SDL_SCANCODE_S ?		this->_S_KeyPressed = false :
						this->_CurrentUserEvents.key.scancode == SDL_SCANCODE_D ?		this->_D_KeyPressed = false : NULL;

		return;
	}

	return;
};