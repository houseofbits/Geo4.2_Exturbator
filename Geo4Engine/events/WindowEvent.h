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

	WindowEvent(unsigned int type, float ft, float t, unsigned int w, unsigned int h) :eventType(type), frametime(ft), time(t), width(w), height(h) {}
	virtual ~WindowEvent(void) {}

	float frametime;
	float time;

	unsigned int width;
	unsigned int height;

	unsigned int eventType;
};

