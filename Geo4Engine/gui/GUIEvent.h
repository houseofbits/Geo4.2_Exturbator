#pragma once
#include "EventReceiver.h"

class GUIEvent : public BaseEvent {
	CLASS_PROTOTYPE(GUIEvent);
public:
	GUIEvent(std::string n, unsigned int t) :name(n), type(t), value(0.0f) {}
	GUIEvent(std::string n, unsigned int t, float val) :name(n), type(t), value(val) {}
	virtual ~GUIEvent(void) {}

	enum {
		BUTTON_PRESSED = 1,
		HIDE,
		SHOW,
		POT_CHANGED,
		POT_VALUE_SET,
		SET_TEXT
	};

	unsigned int	type;
	float			value;
	std::string		name;
};