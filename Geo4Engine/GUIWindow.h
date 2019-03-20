#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"

class GUIWindow :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIWindow);
public:
	GUIWindow();
	~GUIWindow();

	void	Initialise(EventManager*const, SceneManager*);
	void	Deinitialise(EventManager*const, SceneManager*) {};

	void	Serialize(CFONode*, ResourceManager*) {}
	void	Deserialize(CFONode*, ResourceManager*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnInputEvent(InputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	GUIRenderable	renderableTitle;
	GUIRenderable	renderableBody;
	GUIRenderable	renderableShadow;

	float			titleBarSize;
	bool			showOverlay;

	GUIStyleResourceHandle styleSheet;
};

