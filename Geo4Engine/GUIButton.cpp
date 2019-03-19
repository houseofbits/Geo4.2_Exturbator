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

	style.radiusBottomLeft = 50;
	style.radiusBottomRight = 0;
	style.radiusTopLeft = 0;
	style.radiusTopRight = 0;

	style.borderSize = 10;
	style.borderSizeLeft = 10;
	style.borderSizeRight = 5;
	style.borderSizeTop = 20;
	style.borderSizeBottom = 15;

	style.borderColorBottom = Vector4(0, 0, 1, 1);
	style.borderColorTop = Vector4(0, 0.6, 0, 1);
	style.borderColorRight = Vector4(0.6, 0, 0, 1);
	style.borderColorLeft = Vector4(0.6, 0, 0.6, 1);

	style.shadowSize = 15;
	style.shadowHardness = 0;
	style.shadowX = 5;
	style.shadowY = 10;
	style.shadowColor = Vector4(0.4,0.4,0.5,1);

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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
