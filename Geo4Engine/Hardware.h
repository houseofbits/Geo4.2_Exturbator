#pragma once
#include "Geo4.h"
#include "Serial.h"
#include "DataPacketReceiver.h"

class Hardware : public GUIEntity, public EventHandler, public CSerialEx, public DataPacketReceiver
{
CLASS_PROTOTYPE(Hardware);
public:
	Hardware(void);
	virtual ~Hardware(void);
	
	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	DetectPorts();

	bool	OnWindowEvent(WindowEvent*const);

	void	OnSerialEvent (EEvent eEvent, EError eError);

	void	OnReceivePacket(PacketClassType classType, unsigned char* buffer, unsigned short size);

	virtual bool		isRenderable() { return false; }
};	
