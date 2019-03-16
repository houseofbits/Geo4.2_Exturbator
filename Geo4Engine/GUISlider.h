#pragma once
#include "Geo4.h"

class GUISlider: public GUIEntity, public EventHandler
{
CLASS_PROTOTYPE(GUISlider);
public:	
	GUISlider(void);
	virtual ~GUISlider(void);

	void	Initialise(EventManager*const, SceneManager*);
	void	Deinitialise(EventManager*const,SceneManager*){};

	void	Serialize(CFONode*, ResourceManager*){}
	void	Deserialize(CFONode*, ResourceManager*);
	
	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();
	
	bool	OnWindowEvent(WindowEvent*const);
	bool	OnInputEvent(InputEvent*const);
		
	bool	OnChangeValue(Vector2);
	bool	IsOver(Vector2);

	bool			is_selected;
		
	unsigned int	type;
	unsigned int	ticks;
	
	Vector2			value_minmax;
	float			value;
	float			pot_pos;
	std::string		text;
	
//	FontHandle		font;
	float			font_size;

	bool			is_over;

	bool			inputJoystickPresent;
	unsigned int	inputJoystickAxis;
};
