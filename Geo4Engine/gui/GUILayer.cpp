#include "../Geo4.h"

CLASS_DECLARATION(GUILayer);

GUILayer::GUILayer() : renderable(),
	styleSheet(),
	styleName()
{	}

GUILayer::~GUILayer()
{	}

void GUILayer::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	eventManager->RegisterEventHandler(this);

	resourceManager->Get(styleSheet, "style/styles.cfo");

	renderable.style = &styleSheet->get(styleName);

	renderable.update();

	renderable.setTextWithWrapping(m_Title);
}

void GUILayer::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);	

	styleName = "layer";

	node->getValueString("style", styleName);

	renderable.size = m_Size;
}

void GUILayer::PreRender(Renderer*)
{
	glPushMatrix();
	glTranslatef(m_LocalPos.x, m_LocalPos.y, 0);
}

void GUILayer::Render(Renderer* rnd)
{
	if (isVisible() == 0)return;
	
	renderable.Draw();

}

void GUILayer::PostRender()
{
	glPopMatrix();
}
