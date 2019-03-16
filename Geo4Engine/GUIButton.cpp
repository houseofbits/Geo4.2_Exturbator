#include "Geo4.h"

CLASS_DECLARATION(GUIButton);

GUIButton::GUIButton()
{
}


GUIButton::~GUIButton()
{
}


void GUIButton::Initialise(EventManager*const event_manager, SceneManager* mgr)
{
	event_manager->RegisterEventHandler(this);

}

void GUIButton::Deserialize(CFONode* node, ResourceManager* mgr)
{
	GUIEntity::Deserialize(node, mgr);


}

void GUIButton::PreRender(Renderer*)
{

}

void GUIButton::Render(Renderer* rnd)
{
	glPushMatrix();
	/*
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(m_Size.x, 0.0);
	glVertex2f(m_Size.x, m_Size.y);
	glVertex2f(0.0, m_Size.y);
	glEnd();
	*/
	glPopMatrix();
}

void GUIButton::PostRender()
{

}
