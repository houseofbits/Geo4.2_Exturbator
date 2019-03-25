#pragma once
#include "EventReceiver.h"

class GUIEvent : public BaseEvent {
	CLASS_PROTOTYPE(GUIEvent);
public:
	enum Type{
		BUTTON_PRESSED = 1,
		LIST_SELECTED,
		WINDOW_SHOWN,
		WINDOW_HIDDEN,
	};

	GUIEvent(std::string n, Type t) : type(t){}
	virtual ~GUIEvent(void) {}

	Type			type;
};