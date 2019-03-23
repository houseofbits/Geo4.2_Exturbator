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

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIInputEvent(GUIInputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	GUIRenderable	renderableTitle;
	GUIRenderable	renderableBody;
	GUIRenderable	renderableShadow;

	float			titleBarSize;
	bool			showOverlay;

	GUIStyleResourceHandle styleSheet;

	string styleTitleName;
	string styleBodyName;
	string styleShadowName;

	unsigned int			windowWidth;
	unsigned int			windowHeight;
};

