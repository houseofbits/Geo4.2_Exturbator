#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"

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

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIInputEvent(GUIInputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	GUIRenderable	renderableActive;
	GUIRenderable	renderableHover;
	GUIRenderable	renderablePressed;

	GUIStyleResourceHandle styleSheet;
};

