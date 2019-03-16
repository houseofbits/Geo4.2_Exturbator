#include "Geo4.h"
#include <crtdbg.h>
#include <tchar.h>
#include <windows.h>

using namespace std;

CLASS_DECLARATION(Hardware);

Hardware::Hardware(void) : GUIEntity(),
						EventHandler(),
						CSerialEx(),
	fader(0),
	faderMode(1),
	sendTimer(0)
{ 
	memset(faders, 0, 8);
}

Hardware::~Hardware(void)
{	
	
}

void Hardware::Initialise(EventManager*const event_manager, SceneManager* mgr)
{	
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &Hardware::OnInputEvent);
	event_manager->RegisterEventReceiver(this, &Hardware::OnWindowEvent);
	event_manager->RegisterEventReceiver(this, &Hardware::OnGUIEvent);
}

void Hardware::Deserialize(CFONode* node, ResourceManager* mgr)
{	

	outputMapping[0] = 0;
	outputMapping[1] = 1;
	outputMapping[2] = 2;
	outputMapping[3] = 3;
	outputMapping[4] = 4;
	outputMapping[5] = 5;
	outputMapping[6] = 6;
	outputMapping[7] = 7;

	GUIEntity::Deserialize(node, mgr);

	node->getValueInt("fader_mode", faderMode);

	CFONode* n = node->GetFirstChild("output_mapping");
	if (n) {
		n = n->GetFirstChild();
		if (n) {
			vector<string> out;
			Utils::Explode(',', n->GetName(), out);
			if (out.size() > 0) {
				for (int i = 0; i < out.size(); i++) {
					int val = std::stoi(out[i]);
					if(val >= 0 && val < 8)outputMapping[i] = val;
				}
			}
		}
	}

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

bool Hardware::OnInputEvent(InputEvent*const event)
{

	switch (event->sdl->type) {
	case SDL_KEYDOWN:
		if (event->sdl->key.keysym.sym == SDLK_UP) {
			fader+=5;
			cout << "val: " << (unsigned int)fader << endl;
		}
		if (event->sdl->key.keysym.sym == SDLK_DOWN) {
			fader-=5;
			cout << "val: " << (unsigned int)fader << endl;
		}
	};

	return true;
}

bool Hardware::OnGUIEvent(GUIEvent*const event)
{
	if (event->type == GUIEvent::POT_CHANGED) {
		if (event->name == "slider1")faders[0] = (unsigned char)event->value;
		if (event->name == "slider2")faders[1] = (unsigned char)event->value;
		if (event->name == "slider3")faders[2] = (unsigned char)event->value;
		if (event->name == "slider4")faders[3] = (unsigned char)event->value;
		if (event->name == "slider5")faders[4] = (unsigned char)event->value;
		if (event->name == "slider6")faders[5] = (unsigned char)event->value;
		if (event->name == "slider7")faders[6] = (unsigned char)event->value;
		if (event->name == "slider8")faders[7] = (unsigned char)event->value;
	}
	return true;
}

bool Hardware::OnWindowEvent(WindowEvent*const event)
{		
	for (int i = 0; i < 8; i++) {
		Entity* e = getObjectByName("SoundCh"+std::to_string(i+1));
		if (e && e->getTypeId() == GUISound::TypeID()) {
			GUISound* sound = (GUISound*)e;

			float fv = (float)faders[i] / 256.0f;

			float rms = fv * min(Math::Pow(sound->interpolatedRmsLevel, 0.2), 1.0f);

			fadersRms[i] = int(rms * 256.0f);
		}
	}

	sendTimer += event->frametime;

	float freq = 1.0f / 10;

	if (sendTimer > freq) {
		sendTimer = 0;

		unsigned int frameSize = prepareSerialFrame();												
		Write(c_data, frameSize);
	}
	return 1;
}

unsigned int Hardware::prepareSerialFrame() {

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

/*	
	cout << (unsigned int)c_data[4] << ",";
	cout << (unsigned int)c_data[5] << ",";
	cout << (unsigned int)c_data[6] << ",";
	cout << (unsigned int)c_data[7] << ",";
	cout << (unsigned int)c_data[8] << ",";
	cout << (unsigned int)c_data[9] << ",";
	cout << (unsigned int)c_data[10] << ",";
	cout << (unsigned int)c_data[11] << ",";
	cout <<"crc: "<<(unsigned int)c_data[counter-1] << endl;
*/
	return counter;
}

void Hardware::Render(Renderer*)
{
	glPushMatrix();
	glTranslatef(m_LocalPos.x, m_LocalPos.y, 0);

	float size = 65.0f;
	float hSize = size / 2;	
	
	int index = 0;

	for (int iy = 0; iy < 2; iy++) {
		for (int ix = 0; ix < 4; ix++) {

			float color = 0;

			if(faderMode == 1)color = (float)faders[index] / 256.0f;
			else color = (float)fadersRms[index] / 256.0f;

			glEnable(GL_BLEND);
			glPushMatrix();
				glTranslatef(hSize+(size+hSize)*ix, hSize + (size + hSize)*iy, 0);
				//glColor4f(color, color, color, 1);
				glColor4f(1, 1, 1, color);
				glLineWidth(2);
				glBegin(GL_POLYGON);
					glVertex2f(-hSize, -hSize);
					glVertex2f(-hSize, hSize);
					glVertex2f(hSize, hSize);
					glVertex2f(hSize, -hSize);
				glEnd();
				glBegin(GL_LINE_LOOP);
					glColor4f(0.4f, 0.4f, 0.6f, 0.9f);
					glVertex2f(-hSize, hSize);
					glVertex2f(hSize, hSize);
					glColor4f(1, 1, 1, 0.5f);
					glVertex2f(hSize, -hSize);
					glVertex2f(-hSize, -hSize);
				glEnd();

				glLineWidth(1);

				glPushMatrix();
					glTranslatef(-hSize/2,hSize/2, 0);
					glColor4f(0.4f, 0.4f, 0.6f, 0.9f);
					glEnable(GL_TEXTURE_2D);
					Font::m_DefaultInstance.DrawCenter(Utils::IntToString((int)index+1), 0.45f);
					glDisable(GL_TEXTURE_2D);
				glPopMatrix();

			glPopMatrix();

			index++;
		}
	}

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
