#pragma once
#include "Geo4.h"
#include "Serial.h"
#include "DataPacketRequest.h"
#include <queue>
#include <thread>
#include <mutex>

class Hardware : public Entity, public CSerialEx, public DataPacketRequestManager<PacketClassEnumerator>
{
CLASS_PROTOTYPE(Hardware);
public:
	Hardware(void);
	virtual ~Hardware(void);
	
	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OpenPort(string name, string baud, string stop, string parity);

	bool	DetectPorts();

	void	OnSerialEvent (EEvent eEvent, EError eError);

	unsigned char	getInputData();
	bool			hasInputData();

	void	WritePacket(BaseDataPacket* packet);
	void	WritePacketToFile(BaseDataPacket* packet, string filename);

	bool	isRenderable() { return false; }

	string	configPortName;
	string	configBaudRate;
	string	configStopBits;
	string	configParity;

	bool	isPortValid();
	void	setPortValid(bool b) { portValid = b; }

	bool			portValid;
	unsigned short	portIndex;
	
	std::queue<unsigned char> inputBuffer;

	unsigned char	outputBuffer[MAX_PAYLOAD_SIZE];
	unsigned int	size;

	void	requestsThreadProc();

	std::thread*	requestsThread;
	std::mutex*		mtx;
};	
