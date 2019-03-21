#pragma once
#include "EventReceiver.h"
class GUIInputEvent : public BaseEvent
{
CLASS_PROTOTYPE(GUIInputEvent);
public:
	enum EventType {
		CLICK,
		MOUSEDOWN,
		MOUSEUP,
		MOUSEENTER,
		MOUSELEAVE,
		MOUSEMOVE,
		KEYDOWN,
		KEYUP,
		KEYPRESS
	};

	GUIInputEvent();
	GUIInputEvent(EventType type, unsigned int key) : type(type), keyCode(key) {	}
	GUIInputEvent(EventType type, Vector2 mousePosition, unsigned int mouseButton) : type(type), mousePosition(mousePosition), mouseButton(mouseButton) {	}
	
	virtual ~GUIInputEvent();
	
	EventType		type;
	Vector2			mousePosition;
	unsigned int	mouseButton;
	unsigned int	keyCode;
};

