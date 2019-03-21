#include "Geo4.h"

CLASS_DECLARATION(GUIText);

GUIText::GUIText() : renderable(),
	styleSheet()
{	}

GUIText::~GUIText()
{	}

void GUIText::Initialise(EventManager*const event_manager, SceneManager* mgr)
{
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &GUIText::OnGUIInputEvent);
	event_manager->RegisterEventReceiver(this, &GUIText::OnWindowEvent);
}

void GUIText::Deserialize(CFONode* node, ResourceManager* mgr)
{
	GUIEntity::Deserialize(node, mgr);

	mgr->Get(styleSheet, "styles.cfo");

	string style = "textDefault";

	node->getValueString("style", style);

	renderable.size = m_Size;
	renderable.style = &styleSheet->get(style);
}

bool GUIText::OnWindowEvent(WindowEvent*const event)
{
	if (event->event_type == WindowEvent::WINDOW_CREATED) {

		renderable.update();

		renderable.setText(m_Title);

	}
	return 1;
}

bool GUIText::OnGUIInputEvent(GUIInputEvent*const event)
{
	if (isVisible() == 0)return 1;


	return 1;
}

void GUIText::PreRender(Renderer*)
{
	glPushMatrix();
	glTranslatef(m_LocalPos.x, m_LocalPos.y, 0);
}

void GUIText::Render(Renderer* rnd)
{
	if (isVisible() == 0)return

	glEnable(GL_BLEND);
	
	renderable.Draw();

}

void GUIText::PostRender()
{
	glPopMatrix();
}
