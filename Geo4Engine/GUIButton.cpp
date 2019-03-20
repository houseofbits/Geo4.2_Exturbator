#include "Geo4.h"

CLASS_DECLARATION(GUIButton);

GUIButton::GUIButton() : renderableActive(),
	renderableHover(),
	renderablePressed(),
	styleSheet()
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

	string styleActiveName = "buttonDefaultActive";
	string styleHoverName = "buttonDefaultHover";
	string stylePressedName = "buttonDefaultPressed";

	node->getValueString("styleActive", styleActiveName);
	node->getValueString("styleHover", styleHoverName);
	node->getValueString("stylePressed", stylePressedName);

	renderableActive.size = m_Size;
	renderableActive.style = &styleSheet->get(styleActiveName);

	renderableHover.size = m_Size;
	renderableHover.style = &styleSheet->get(styleHoverName);

	renderablePressed.size = m_Size;
	renderablePressed.style = &styleSheet->get(stylePressedName);
}

bool GUIButton::OnWindowEvent(WindowEvent*const event)
{
	if (event->event_type == WindowEvent::WINDOW_CREATED) {

		renderableActive.generateGeometry();
		renderableHover.generateGeometry();
		renderablePressed.generateGeometry();

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
	if (isVisible() == 0)return

	glEnable(GL_BLEND);

	glPushMatrix();

	glTranslatef(m_LocalPos.x, m_LocalPos.y, 0);	
	
	renderableActive.Draw(m_Title);

	glPopMatrix();
}

void GUIButton::PostRender()
{

}
