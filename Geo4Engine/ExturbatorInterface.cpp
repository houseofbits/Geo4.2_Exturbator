#include "Geo4.h"

#ifndef NO_OPENGL

#include <crtdbg.h>
#include <tchar.h>
#include <windows.h>

using namespace std;

CLASS_DECLARATION(ExturbatorInterface);

ExturbatorInterface::ExturbatorInterface(void) : Entity(), 
	hardware(0)
{	}

ExturbatorInterface::~ExturbatorInterface(void)
{	}

void ExturbatorInterface::Initialise(EventManager*const event_manager, ResourceManager*const resourceManager)
{	
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &ExturbatorInterface::OnWindowEvent);
	event_manager->RegisterEventReceiver(this, &ExturbatorInterface::OnGUIEvent);

	hardware = getObjectByName<ExturbatorRequests>("hardwareRequests");
	if (!hardware)cout << "ExturbatorRequests module not found" << endl;
}

void ExturbatorInterface::Deserialize(CFONode* node)
{	
	Entity::Deserialize(node);
}

bool ExturbatorInterface::OnWindowEvent(WindowEvent*const event){

	/*
	Hardware* hardware = getObjectByName<Hardware>("hardware");
	GUIWindow* window = getObjectByName<GUIWindow>("hardwareDiscoveryWarningModal");
	if (hardware && window) {
		if (!hardware->isPortValid())
			window->setVisible(true);
		else
			window->setVisible(false);
	}
	*/
	//ExturbatorRequests* hardware = getObjectByName<ExturbatorRequests>("hardwareRequests");
	if (hardware) {

		//Check status

		//Update form data

	}

	return 1;
}

bool ExturbatorInterface::OnGUIEvent(GUIEvent*const event) {

	if (event->type == GUIEvent::SLIDER_CHANGED) {
		//Winder settings
		if (event->m_Sender->getName() == "sliderWinderSpeed") {
			GUISlider* s = (GUISlider*)event->m_Sender;
			if (s) {
				GUITextInput* t = getObjectByName<GUITextInput>("inputWinderSpeed");
				if (t) {
					t->setTitle(Utils::FloatToString(s->value, 4));
				}
			}
		}
	}

	if (event->type == GUIEvent::TEXT_INPUT_CHANGED) {
		//Winder settings
		if (event->m_Sender->getName() == "inputWinderSpeed") {
			GUITextInput* i = (GUITextInput*)event->m_Sender;
			if (i) {
				float value = (float)atof(i->m_Title.c_str());
				GUISlider* t = getObjectByName<GUISlider>("sliderWinderSpeed");
				if (t) {
					t->setValue(value);
				}
			}
		}
	}

	if (event->type == GUIEvent::BUTTON_PRESSED) {

		if (event->m_Sender->getName() == "buttonCheckConnected") {
			cout << "check connection button" << endl;
		}
		if (event->m_Sender->getName() == "buttonZone1Settings") {

			GUIWindow* w = getObjectByName<GUIWindow>("windowZoneSettings");
			if (w) {
				w->setTitle("Zone1 settings");
				w->setVisible(true);
			}
		}
		if (event->m_Sender->getName() == "buttonZone2Settings") {
			GUIWindow* w = getObjectByName<GUIWindow>("windowZoneSettings");
			if (w) {
				w->setTitle("Zone2 settings");
				w->setVisible(true);
			}
		}
		if (event->m_Sender->getName() == "buttonZoneSettingsClose") {
			GUIWindow* w = getObjectByName<GUIWindow>("windowZoneSettings");
			if (w)w->setVisible(false);
		}
		if (event->m_Sender->getName() == "buttonPresets") {
			GUIWindow* w = getObjectByName<GUIWindow>("windowPresets");
			if (w)w->setVisible(true);
		}
		if (event->m_Sender->getName() == "buttonLoadPreset") {
			GUIWindow* w = getObjectByName<GUIWindow>("windowPresets");
			if (w)w->setVisible(false);
			GUIList* l = getObjectByName<GUIList>("presetsList");
			if (l) {
				GUIListItem* item = l->getSelectedItem();
				if (item) {
					GUILayer* titleBar = getObjectByName<GUILayer>("layerProgramTitle");
					if (titleBar) {
						titleBar->setTitle("Preset - "+item->m_Title);
					}
				}
			}
		}
		if (event->m_Sender->getName() == "buttonClosePreset") {
			GUIWindow* w = getObjectByName<GUIWindow>("windowPresets");
			if (w)w->setVisible(false);
		}
		if (event->m_Sender->getName() == "buttonShutdown") {
			return 0;
		}
		//Puller
		if (event->m_Sender->getName() == "buttonPullerSettings") {
			GUIWindow* w = getObjectByName<GUIWindow>("windowPullerSettings");
			if (w)w->setVisible(true);
		}
		if (event->m_Sender->getName() == "buttonClosePuller") {
			GUIWindow* w = getObjectByName<GUIWindow>("windowPullerSettings");
			if (w)w->setVisible(false);
		}
		//Winder
		if (event->m_Sender->getName() == "buttonWinderSettings") {
			GUIWindow* w = getObjectByName<GUIWindow>("windowWinderSettings");
			if (w)w->setVisible(true);
		}
		if (event->m_Sender->getName() == "buttonCloseWinder") {
			GUIWindow* w = getObjectByName<GUIWindow>("windowWinderSettings");
			if (w)w->setVisible(false);
		}
		if (event->m_Sender->getName() == "buttonWinderStart") {
			GUIButton* w = getObjectByName<GUIButton>("buttonWinderStart");
			if (w)w->setVisible(false);
			w = getObjectByName<GUIButton>("buttonWinderStop");
			if (w)w->setVisible(true);
		}
		if (event->m_Sender->getName() == "buttonWinderStop") {
			GUIButton* w = getObjectByName<GUIButton>("buttonWinderStart");
			if (w)w->setVisible(true);
			w = getObjectByName<GUIButton>("buttonWinderStop");
			if (w)w->setVisible(false);
		}
		//Reset winder modal
		if (event->m_Sender->getName() == "buttonWinderReset") {
			GUIWindow* w = getObjectByName<GUIWindow>("windowWinderReset");
			if (w)w->setVisible(true);
		}
		if (event->m_Sender->getName() == "buttonResetWinderCancel") {
			GUIWindow* w = getObjectByName<GUIWindow>("windowWinderReset");
			if (w)w->setVisible(false);
		}


		/*
		if (event->m_Sender->getName() == "buttonExtruderOn") {
			if (hardware)hardware->extruderState = true;
			GUIButton* b = getObjectByName<GUIButton>("buttonExtruderOn");
			b->setVisible(false);
			b = getObjectByName<GUIButton>("buttonExtruderOff");
			b->setVisible(true);
		}
		if (event->m_Sender->getName() == "buttonExtruderOff") {
			if (hardware)hardware->extruderState = false;
			GUIButton* b = getObjectByName<GUIButton>("buttonExtruderOn");
			b->setVisible(true);
			b = getObjectByName<GUIButton>("buttonExtruderOff");
			b->setVisible(false);
		}
		*/
	}

	return 1;
}

#endif