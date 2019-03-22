#include "Geo4.h"
#include "StringUtils.h"
#include <algorithm>

using namespace std;

CLASS_DECLARATION(GUISlider);

GUISlider::GUISlider(void) : GUIEntity(), EventHandler(), 
is_selected(0),
//font(), 
text(""), 
font_size(0.4f), 
type(0), 
ticks(0),
value(0),
pot_pos(60),
value_minmax(0,256),
is_over(0),
inputJoystickPresent(false),
inputJoystickAxis(0)
{ }

GUISlider::~GUISlider(void)
{ }

void GUISlider::Initialise(EventManager* const eventManager, ResourceManager*const resourceManagerr)
{	
	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUISlider::OnInputEvent);
	eventManager->RegisterEventReceiver(this, &GUISlider::OnWindowEvent);
}

void GUISlider::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);

	node->getValueString("caption",text);
	node->getValueInt("ticks", ticks);
	node->getValueVector2("minmax", value_minmax);
	node->getValueFloat("value", value);	
	node->getValueInt("joyAxisID", inputJoystickAxis);

	if(value > value_minmax.y)value = value_minmax.y;
	if(value < value_minmax.x)value = value_minmax.x;
}

bool GUISlider::OnWindowEvent(WindowEvent*const event)
{
	//Set default values on window creation
	if (event->event_type == WindowEvent::WINDOW_CREATED) {
		Entity* e = getObjectByName("window");
		if (e && e->getTypename() == "SDLWindow") {
			SDLWindow* window = (SDLWindow*)e;			
			if (window->joystick) {								
				Sint16 val = SDL_JoystickGetAxis(window->joystick, inputJoystickAxis);
				if (val != 0) {
					float fval = ((float)val + 32768) / 65535.0f;
					value = fval * 255;
					inputJoystickPresent = true;
				}
			}
		}

		SendEvent(new GUIEvent(getName(), GUIEvent::POT_CHANGED, (float)value));
	}
	return 1;
}

bool GUISlider::OnInputEvent(InputEvent*const event)
{	
	if(isVisible()==0)return 1;

	Vector2 mouse_pos;
	switch(event->sdl->type)
	{	
		case SDL_JOYAXISMOTION:
			if(event->sdl->jaxis.axis == inputJoystickAxis){
				float val = ((float)event->sdl->jaxis.value + 32768) / 65535.0f;
				//cout << "jaxis " << (unsigned int)event->sdl->jaxis.axis << ", " << val << endl;
				Math::clamptozero(val);
				val = min(1.0f,val);
				value = val * 255;
				SendEvent(new GUIEvent(getName(), GUIEvent::POT_CHANGED, (float)value));
			}
		break;
		case SDL_MOUSEMOTION:
			mouse_pos = Vector2((float)event->sdl->motion.x, (float)event->sdl->motion.y);
			//cout << mouse_pos.x << "," << mouse_pos.y << endl;
			if(Clip(mouse_pos)){
				is_over = 1;
			}else is_over = 0;

			if(event->sdl->button.button==SDL_BUTTON_LEFT && is_selected){				
				if(OnChangeValue(mouse_pos))SendEvent(new GUIEvent(getName(), GUIEvent::POT_CHANGED, (float)value));
			}
		break;
		case SDL_MOUSEBUTTONDOWN:
			if(event->sdl->button.button==SDL_BUTTON_LEFT){
				mouse_pos = Vector2((float)event->sdl->motion.x, (float)event->sdl->motion.y);
				if(IsOver(mouse_pos)){
					OnChangeValue(mouse_pos);
					is_selected = 1;
				}
			}
		break;
		case SDL_MOUSEBUTTONUP:			
			if(event->sdl->button.button==SDL_BUTTON_LEFT && is_selected){
				mouse_pos = Vector2((float)event->sdl->motion.x, (float)event->sdl->motion.y);
				if(OnChangeValue(mouse_pos)){
					SendEvent(new GUIEvent(getName(), GUIEvent::POT_CHANGED, (float)value));
				}
			}
			is_selected = 0;
		break;
	};
	return 1;
}

