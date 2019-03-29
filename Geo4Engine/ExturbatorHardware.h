#pragma once
#include "Geo4.h"
#include "Serial.h"
#include "DataPacketReceiver.h"

#include <queue>

enum PacketClassEnumerator : unsigned short
{
	COMMAND = 0,
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
	StatusPacketIn status;	
	//Extruder specific data
};
struct EXTRBPUL_ProcessDataPacketIn {
	StatusPacketIn status;
	//Puller specific data
};
struct EXTRBWND_ProcessDataPacketIn {
	StatusPacketIn status;
	//Winder specific data
};
*/

typedef CompleteDataPacket<CommandOutStructure, PacketClassEnumerator> CommandDataPacket;


class ExturbatorHardware : public Entity, public EventHandler, public CSerialEx, public DataPacketReceiver<PacketClassEnumerator>
{
CLASS_PROTOTYPE(ExturbatorHardware);
public:
	ExturbatorHardware(void);
	virtual ~ExturbatorHardware(void);
	
	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OpenPort(string name, string baud, string stop, string parity);

	bool	DetectPorts();

	bool	OnWindowEvent(WindowEvent*const);

	void	OnSerialEvent (EEvent eEvent, EError eError);

	void	OnReceivePacket(PacketClassEnumerator classType, unsigned char* buffer, unsigned short size);

	void	WritePacket(BaseDataPacket* packet);
	void	WritePacketToFile(BaseDataPacket* packet, string filename);

	bool	isRenderable() { return false; }

	string	configPortName;
	string	configBaudRate;
	string	configStopBits;
	string	configParity;

	unsigned short	portIndex;
	bool			portIsValid;
	float			timeoutCounter;
	
	std::queue<unsigned char> serialInBuffer;
	unsigned char	outputBuffer[MAX_PAYLOAD_SIZE];
	unsigned int	size;
};	
