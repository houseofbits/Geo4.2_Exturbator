#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"

class GUIGraph :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIGraph);
public:
	GUIGraph();
	~GUIGraph();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIInputEvent(GUIInputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	GUIRenderable	renderable;

	GUIStyleResourceHandle styleSheet;

	string styleName;
};

