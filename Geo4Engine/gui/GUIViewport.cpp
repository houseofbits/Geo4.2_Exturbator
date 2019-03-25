#include "../Geo4.h"

CLASS_DECLARATION(GUIViewport);

GUIViewport::GUIViewport() :
	window_width(800),
	window_height(600),
	hoverObject(0)
{	}


GUIViewport::~GUIViewport()
{	}


void GUIViewport::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUIViewport::OnWindowEvent);
}

void GUIViewport::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);
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
void GUIViewport::mouseMoveEvent(Vector2 mousePos, Vector2 mouseRel, bool leftButton, bool rightButton) {

	SendEvent(new GUIInputEvent(GUIInputEvent::EventType::MOUSEMOVE, mousePos, mouseRel, leftButton, rightButton));

	//Drag event
	if (hoverObject && leftButton) {

		SendEvent(new GUIInputEvent(GUIInputEvent::EventType::DRAG, mousePos, mouseRel, leftButton, rightButton), hoverObject);

	}
	else {
		Entity* ent = getObjectAtPoint(mousePos);
		if (ent) {
			if (ent != hoverObject) {
				if (hoverObject)SendEvent(new GUIInputEvent(GUIInputEvent::EventType::MOUSELEAVE, mousePos, mouseRel, leftButton, rightButton), hoverObject);
				hoverObject = ent;
				if (hoverObject)SendEvent(new GUIInputEvent(GUIInputEvent::EventType::MOUSEENTER, mousePos, mouseRel, leftButton, rightButton), ent);
			}
		}
		else {
			if (hoverObject)SendEvent(new GUIInputEvent(GUIInputEvent::EventType::MOUSELEAVE, mousePos, mouseRel, leftButton, rightButton), hoverObject);
			hoverObject = 0;
		}
	}
}
void GUIViewport::mouseButtonDownEvent(Vector2 mousePos, bool leftButton, bool rightButton) {
	Entity* ent = getObjectAtPoint(mousePos);
	if (ent) {
		hoverObject = ent;
		SendEvent(new GUIInputEvent(GUIInputEvent::EventType::MOUSEDOWN, mousePos, leftButton, rightButton), ent);
	}
}
void GUIViewport::mouseButtonUpEvent(Vector2 mousePos, bool leftButton, bool rightButton) {
	Entity* ent = getObjectAtPoint(mousePos);
	if (ent) {
		//Dragging has finished
		if (ent != hoverObject && leftButton) {
			if (hoverObject)SendEvent(new GUIInputEvent(GUIInputEvent::EventType::MOUSELEAVE, mousePos, Vector2(0,0), leftButton, rightButton), hoverObject);
			hoverObject = ent;
			if (hoverObject)SendEvent(new GUIInputEvent(GUIInputEvent::EventType::MOUSEENTER, mousePos, Vector2(0, 0), leftButton, rightButton), ent);
		}
		hoverObject = ent;
		SendEvent(new GUIInputEvent(GUIInputEvent::EventType::MOUSEUP, mousePos, leftButton, rightButton), ent);
	}
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
	/*
	glPushMatrix();
	glTranslatef(0, 300, 0);
	glEnable(GL_TEXTURE_2D);
	glColor4f(0, 1, 0, 1);
	ttfont.Draw("Parish so enable innate in formed missed. Hand two was eat busy fail.", 10);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	*/

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
