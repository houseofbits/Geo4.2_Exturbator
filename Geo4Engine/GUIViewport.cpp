#include "Geo4.h"

CLASS_DECLARATION(GUIViewport);

GUIViewport::GUIViewport() :
	window_width(800),
	window_height(600)
{	}


GUIViewport::~GUIViewport()
{	}


void GUIViewport::Initialise(EventManager*const event_manager, SceneManager* mgr)
{
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &GUIViewport::OnWindowEvent);

}

void GUIViewport::Deserialize(CFONode* node, ResourceManager* mgr)
{
	GUIEntity::Deserialize(node, mgr);


}

bool GUIViewport::OnWindowEvent(WindowEvent*const event)
{
	window_width = event->window->width;
	window_height = event->window->height;

	return 1;
}

void GUIViewport::PreRender(Renderer*)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, window_width, window_height, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);

}

void GUIViewport::Render(Renderer* rnd)
{
	glPushMatrix();

	glPopMatrix();
}

void GUIViewport::PostRender()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
