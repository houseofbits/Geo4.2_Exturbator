#pragma once

#include "Geo4.h"
#include "DataPacketRequest.h"

enum PacketClassEnumerator : unsigned short
{
	HEADER = 0,
	COMMAND,
	STATUS,
	//	EXT_PROCESS,
	//	PUL_PROCESS,
	//	WND_PROCESS
};

enum GlobalCommandEnumerator : unsigned short
{
	GET_STATUS = 0,		//Get system status
//	GET_PROCESS_DATA,	//Get full settings of system 
};

enum GlobalStatusType : unsigned short
{
	STATUS_IDLE = 0,		//System is on, idling
	STATUS_PREPARING,		//System is preparing for process start
	STATUS_READY,			//System is ready to start process
	STATUS_PROCESS_RUNNING,	//Process has started
	STATUS_ERROR
};

struct CommandOutStructure {
	GlobalCommandEnumerator command;
};
struct StatusInStructure {
	GlobalStatusType	status;
	unsigned short		statusCode;
};
/*
struct EXTRBEXT_ProcessDataPacketIn {
	//Extruder specific data
};
struct EXTRBPUL_ProcessDataPacketIn {
	//Puller specific data
};
struct EXTRBWND_ProcessDataPacketIn {
	//Winder specific data
};
*/

typedef CompleteDataPacket<CommandOutStructure, PacketClassEnumerator> CommandDataPacket;
typedef DataPacket<StatusInStructure, PacketClassEnumerator> StatusDataPacket;


class ExturbatorRequests : public Entity, public EventHandler
{
CLASS_PROTOTYPE(ExturbatorRequests);
public:
	ExturbatorRequests();
	~ExturbatorRequests();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	isRenderable() { return false; }

	bool	OnWindowEvent(WindowEvent*const);

	void	setExtruderState(bool state, float speed);
	void	setPullerState(bool state, float speed);
	void	setWinderState(bool state, float speed);

	DataPacketRequest<CommandDataPacket, StatusDataPacket>		statusResponseEXT;
//	DataPacketRequest<CommandDataPacket, StatusDataPacket>		statusResponseWND;
//	DataPacketRequest<CommandDataPacket, StatusDataPacket>		statusResponsePUL;

};