bool GUISlider::IsOver(Vector2 p)
{
	return Clip(Vector2(p));
}

bool GUISlider::OnChangeValue(Vector2 mouse_pos)
{	
	float rel_mx = m_WorldPos.x - mouse_pos.x;
	if(rel_mx<(-m_Size.x/2) || rel_mx>(m_Size.x/2))return 0;
	value = (((-rel_mx + m_Size.x/2) * (value_minmax.y - value_minmax.x)) / m_Size.x) + value_minmax.x;
	return 1;
}

void GUISlider::PreRender(Renderer*)
{ }

void GUISlider::Render(Renderer*)
{

//glDisable(GL_FOG);
//glDisable(GL_LIGHTING);
//glDisable(GL_CULL_FACE);



glPushMatrix();
	glEnable(GL_BLEND);
	glTranslatef(m_LocalPos.x, m_LocalPos.y, 0);
	
	if(is_over){
		glColor4f(1,1,1,0.1f);
		Renderer::DrawRect(-m_Size.x/2, m_Size.y/2, m_Size.x/2,-m_Size.y/2);
	}

	glColor3f(0.5,0.5,1);
	glLineWidth(3);
	glBegin(GL_LINES);	
		glVertex2f(-m_Size.x/2, 0);
		glVertex2f(m_Size.x/2, 0);
	glEnd();
	glBegin(GL_LINES);	
		glVertex2f(-m_Size.x/2, 0);
		glVertex2f(-m_Size.x/2, -m_Size.y/2);
	glEnd();
	glBegin(GL_LINES);	
		glVertex2f(m_Size.x/2, 0);
		glVertex2f(m_Size.x/2, -m_Size.y/2);
	glEnd();

	glPushMatrix();
		glTranslatef(0,-(m_Size.y/2),0);
		glColor4f(1,1,1, 0.5);
		glEnable(GL_TEXTURE_2D);
		if (!text.empty()) {
			string title = text;
			if (inputJoystickPresent) {
				title = text + "(input:" + Utils::IntToString(inputJoystickAxis) + ")";
			}
//			Font::m_DefaultInstance.DrawCenter(title, font_size);
		}
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	if(ticks>0){
		glColor3f(0.5,0.5,1);
		glLineWidth(1);
		glBegin(GL_LINES);			
			float step = m_Size.x/ticks;
			for (float n=-m_Size.x/2; n<= m_Size.x/2; n+=step){
				glVertex2f(n, 0);		
				glVertex2f(n, -m_Size.y/4);
			}
		glEnd();
	}
	
	//pot
	glPushMatrix();
		float v = (-m_Size.x/2) + ((value - value_minmax.x) * m_Size.x) / (value_minmax.y - value_minmax.x);
		glTranslatef(v,0,0);
		glColor3f(1,1,0);
		glBegin(GL_POLYGON);
			glVertex2f(-7,-m_Size.y/2);
			glVertex2f(7,-m_Size.y/2);
			glVertex2f(7,-m_Size.y/4);
			glVertex2f(0, 0);
			glVertex2f(-7,-m_Size.y/4);
		glEnd();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0, m_Size.y/2, 0);
		glColor3f(1,1,0);
		glEnable(GL_TEXTURE_2D);
		//if(!text.empty() && font.isValid())
//		Font::m_DefaultInstance.DrawCenter(Utils::IntToString((int)value),font_size);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//marks
	glPushMatrix();
		glTranslatef(-(m_Size.x/2), m_Size.y/2,0);
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		//if(!text.empty() && font.isValid())
//		Font::m_DefaultInstance.DrawCenter(Utils::IntToString((int)value_minmax.x),font_size);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPushMatrix();
		glTranslatef((m_Size.x/2), m_Size.y/2,0);
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		//if(!text.empty() && font.isValid())
//		Font::m_DefaultInstance.DrawCenter(Utils::IntToString((int)value_minmax.y),font_size);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glLineWidth(1);

	glDisable(GL_BLEND);

glPopMatrix();
}

void GUISlider::PostRender()
{ }
