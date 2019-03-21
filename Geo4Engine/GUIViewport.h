#pragma once
#include "GUIEntity.h"
class GUIViewport :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIViewport);
public:
	GUIViewport();
	~GUIViewport();

	void	Initialise(EventManager*const, SceneManager*);
	void	Deinitialise(EventManager*const, SceneManager*) {};

	void	Serialize(CFONode*, ResourceManager*) {}
	void	Deserialize(CFONode*, ResourceManager*);

	bool	OnWindowEvent(WindowEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	void	keyPressedEvent(unsigned int);
	void	keyDownEvent(unsigned int);
	void	keyUpEvent(unsigned int);
	void	mouseMoveEvent(Vector2, Vector2, bool, bool);
	void	mouseButtonDownEvent(Vector2, bool, bool);
	void	mouseButtonUpEvent(Vector2, bool, bool);

	Entity* hoverObject;

	int			window_width;
	int			window_height;
};

