#pragma once
#include "Geo4.h"
#include "Serial.h"
#include "HardwareInterface.h"

class GUIEntity;

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

	void	Render(Renderer*);

	bool	OnWindowEvent(WindowEvent*const);

	void	OnSerialEvent (EEvent eEvent, EError eError);

	void	OnReceivePacket(PacketClassType classType, unsigned char* buffer, unsigned short size);

};	
