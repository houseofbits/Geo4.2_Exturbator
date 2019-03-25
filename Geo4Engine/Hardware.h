#pragma once
#include "Geo4.h"
#include "Serial.h"

static unsigned char c_data[128];

class GUIEntity;



class Hardware : public GUIEntity, public EventHandler, public CSerialEx
{
CLASS_PROTOTYPE(Hardware);
public:
	Hardware(void);
	virtual ~Hardware(void);
	
	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	void	Render(Renderer*);

	bool	OnWindowEvent(WindowEvent*const);

	void	OnSerialEvent (EEvent eEvent, EError eError);	

	unsigned int	prepareSerialFrame();




};	
