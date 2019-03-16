#include "../Geo4.h"

CLASS_DECLARATION(InputEvent);

InputEvent::~InputEvent()
{
	delete sdl;
}
