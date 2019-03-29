#include "Geo4.h"
#include <crtdbg.h>
#include <tchar.h>
#include <windows.h>

using namespace std;

CLASS_DECLARATION(ExturbatorHardware);

ExturbatorHardware::ExturbatorHardware(void) : Entity(),
	EventHandler(),
	CSerialEx(),
	DataPacketReceiver<PacketClassEnumerator>(),
	configPortName(),
	configBaudRate(),
	configStopBits(),
	configParity(),
	portIndex(0),
	timeoutCounter(0),
	serialInBuffer(),
	portIsValid(false),
	size()
{ 
	memset(outputBuffer, 0, MAX_PAYLOAD_SIZE);
}

ExturbatorHardware::~ExturbatorHardware(void)
{	}

void ExturbatorHardware::Initialise(EventManager*const event_manager, ResourceManager*const resourceManager)
{	
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &ExturbatorHardware::OnWindowEvent);


	CommandDataPacket packet("EXTRBEXT", PacketClassEnumerator::COMMAND);
	packet.setPayload(CommandOutStructure{ GET_STATUS });
	WritePacket(&packet);
}

void ExturbatorHardware::Deserialize(CFONode* node)
{	
	Entity::Deserialize(node);

	//DetectPorts();

	node->getValueString("serial_port", configPortName);
	node->getValueString("baud_rate", configBaudRate);
	node->getValueString("stop_bits", configStopBits);
	node->getValueString("parity", configParity);

	OpenPort(configPortName, configBaudRate, configStopBits, configParity);
}

bool ExturbatorHardware::OpenPort(string name, string baudStr, string stop, string parityStr) {

	LONG    lLastError = ERROR_SUCCESS;
	std::wstring ws;
	ws.assign(name.begin(), name.end());
	lLastError = Open(&ws[0]);
	if (lLastError != ERROR_SUCCESS) {
		cout << "Unable to open COM-port: " << name << endl;
		return false;
	}
	else {
		cout << "Port is open " << name<< endl;
	}
	CSerial::EBaudrate baud = CSerial::EBaud9600;
	CSerial::EStopBits stopb = CSerial::EStop2;
	CSerial::EParity parity = CSerial::EParNone;
	CSerial::EDataBits datab = CSerial::EData8;

	if (baudStr.length() > 0) {
		if (baudStr == "110")baud = CSerial::EBaud110;
		else if (baudStr == "300")baud = CSerial::EBaud300;
		else if (baudStr == "600")baud = CSerial::EBaud600;
		else if (baudStr == "1200")baud = CSerial::EBaud1200;
		else if (baudStr == "2400")baud = CSerial::EBaud2400;
		else if (baudStr == "4800")baud = CSerial::EBaud4800;
		else if (baudStr == "9600")baud = CSerial::EBaud9600;
		else if (baudStr == "14400")baud = CSerial::EBaud14400;
		else if (baudStr == "19200")baud = CSerial::EBaud19200;
		else if (baudStr == "38400")baud = CSerial::EBaud38400;
		else if (baudStr == "56000")baud = CSerial::EBaud56000;
		else if (baudStr == "57600")baud = CSerial::EBaud57600;
		else if (baudStr == "115200")baud = CSerial::EBaud115200;
		else if (baudStr == "128000")baud = CSerial::EBaud128000;
		else if (baudStr == "256000")baud = CSerial::EBaud256000;
	}
	if (stop.length() > 0) {
		if (stop == "1")stopb = CSerial::EStop1;
		else if (stop == "2")stopb = CSerial::EStop2;
	}
	if (parityStr.length() > 0) {
		if (parityStr == "none")parity = CSerial::EParNone;
		else if (parityStr == "odd")parity = CSerial::EParOdd;
		else if (parityStr == "even")parity = CSerial::EParEven;
		else if (parityStr == "mark")parity = CSerial::EParMark;
		else if (parityStr == "space")parity = CSerial::EParSpace;
	}
	Setup(baud, datab, parity, stopb);
	SetupHandshaking(CSerial::EHandshakeOff);
	SetMask(CSerial::EEventBreak |
		CSerial::EEventCTS |
		CSerial::EEventDSR |
		CSerial::EEventError |
		CSerial::EEventRing |
		CSerial::EEventRLSD |
		CSerial::EEventRecv);

	StartListener();

	return true;
}

bool ExturbatorHardware::DetectPorts() {

	for (int i = portIndex; i < 255; i++){
		string str = "COM" + Utils::IntToString(i);
		std::wstring ws;
		ws.assign(str.begin(), str.end());
		EPort p = CheckPort(&ws[0]);
		if (p == EPort::EPortAvailable) {
			cout << str<< " is available" << endl;
			portIndex = i;
			if (OpenPort(str, configBaudRate, configStopBits, configParity)) {

				configPortName = str;
				portIsValid = false;

				//Send STATUS packet


				return true;
			}
		}
	}
	//Finished scanning all ports, start over
	portIndex = 0;
	return false;
}

bool ExturbatorHardware::OnWindowEvent(WindowEvent*const event){

	if(portIsValid)timeoutCounter += event->frametime;

	if (timeoutCounter > 5000 && portIsValid) {
		portIsValid = false;
	}
	unsigned char c;
	while (!serialInBuffer.empty()){
		c = serialInBuffer.front();
		serialInBuffer.pop();
		readPacketByte(c);
	}
	return 1;
}

void ExturbatorHardware::OnSerialEvent (EEvent eEvent, EError eError){

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
			serialInBuffer.push(szBuffer[i]);
		}
	}	
}

void ExturbatorHardware::OnReceivePacket(PacketClassEnumerator classType, unsigned char* buffer, unsigned short size) {

	timeoutCounter = 0;

	if (classType == STATUS) {

		DataPacket<StatusInStructure> packet;
		packet.fromBytes(buffer, size);

		cout << "Received STATUS packet" << endl;

		portIsValid = true;
	}

}

void ExturbatorHardware::WritePacket(BaseDataPacket* packet) {
	size = packet->getSize();
	cout << "Send packet of size: " << size<<endl;
	packet->toBytes(outputBuffer, size);
	if(IsOpen())Write(outputBuffer, size,0,0,10);
}

void ExturbatorHardware::WritePacketToFile(BaseDataPacket* packet, string filename) {
	size = packet->getSize();
	packet->toBytes(outputBuffer, size);
	FILE* pFile;
	fopen_s(&pFile, filename.c_str(), "wb");
	fwrite(outputBuffer, 1, size, pFile);
	fclose(pFile);
	cout << "Packet written to file '" << filename.c_str() << "'" << endl;
}