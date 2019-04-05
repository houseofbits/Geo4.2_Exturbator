#include "../Geo4.h"

#ifndef NO_OPENGL

CLASS_DECLARATION(GUIGraph);

GUIGraph::GUIGraph() : styleSheet(),
	renderable()
{	}

GUIGraph::~GUIGraph()
{	}

void GUIGraph::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	resourceManager->Get(styleSheet, "style/styles.cfo");

	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUIGraph::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUIGraph::OnWindowEvent);

	resourceManager->Get(styleSheet, "style/styles.cfo");

	renderable.style = &styleSheet->get(styleName);

	renderable.update();

	renderable.setTextWithWrapping(m_Title);
}

void GUIGraph::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);

	styleName = "layer";

	node->getValueString("style", styleName);

	renderable.size = m_Size;
}

bool GUIGraph::OnWindowEvent(WindowEvent*const event)
{
	if (event->eventType == WindowEvent::WINDOW_CREATED) {
		renderable.update();
	}
	return 1;
}

bool GUIGraph::OnGUIInputEvent(GUIInputEvent*const event)
{
	if (isVisible() == 0)return 1;

	switch (event->type) {
	case GUIInputEvent::EventType::MOUSEUP:

		break;
	case GUIInputEvent::EventType::MOUSEDOWN:

		break;
	case GUIInputEvent::EventType::MOUSEENTER:

		break;
	case GUIInputEvent::EventType::MOUSELEAVE:

		break;
	};
	return 1;
}

void GUIGraph::PreRender(Renderer* r) {
	GUIEntity::PreRender(r);
}

void GUIGraph::Render(Renderer* rnd){

	renderable.Draw();

	glColor4f(0.5f, 0.5f, 0.5f, 1);
	
	Vector2 hs = m_Size * 0.5f;

	unsigned int xGrid = 20;
	unsigned int yGrid = 10;
	glBegin(GL_LINES);
	for (unsigned int i = 0; i < yGrid; i++) {
		float p = -hs.y + (((float)i/yGrid) * m_Size.y);
		glVertex2f(-hs.x, p);
		glVertex2f(hs.x, p);
	}
	glEnd();
	glBegin(GL_LINES);
	for (unsigned int i = 0; i < xGrid; i++) {
		float p = -hs.x + (((float)i / xGrid) * m_Size.x);
		glVertex2f(p, -hs.y);
		glVertex2f(p, hs.y);
	}
	glEnd();

}

void GUIGraph::PostRender() {
	GUIEntity::PostRender();
}

#endif