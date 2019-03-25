#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"

class GUIList :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIList);
public:
	GUIList();
	~GUIList();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIInputEvent(GUIInputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	float	itemHeight;

	GUIStyleResourceHandle styleSheet;

	GUIRenderable	renderableBody;
};

