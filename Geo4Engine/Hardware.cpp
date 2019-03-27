#include "Geo4.h"
#include <crtdbg.h>
#include <tchar.h>
#include <windows.h>

using namespace std;

CLASS_DECLARATION(Hardware);

Hardware::Hardware(void) : GUIEntity(),
	EventHandler(),
	CSerialEx(),
	DataPacketReceiver()
{ 

}

Hardware::~Hardware(void)
{	}

void Hardware::Initialise(EventManager*const event_manager, ResourceManager*const resourceManager)
{	
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &Hardware::OnWindowEvent);
}

void Hardware::Deserialize(CFONode* node)
{	
	GUIEntity::Deserialize(node);

	string str;
	if(node->getValueString("serial_port", str)){
		LONG    lLastError = ERROR_SUCCESS;
		std::wstring ws;
		ws.assign(str.begin(), str.end());
		lLastError = Open(&ws[0]);
		if (lLastError != ERROR_SUCCESS){
			cout<<"Unable to open COM-port: "<<str<<endl;	
		}
		CSerial::EBaudrate baud = CSerial::EBaud9600;
		CSerial::EStopBits stopb = CSerial::EStop2;
		CSerial::EParity parity = CSerial::EParNone;
		CSerial::EDataBits datab = CSerial::EData8;

		if(node->getValueString("baud_rate", str)){
			if(str=="110")baud = CSerial::EBaud110;
			else if(str=="300")baud = CSerial::EBaud300;
			else if(str=="600")baud = CSerial::EBaud600;
			else if(str=="1200")baud = CSerial::EBaud1200;
			else if(str=="2400")baud = CSerial::EBaud2400;
			else if(str=="4800")baud = CSerial::EBaud4800;
			else if(str=="9600")baud = CSerial::EBaud9600;
			else if(str=="14400")baud = CSerial::EBaud14400;
			else if(str=="19200")baud = CSerial::EBaud19200;
			else if(str=="38400")baud = CSerial::EBaud38400;
			else if(str=="56000")baud = CSerial::EBaud56000;
			else if(str=="57600")baud = CSerial::EBaud57600;
			else if(str=="115200")baud = CSerial::EBaud115200;
			else if(str=="128000")baud = CSerial::EBaud128000;
			else if(str=="256000")baud = CSerial::EBaud256000;
		}
		if(node->getValueString("stop_bits", str)){
			if(str=="1")stopb = CSerial::EStop1;
			else if(str=="2")stopb = CSerial::EStop2;
		}
		if(node->getValueString("parity", str)){
			if(str=="none")parity = CSerial::EParNone;
			else if(str=="odd")parity = CSerial::EParOdd;
			else if(str=="even")parity = CSerial::EParEven;
			else if(str=="mark")parity = CSerial::EParMark;
            else if(str=="space")parity = CSerial::EParSpace;
		}
		Setup(baud,datab,parity,stopb);
		SetupHandshaking(CSerial::EHandshakeOff);
		SetMask(CSerial::EEventBreak |
									CSerial::EEventCTS   |
									CSerial::EEventDSR   |
									CSerial::EEventError |
									CSerial::EEventRing  |
									CSerial::EEventRLSD  |
									CSerial::EEventRecv);
		StartListener();
	}
}

bool Hardware::OnWindowEvent(WindowEvent*const event){

	return 1;
}

void Hardware::Render(Renderer*)
{	}

void Hardware::OnSerialEvent (EEvent eEvent, EError eError){

	if (eEvent & CSerial::EEventRecv){
		DWORD dwBytesRead = 0;
		char szBuffer[101];
		do{
			Read(szBuffer,sizeof(szBuffer)-1,&dwBytesRead);

			if (dwBytesRead > 0){
				szBuffer[dwBytesRead] = '\0';
			}
		}while (dwBytesRead == sizeof(szBuffer)-1);
		
		for (unsigned int i = 0; i < dwBytesRead; i++) {
			readPacketByte(szBuffer[i]);
		}
	}	
}


//For testing
struct SimplePacket {
	unsigned char c1;
	unsigned char c2;
};

void Hardware::OnReceivePacket(PacketClassType classType, unsigned char* buffer, unsigned short size) {

	if (classType == COMMAND) {
		cout << "Received COMMAND packet" << endl;

		DataPacket<SimplePacket> pack;
		pack.fromBytes(buffer, size);

		cout << "c1: " << pack.packet.data.c1 << endl;
		cout << "c2: " << pack.packet.data.c2 << endl;
	}

}