#include "../Geo4.h"

#ifndef NO_OPENGL

CLASS_DECLARATION(GUITextInput);

GUITextInput::GUITextInput() : renderable(),
	renderableActive(),
	styleSheet(),
	styleName(),
	styleNameActive(),
	maxCharacters(50),
	keyDownDelay(0.2f),
	keyDownTimer(0),
	backspaceDown(false),
	inputMask(),
	doInputMasking(false)
{	}

GUITextInput::~GUITextInput()
{	}

void GUITextInput::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUITextInput::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUITextInput::OnWindowEvent);

	resourceManager->Get(styleSheet, "style/styles.cfo");

	renderable.style = &styleSheet->get(styleName);
	renderableActive.style = &styleSheet->get(styleNameActive);
}

void GUITextInput::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);	

	styleName = "inputText";
	styleNameActive = "inputTextActive";

	if (node->getValueString("style", styleName)) {
		styleNameActive = styleName + "Active";
	}
	node->getValueString("styleActive", styleNameActive);

	node->getValueInt("length", maxCharacters);

	string regex = "";
	if (node->getValueString("mask", regex) && regex.length() > 0) {
		inputMask = std::regex(regex);
		doInputMasking = true;
	}

	renderable.size = m_Size;
	renderableActive.size = m_Size;
}

bool GUITextInput::OnWindowEvent(WindowEvent*const event)
{
	if (event->eventType == WindowEvent::WINDOW_CREATED) {

		renderable.update();
		renderableActive.update();

		renderable.setText(m_Title);
		renderableActive.setText(m_Title);

	}
	if (backspaceDown) {
		keyDownTimer += event->frametime;
		if (keyDownTimer > keyDownDelay) {
			if (!m_Title.empty()) {
				m_Title.erase(m_Title.size() - 1);
			}
			renderable.setText(m_Title);
			renderableActive.setText(m_Title);
			keyDownTimer = 0;
		}
	}
	return 1;
}

bool GUITextInput::OnGUIInputEvent(GUIInputEvent*const event)
{
	if (isVisible() == 0)return 1;
	
	std::smatch base_match;
	std::string tmpStr = "";
	switch (event->type) {
	case GUIInputEvent::EventType::KEYPRESS:
		backspaceDown = false;
		if (isFocused()) {
			//backspace
			if (event->keyCode == 42 && !m_Title.empty()) {
				backspaceDown = true;
			}
		}
		break;
	case GUIInputEvent::EventType::KEYUP:
		backspaceDown = false;
		if (isFocused()) {
			//backspace
			if (event->keyCode == 42 && !m_Title.empty()) {
				m_Title.erase(m_Title.size() - 1);
				renderable.setText(m_Title);
				renderableActive.setText(m_Title);
				SendEvent(new GUIEvent(GUIEvent::TEXT_INPUT_CHANGED));
			}
			//enter - leave focus
			if (event->keyCode == 40 || event->keyCode == 88) {
				setFocus(false);
			}
		}
		break;
	case GUIInputEvent::EventType::TEXENTER:
		backspaceDown = false;
		if (isFocused()) {
			if (doInputMasking) {
				tmpStr = m_Title + event->textInput;
				if (!std::regex_match(tmpStr, base_match, inputMask)) break;
			}
			m_Title = m_Title + event->textInput;
			while (m_Title.size() > maxCharacters) {
				m_Title.erase(m_Title.size() - 1);
			}
			SendEvent(new GUIEvent(GUIEvent::TEXT_INPUT_CHANGED));
			renderable.setText(m_Title);
			renderableActive.setText(m_Title);
		}
		break;
	case GUIInputEvent::EventType::ENTERFOCUS:

		//Show onscreen keyboard for touch devices

		break;
	};
	return 1;
}

void GUITextInput::PreRender(Renderer* r) {
	GUIEntity::PreRender(r);
}

void GUITextInput::Render(Renderer* rnd){
	if(m_Focused)renderableActive.Draw();
	else renderable.Draw();
}

void GUITextInput::PostRender() {
	GUIEntity::PostRender();
}

#endif