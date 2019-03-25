#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"

class GUILayer :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUILayer);
public:
	GUILayer();
	~GUILayer();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	GUIRenderable	renderable;

	GUIStyleResourceHandle styleSheet;

	string styleName;
};

