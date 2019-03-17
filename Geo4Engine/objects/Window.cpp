
#include "../Geo4.h"

//#pragma comment(lib, "../Lib/SDL2/lib/x86/SDL2main.lib")
//#pragma comment(lib, "../Lib/SDL2/lib/x86/SDL2.lib")

#pragma comment(lib, "../Lib/SDL2/lib/SDL2main.lib")
#pragma comment(lib, "../Lib/SDL2/lib/SDL2.lib")

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "../../Lib/SDL2/include/SDL_syswm.h"

#include "../../Lib/SDL2/include/SDL_syswm.h"

using namespace std;

CLASS_DECLARATION(SDLWindow);

/*
IDirectInput8* dev;
static BOOL CALLBACK staticEnumerateGameControllers(LPCDIDEVICEINSTANCE devInst, LPVOID pvRef)
{
	wcout <<"Prod name: "<<devInst->tszProductName << ", " << devInst->tszInstanceName << endl;

	HRESULT result;
	LPDIRECTINPUTDEVICE8 device;
	LPDIRECTINPUTDEVICE8 InputDevice;
	DIPROPGUIDANDPATH dipdw2;

	result = dev->CreateDevice(devInst->guidInstance, &device, NULL);	// IDirectInput8_CreateDevice(dinput, &(pdidInstance->guidInstance), &device, NULL);
	if (FAILED(result)) {
		cout << "failed CreateDevice" << endl;
		return DIENUM_CONTINUE;
	}

	result = device->QueryInterface(IID_IDirectInputDevice8, (LPVOID *)&InputDevice);// IDirectInputDevice8_QueryInterface(device, &IID_IDirectInputDevice8, (LPVOID *)&InputDevice);
	IDirectInputDevice8_Release(device);
	if (FAILED(result)) {
		cout << "failed QueryInterface" << endl;
		return DIENUM_CONTINUE;
	}
	dipdw2.diph.dwSize = sizeof(dipdw2);
	dipdw2.diph.dwHeaderSize = sizeof(dipdw2.diph);
	dipdw2.diph.dwObj = 0; // device property
	dipdw2.diph.dwHow = DIPH_DEVICE;

	result = InputDevice->GetProperty(DIPROP_GUIDANDPATH, &dipdw2.diph);	////IDirectInputDevice8_GetProperty(InputDevice, DIPROP_GUIDANDPATH, &dipdw2.diph);
	IDirectInputDevice8_Release(InputDevice);
	if (FAILED(result)) {
		cout << "failed GetProperty" << endl;
		return DIENUM_CONTINUE; 
	}

	wcout << "GUID Path: "<<dipdw2.wszPath << endl;


	return true;
}/**/

SDLWindow::SDLWindow(void) : Entity(), 
	EventHandler(),
	window_title("GEO4"),
	rotate(0),
	width(1024),
	height(768),
	window(0),
	glcontext(),
	time(0),
	frame_time(0),
	joystick(0)
{ 

}

SDLWindow::~SDLWindow(void)
{
	SDL_GL_DeleteContext(glcontext);
	SDL_Quit();
}

void SDLWindow::Initialise(EventManager*const event_manager, SceneManager* mgr)
{	
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &SDLWindow::OnWindowEvent);

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	cout << "SDL Joysticks: " << SDL_NumJoysticks() << endl;

	if (SDL_NumJoysticks() > 0) {
		joystick = SDL_JoystickOpen(0);

		if (joystick) {
			printf("Opened Joystick 0\n");
			printf(" Name: %s\n", SDL_JoystickNameForIndex(0));
			printf(" Number of Axes: %d\n", SDL_JoystickNumAxes(joystick));
			//printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
			//printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
		}
		else {
			printf("Couldn't open Joystick 0\n");
		}

	}


	Uint32 flags = SDL_WINDOW_OPENGL;

	if (fullscreen == "FULL") {
		flags = flags | SDL_WINDOW_FULLSCREEN;
	}
	else if (fullscreen == "FULL_DESKTOP") {
		flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
		width = 0;
		height = 0;
	}

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 6);

	window = SDL_CreateWindow(window_title.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height,
		flags);

	SDL_GetWindowSize(window, &width, &height);

	glcontext = SDL_GL_CreateContext(window);

	GLenum errr = glewInit();




	SendEvent(new WindowEvent(WindowEvent::WINDOW_CREATED, this));

	SendEvent(new WindowEvent(0, 0, this));

	Font::m_DefaultInstance.Load("arial.glf");

	glEnable(GL_MULTISAMPLE);
}

void SDLWindow::Deserialize(CFONode* node, ResourceManager* mgr)
{	
	Entity::Deserialize(node, mgr);

	node->getValueString("fullscreen", fullscreen);
	node->getValueInt("width", width);
	node->getValueInt("height", height);
	node->getValueString("title", window_title);

}

bool SDLWindow::OnWindowEvent(WindowEvent*const event)
{
	SceneManager* mgr = getSceneManager();

	if (!mgr)return false;

	float tme = (float)SDL_GetTicks() / 1000.0f;
	frame_time = tme - time;
	time = tme;

	SendEvent(new WindowEvent(frame_time, time, this));

	mgr->event_dispatcher.SetCurrentTime(time);

	SDL_Event sdlevent;

	while (SDL_PollEvent(&sdlevent)) {

		//cout << sdlevent.type << endl;

		switch(sdlevent.type) {
			case SDL_WINDOWEVENT:
				switch (sdlevent.window.event) {
					case SDL_WINDOWEVENT_CLOSE:
					
						//EXIT
						return false;

						break;
				};
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
			case SDL_JOYAXISMOTION:

				if (sdlevent.type == SDL_KEYUP && sdlevent.key.keysym.sym == SDLK_F4)return false;

				SendEvent(new InputEvent(new SDL_Event(sdlevent)));

				break;
		};
	};

	return true;
}

void SDLWindow::PreRender(Renderer*){		}

void SDLWindow::Render(Renderer* rnd){		}

void SDLWindow::PostRender()
{
	SDL_GL_SwapWindow(window);
}