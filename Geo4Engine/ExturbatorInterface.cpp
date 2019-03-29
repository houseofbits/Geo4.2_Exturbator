#include "Geo4.h"
#include <crtdbg.h>
#include <tchar.h>
#include <windows.h>

using namespace std;

CLASS_DECLARATION(ExturbatorInterface);

ExturbatorInterface::ExturbatorInterface(void) : Entity()
{	}

ExturbatorInterface::~ExturbatorInterface(void)
{	}

void ExturbatorInterface::Initialise(EventManager*const event_manager, ResourceManager*const resourceManager)
{	
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &ExturbatorInterface::OnWindowEvent);
	event_manager->RegisterEventReceiver(this, &ExturbatorInterface::OnGUIEvent);
}

void ExturbatorInterface::Deserialize(CFONode* node)
{	
	Entity::Deserialize(node);
}

bool ExturbatorInterface::OnWindowEvent(WindowEvent*const event){

	ExturbatorHardware* hardware = getObjectByName<ExturbatorHardware>("hardware");
	GUIWindow* window = getObjectByName<GUIWindow>("hardwareDiscoveryWarningModal");
	if (hardware && window) {
		if (!hardware->portIsValid || hardware->waitingPortAnswer)
			window->setVisible(true);
		else
			window->setVisible(false);
	}

	return 1;
}

bool ExturbatorInterface::OnGUIEvent(GUIEvent*const event) {

	if (event->type == GUIEvent::BUTTON_PRESSED) {

		if (event->m_Sender->getName() == "buttonCheckConnected") {
			cout << "check connection button" << endl;
		}

	}

	return 1;
}