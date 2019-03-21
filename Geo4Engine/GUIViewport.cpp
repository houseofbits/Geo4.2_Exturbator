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

void GUIViewport::keyPressedEvent(unsigned int keysym) {
	SendEvent(new GUIInputEvent(GUIInputEvent::EventType::KEYPRESS, keysym));
}

void GUIViewport::keyDownEvent(unsigned int keysym) {
	SendEvent(new GUIInputEvent(GUIInputEvent::EventType::KEYDOWN, keysym));
}
void GUIViewport::keyUpEvent(unsigned int keysym) {
	SendEvent(new GUIInputEvent(GUIInputEvent::EventType::KEYUP, keysym));
}
void GUIViewport::mouseMoveEvent(Vector2 mousePos) {
	//SendEvent(new GUIInputEvent(GUIInputEvent::EventType::MOUSEMOVE, sdlevent->key.keysym.sym));
	//1.Get mouse cursor pos
	//2.Get object under mouse cursor
	//3.Send events
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
	glOrtho(0.0, window_width, 0, window_height, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glClearStencil(0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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
