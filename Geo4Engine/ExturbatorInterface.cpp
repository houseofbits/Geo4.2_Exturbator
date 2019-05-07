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
	/**/
	//ExturbatorRequests* hardware = getObjectByName<ExturbatorRequests>("hardwareRequests");
	if (hardware) {

		//Check status

		//Update form data

	}

	return 1;
}

bool ExturbatorInterface::OnGUIEvent(GUIEvent*const event) {

	try {

		if (event->type == GUIEvent::SLIDER_CHANGED) {

			switch (event->m_Sender->getHashName()) {

			//Winder settings
			case hashStr("sliderWinderSpeed"):
				instanceOf<GUITextInput>("inputWinderSpeed")->setTitle(Utils::FloatToString(event->m_Sender->instanceOf<GUISlider>()->value, 4));
				break;
			default:
				break;
			}
		}

		if (event->type == GUIEvent::TEXT_INPUT_CHANGED) {

			float value = 0;

			switch (event->m_Sender->getHashName()) {

			//Winder settings
			case hashStr("inputWinderSpeed"):
				value = (float)atof(event->m_Sender->instanceOf<GUITextInput>()->m_Title.c_str());
				instanceOf<GUISlider>("sliderWinderSpeed")->setValue(value);
				break;
			default:
				break;
			}
		}

		if (event->type == GUIEvent::BUTTON_PRESSED) {

			GUIListItem* item = 0;

			switch (event->m_Sender->getHashName()){

			case hashStr("buttonCheckConnected"):
				cout << "check connection button" << endl;
				break;
			case hashStr("buttonZone1Settings"):
				instanceOf<GUIWindow>("windowZoneSettings")->setTitle("Zone1 settings");
				instanceOf<GUIWindow>("windowZoneSettings")->setVisible(true);
				break;
			case hashStr("buttonZone2Settings"):
				instanceOf<GUIWindow>("windowZoneSettings")->setTitle("Zone2 settings");
				instanceOf<GUIWindow>("windowZoneSettings")->setVisible(true);
				break;
			//etc...


			case hashStr("buttonZoneSettingsClose"):
				instanceOf<GUIWindow>("windowZoneSettings")->setVisible(false);
				break;
			case hashStr("buttonPresets"):
				instanceOf<GUIWindow>("windowPresets")->setVisible(true);
				break;
			case hashStr("buttonLoadPreset"):
				instanceOf<GUIWindow>("windowPresets")->setVisible(false);
				item = instanceOf<GUIList>("presetsList")->getSelectedItem();
				if (item)instanceOf<GUILayer>("layerProgramTitle")->setTitle("Preset - " + item->m_Title);
				break;
			case hashStr("buttonClosePreset"):
				instanceOf<GUIWindow>("windowPresets")->setVisible(false);
				break;
			case hashStr("buttonShutdown"):
				return 0;
				break;				
			//Puller
			case hashStr("buttonPullerSettings"):
				instanceOf<GUIWindow>("windowPullerSettings")->setVisible(true);
				break;
			case hashStr("buttonClosePuller"):
				instanceOf<GUIWindow>("windowPullerSettings")->setVisible(false);
				break;
			//Winder
			case hashStr("buttonWinderSettings"):
				instanceOf<GUIWindow>("windowWinderSettings")->setVisible(true);
				break;
			case hashStr("buttonCloseWinder"):
				instanceOf<GUIWindow>("windowWinderSettings")->setVisible(false);
				break;
			case hashStr("buttonWinderStart"):
				event->m_Sender->instanceOf<GUIButton>()->setVisible(false);
				instanceOf<GUIButton>("buttonWinderStop")->setVisible(true);
				break;
			case hashStr("buttonWinderStop"):
				event->m_Sender->instanceOf<GUIButton>()->setVisible(false);
				instanceOf<GUIButton>("buttonWinderStart")->setVisible(true);
				break;
			//Reset winder modal
			case hashStr("buttonWinderReset"):
				//Show winder reset modal window
				instanceOf<GUIWindow>("windowWinderReset")->setVisible(true);
				break;
			case hashStr("buttonResetWinderReset"):
				//TODO Reset winder position
				instanceOf<GUIWindow>("windowWinderReset")->setVisible(false);
				break;
			case hashStr("buttonResetWinderSave"):
				//TODO Save process to log db and reset winder position
				instanceOf<GUIWindow>("windowWinderReset")->setVisible(false);
				break;
			case hashStr("buttonResetWinderCancel"):
				instanceOf<GUIWindow>("windowWinderReset")->setVisible(false);
				break;
			default:
				break;
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
	}
	catch (int e) {
		cout << "Exception (ExturbatorInterface) " << e << endl;
	}
	return 1;
}

#endif