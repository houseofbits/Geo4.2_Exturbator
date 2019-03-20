#pragma once
#include "EventReceiver.h"
class GUIInputEvent : public BaseEvent
{
CLASS_PROTOTYPE(GUIInputEvent);
public:
	GUIInputEvent();
	virtual ~GUIInputEvent();

	enum EventType {
		CLICK,
		HOVER,
		LEAVE,
	};

	EventType	type;
	Vector2		cursorPosition;
};

