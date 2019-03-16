#pragma once
#include "GUIEntity.h"
class GUIButton :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIButton);
public:
	GUIButton();
	~GUIButton();

	void	Initialise(EventManager*const, SceneManager*);
	void	Deinitialise(EventManager*const, SceneManager*) {};

	void	Serialize(CFONode*, ResourceManager*) {}
	void	Deserialize(CFONode*, ResourceManager*);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	int			window_width;
	int			window_height;
};

