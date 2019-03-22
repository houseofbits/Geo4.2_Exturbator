#include "Geo4.h"

CLASS_DECLARATION(GUIWindow);

GUIWindow::GUIWindow() : renderableTitle(),
	renderableBody(),
	renderableShadow(),
	styleSheet(),
	titleBarSize(30),
	styleTitleName(),
	styleBodyName(),
	styleShadowName()
{	}

GUIWindow::~GUIWindow()
{	}

void GUIWindow::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUIWindow::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUIWindow::OnWindowEvent);

	resourceManager->Get(styleSheet, "styles.cfo");

	renderableTitle.style = &styleSheet->get(styleTitleName);
	renderableBody.style = &styleSheet->get(styleBodyName);
	renderableShadow.style = &styleSheet->get(styleShadowName);
}

void GUIWindow::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);

	node->getValueBool("showOverlay", showOverlay);

	styleTitleName = "windowDefaultTitle";
	styleBodyName = "windowDefaultBody";
	styleShadowName = "windowDefaultShadow";

	node->getValueString("styleTitle", styleTitleName);
	node->getValueString("styleBody", styleBodyName);
	node->getValueString("styleShadow", styleShadowName);

	renderableTitle.size = m_Size;
	renderableTitle.size.y = titleBarSize;
	renderableBody.size = m_Size;
	renderableBody.size.y = m_Size.y - titleBarSize;
	renderableShadow.size = m_Size;
}

bool GUIWindow::OnWindowEvent(WindowEvent*const event)
{
	if (event->event_type == WindowEvent::WINDOW_CREATED) {

		renderableTitle.update();
		renderableBody.update();
		renderableShadow.update();

		renderableTitle.setText(m_Title);
	}
	return 1;
}

bool GUIWindow::OnGUIInputEvent(GUIInputEvent*const event)
{
	if (isVisible() == 0)return 1;

	switch (event->type) {
	case GUIInputEvent::EventType::MOUSEMOVE:

		if(event->mouseButtonLeft)m_LocalPos += event->mouseMotion;

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

void GUIWindow::PreRender(Renderer*)
{
	glPushMatrix();
	glTranslatef(m_LocalPos.x, m_LocalPos.y, 0);
}

void GUIWindow::Render(Renderer* rnd)
{
	if (isVisible() == 0)return

	glEnable(GL_BLEND);

	renderableShadow.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(m_LocalPos.x, m_LocalPos.y + (m_Size.y * 0.5f) - (titleBarSize * 0.5f), 0);
	renderableTitle.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(m_LocalPos.x, m_LocalPos.y - (titleBarSize * 0.5f), 0);
	renderableBody.Draw();
}

void GUIWindow::PostRender()
{
	glPopMatrix();
}
