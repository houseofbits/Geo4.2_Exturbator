#include "Geo4.h"

CLASS_DECLARATION(GUIButton);

GUIButton::GUIButton() : renderable()
{
}


GUIButton::~GUIButton()
{
}


void GUIButton::Initialise(EventManager*const event_manager, SceneManager* mgr)
{
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &GUIButton::OnInputEvent);
	event_manager->RegisterEventReceiver(this, &GUIButton::OnWindowEvent);

	

}

void GUIButton::Deserialize(CFONode* node, ResourceManager* mgr)
{
	GUIEntity::Deserialize(node, mgr);

	style.radiusBottomLeft = 70;
	style.radiusBottomRight = 0;
	style.radiusTopLeft = 10;
	style.radiusTopRight = 20;
	style.borderSize = 10;
	style.shadowSize = 25;
	style.backgroundFill = GUIStyle::FillType::SOLID;
	style.backgroundColor = Vector4(1,1,0,0.7f);

	renderable.size.x = 300;
	renderable.size.y = 300;
	renderable.style = &style;

}

bool GUIButton::OnWindowEvent(WindowEvent*const event)
{
	//Set default values on window creation
	if (event->event_type == WindowEvent::WINDOW_CREATED) {
		
		cout << "windowcreated" << endl;

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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Renderer::DrawCapsule(300, 300, 100, Color::RGBAf(1,0,0,1), Color::RGBAf(0, 1, 0, 1));

	renderable.Draw();


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
