#pragma once
#include "../EventReceiver.h"

class SceneManager;
class SDLWindow;

class WindowEvent :
	public BaseEvent
{
CLASS_PROTOTYPE(WindowEvent);
public:
	enum { FRAME_TIME,
			WINDOW_CREATED};

	WindowEvent(float ft, float t, SDLWindow* w) :frametime(ft), time(t), window(w), event_type(WindowEvent::FRAME_TIME){}
	WindowEvent(unsigned int t, SDLWindow* w) :event_type(t), window(w) {}
	virtual ~WindowEvent(void) {}

	//FRAME_TIME
	float frametime;
	float time;

	SDLWindow*	window;

	unsigned int event_type;
};

