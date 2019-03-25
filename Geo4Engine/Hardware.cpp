#include "Geo4.h"
#include <crtdbg.h>
#include <tchar.h>
#include <windows.h>

using namespace std;

CLASS_DECLARATION(Hardware);

Hardware::Hardware(void) : GUIEntity(),
						EventHandler(),
						CSerialEx()
{ 

}

Hardware::~Hardware(void)
{	
	
}

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

bool Hardware::OnWindowEvent(WindowEvent*const event)
{		

	return 1;
}

unsigned int Hardware::prepareSerialFrame() {
	/*
	unsigned int counter = 0;

	c_data[counter++] = 'W';
	c_data[counter++] = 'A';
	c_data[counter++] = 'L';
	c_data[counter++] = 'L';

	if (faderMode == 1) {
		//cout << std::to_string(255 - faders[0]) << endl;
		c_data[counter++] = 255 - faders[outputMapping[0]];
		c_data[counter++] = 255 - faders[outputMapping[1]];
		c_data[counter++] = 255 - faders[outputMapping[2]];
		c_data[counter++] = 255 - faders[outputMapping[3]];
		c_data[counter++] = 255 - faders[outputMapping[4]];
		c_data[counter++] = 255 - faders[outputMapping[5]];
		c_data[counter++] = 255 - faders[outputMapping[6]];
		c_data[counter++] = 255 - faders[outputMapping[7]];
	}
	else {
		c_data[counter++] = 255 - fadersRms[outputMapping[0]];
		c_data[counter++] = 255 - fadersRms[outputMapping[1]];
		c_data[counter++] = 255 - fadersRms[outputMapping[2]];
		c_data[counter++] = 255 - fadersRms[outputMapping[3]];
		c_data[counter++] = 255 - fadersRms[outputMapping[4]];
		c_data[counter++] = 255 - fadersRms[outputMapping[5]];
		c_data[counter++] = 255 - fadersRms[outputMapping[6]];
		c_data[counter++] = 255 - fadersRms[outputMapping[7]];
	}
	c_data[counter++] = Utils::CRC8(c_data, counter);
	*/
	return 0;
}

void Hardware::Render(Renderer*)
{
	glPushMatrix();
	glTranslatef(m_LocalPos.x, m_LocalPos.y, 0);


	glPopMatrix();
}

void Hardware::OnSerialEvent (EEvent eEvent, EError eError)
{
	if (eEvent & CSerial::EEventRecv){
		DWORD dwBytesRead = 0;
		char szBuffer[101];
		do{
			Read(szBuffer,sizeof(szBuffer)-1,&dwBytesRead);

			if (dwBytesRead > 0){
				szBuffer[dwBytesRead] = '\0';
			}
		}while (dwBytesRead == sizeof(szBuffer)-1);
	}
}
