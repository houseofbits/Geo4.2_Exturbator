#include "Geo4.h"

CLASS_DECLARATION(GUIButton);

GUIButton::GUIButton() : renderable(),
	styleSheet(),
	styleActiveName(),
	styleHoverName(),
	stylePressedName()
{	}

GUIButton::~GUIButton()
{	}

void GUIButton::Initialise(EventManager*const event_manager, SceneManager* mgr)
{
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &GUIButton::OnInputEvent);
	event_manager->RegisterEventReceiver(this, &GUIButton::OnWindowEvent);
}

void GUIButton::Deserialize(CFONode* node, ResourceManager* mgr)
{
	GUIEntity::Deserialize(node, mgr);

	mgr->Get(styleSheet, "styles.cfo");

	styleActiveName = "buttonDefaultActive";
	styleHoverName = "buttonDefaultHover";
	stylePressedName = "buttonDefaultPressed";

	node->getValueString("styleActive", styleActiveName);
	node->getValueString("styleHover", styleHoverName);
	node->getValueString("stylePressed", stylePressedName);

	renderable.size = m_Size;
	renderable.style = &styleSheet->get(styleActiveName);
}

bool GUIButton::OnWindowEvent(WindowEvent*const event)
{
	if (event->event_type == WindowEvent::WINDOW_CREATED) {

		renderable.generateGeometry();

	}
	return 1;
}

bool GUIButton::OnInputEvent(InputEvent*const event)
{
	if (isVisible() == 0)return 1;

	return 1;
}

void GUIButton::PreRender(Renderer*)
{

}

void GUIButton::Render(Renderer* rnd)
{
	glPushMatrix();
	glTranslatef(m_LocalPos.x, m_LocalPos.y, 0);	
	renderable.Draw();
	glPopMatrix();
}

void GUIButton::PostRender()
{

}
