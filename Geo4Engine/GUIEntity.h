#pragma once
#include "Entity.h"

class GUIEntity :
	public Entity
{
	BASE_CLASS_PROTOTYPE(GUIEntity)
public:
	GUIEntity();
	virtual ~GUIEntity();

	virtual void			Deserialize(CFONode*);

	Vector2					getWorldSpace(Vector2&);

	virtual bool			isVisible();

	virtual bool			Clip(Vector2);

	virtual void			setVisible(bool visible){	m_Visible = visible;	}
	virtual void			setDisabled(bool d){	m_Disabled=d;	}
	virtual bool			isDisabled();

	virtual float			getZIndex(){	return m_ZIndex;	}
	virtual void			setZIndex(float z){	m_ZIndex = z;	}	

	void					setLocalPosition(Vector2 p) { m_LocalPos = p; }
	Vector2					getLocalPosition() { return m_LocalPos; }

	Entity*					getObjectAtPoint(Vector2& p);

private:
	virtual bool			_isVisible(Entity*);
	Vector2					_getWorldSpace(Vector2&, Entity*);
	void					_SortByZIndex();
	static void				_RecursiveFindObjectByPosition(Entity* e, Entity* &obj, const Vector2& p);

public:
	bool					m_Visible;
	bool					m_Disabled;
	float					m_ZIndex;
	string					m_Title;
	Vector2					m_WorldPos;
	Vector2					m_LocalPos;
	Vector2					m_Size;
};

