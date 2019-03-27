#include "../Geo4.h"

CLASS_DECLARATION(GUIButton);

GUIButton::GUIButton() : renderableActive(),
	renderableHover(),
	renderablePressed(),
	styleSheet(),
	styleActiveName(),
	styleHoverName(),
	stylePressedName()
{	}

GUIButton::~GUIButton()
{	}

void GUIButton::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	resourceManager->Get(styleSheet, "style/styles.cfo");

	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUIButton::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUIButton::OnWindowEvent);

	renderableActive.style = &styleSheet->get(styleActiveName);
	renderableHover.style = &styleSheet->get(styleHoverName);
	renderablePressed.style = &styleSheet->get(stylePressedName);

}

void GUIButton::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);

	styleActiveName = "buttonDefaultActive";
	styleHoverName = "buttonDefaultHover";
	stylePressedName = "buttonDefaultPressed";

	node->getValueString("styleActive", styleActiveName);
	node->getValueString("styleHover", styleHoverName);
	node->getValueString("stylePressed", stylePressedName);

	renderableActive.size = m_Size;
	renderableHover.size = m_Size;
	renderablePressed.size = m_Size;
}

bool GUIButton::OnWindowEvent(WindowEvent*const event)
{
	if (event->event_type == WindowEvent::WINDOW_CREATED) {

		renderableActive.update();
		renderableHover.update();
		renderablePressed.update();

		renderableActive.setText(m_Title);
		renderableHover.setText(m_Title);
		renderablePressed.setText(m_Title);

	}
	return 1;
}

bool GUIButton::OnGUIInputEvent(GUIInputEvent*const event)
{
	if (isVisible() == 0)return 1;

	switch (event->type) {
	case GUIInputEvent::EventType::MOUSEUP:
		clickState = 0;
		break;
	case GUIInputEvent::EventType::MOUSEDOWN:
		clickState = 1;
		break;
	case GUIInputEvent::EventType::MOUSEENTER:
		hoverState = 1;
		break;
	case GUIInputEvent::EventType::MOUSELEAVE:
		hoverState = 0;
		clickState = 0;
		break;
	};
	return 1;
}

void GUIButton::PreRender(Renderer* r){
	GUIEntity::PreRender(r);
}

void GUIButton::Render(Renderer* rnd)
{
	if (!hoverState && !clickState)renderableActive.Draw();
	else if (hoverState && !clickState)renderableHover.Draw();
	else if (clickState)renderablePressed.Draw();

}

void GUIButton::PostRender() {
	GUIEntity::PostRender();
}
