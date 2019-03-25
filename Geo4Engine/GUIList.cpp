#include "Geo4.h"

CLASS_DECLARATION(GUIList);

GUIList::GUIList() : itemHeight(40),
	styleSheet(),
	renderableBody()
{	}

GUIList::~GUIList()
{	}

void GUIList::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	resourceManager->Get(styleSheet, "styles.cfo");

	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUIList::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUIList::OnWindowEvent);

	renderableBody.style = &styleSheet->get("listBody");
}

void GUIList::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);

	renderableBody.size = m_Size;
}

bool GUIList::OnWindowEvent(WindowEvent*const event)
{
	if (event->event_type == WindowEvent::WINDOW_CREATED) {
		renderableBody.update();
	}
	return 1;
}

bool GUIList::OnGUIInputEvent(GUIInputEvent*const event)
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

void GUIList::PreRender(Renderer*)
{
	glPushMatrix();
	glTranslatef(m_LocalPos.x, m_LocalPos.y, 0);
}

void GUIList::Render(Renderer* rnd)
{
	if (isVisible() == 0)return;

	renderableBody.Draw();

}

void GUIList::PostRender()
{
	glPopMatrix();
}
