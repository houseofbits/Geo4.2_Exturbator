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

	int			window_width;
	int			window_height;
};

