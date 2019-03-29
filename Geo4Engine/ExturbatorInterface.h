#pragma once
#include "Geo4.h"
#include "Serial.h"

class ExturbatorInterface : public Entity, public EventHandler
{
CLASS_PROTOTYPE(ExturbatorInterface);
public:
	ExturbatorInterface(void);
	virtual ~ExturbatorInterface(void);
	
	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIEvent(GUIEvent*const);

	bool	isRenderable() { return false; }
};	
