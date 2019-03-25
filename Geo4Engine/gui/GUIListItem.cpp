#include "../Geo4.h"

CLASS_DECLARATION(GUIListItem);

GUIListItem::GUIListItem() : 
	renderableActive(),
	renderableHover(),
	renderablePressed(),
	styleSheet(),
	styleActiveName(),
	styleHoverName(),
	stylePressedName(),
	selected(false),
	parentList(0),
	hoverState(false)
{	}

GUIListItem::~GUIListItem()
{	}

void GUIListItem::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	resourceManager->Get(styleSheet, "style/styles.cfo");

	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUIListItem::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUIListItem::OnWindowEvent);

	renderableActive.style = &styleSheet->get(styleActiveName);
	renderableHover.style = &styleSheet->get(styleHoverName);
	renderablePressed.style = &styleSheet->get(stylePressedName);

	renderableActive.update();
	renderableHover.update();
	renderablePressed.update();

	renderableActive.setText(m_Title);
	renderableHover.setText(m_Title);
	renderablePressed.setText(m_Title);

	if (parentList && !parentList->multiSelect)parentList->reorderItems();
}

void GUIListItem::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);

	styleActiveName = "listItemActive";
	styleHoverName = "listItemActive";
	stylePressedName = "listItemPressed";

	node->getValueString("styleActive", styleActiveName);
	node->getValueString("styleHover", styleHoverName);
	node->getValueString("stylePressed", stylePressedName);

	if (m_Parent && m_Parent->getTypename() == "GUIList"){
		parentList = (GUIList*)m_Parent;
		m_Size = Vector2(parentList->m_Size.x, parentList->itemHeight);
		renderableActive.size = m_Size;
		renderableHover.size = m_Size;
		renderablePressed.size = m_Size;
	}
}

bool GUIListItem::OnWindowEvent(WindowEvent*const event)
{
	if (event->event_type == WindowEvent::WINDOW_CREATED) {


	}
	return 1;
}

bool GUIListItem::OnGUIInputEvent(GUIInputEvent*const event)
{	
	if (isVisible() == 0)return 1;

	switch (event->type) {
	case GUIInputEvent::EventType::MOUSEUP:
		if (parentList && !parentList->multiSelect)parentList->DeselectAll();
		selected = !selected;
		break;
	case GUIInputEvent::EventType::MOUSEDOWN:
		break;
	case GUIInputEvent::EventType::MOUSEENTER:
		if(!selected)hoverState = 1;
		break;
	case GUIInputEvent::EventType::MOUSELEAVE:
		hoverState = 0;
		break;
	};
	return 1;
}

void GUIListItem::PreRender(Renderer*)
{
	glPushMatrix();
	glTranslatef(m_LocalPos.x, m_LocalPos.y, 0);
}

void GUIListItem::Render(Renderer* rnd)
{
	if (isVisible() == 0)return;

	if (!hoverState && !selected)renderableActive.Draw();
	else if (hoverState && !selected)renderableHover.Draw();
	else if (selected)renderablePressed.Draw();

}

void GUIListItem::PostRender()
{
	glPopMatrix();
}