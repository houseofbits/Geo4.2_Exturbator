#pragma once

#include <vector>

class GUIGradientColor {
public:
	class GradientPoint {
	public:
		Vector4 color;
		float	position;
	};
	vector<GradientPoint> points;

	//void Deserialize(CFONode*) {};
};

class GUIStyle {
public:
	GUIStyle();

	enum FillType{
		NONE = 0,
		SOLID,
		GRADIENT
	};

	virtual ~GUIStyle();
	float radius;
	float radiusTopLeft;
	float radiusTopRight;
	float radiusBottomLeft;
	float radiusBottomRight;
	
	float borderSize;
	float borderSizeTop;
	float borderSizeBottom;
	float borderSizeLeft;
	float borderSizeRight;

	float shadowSize;
	float shadowSizeTop;
	float shadowSizeBottom;
	float shadowSizeLeft;
	float shadowSizeRight;

//	Vector4 shadowColor;

	//GUIGradientColor backgroundGradientColor;
	Vector4		backgroundColor;
	FillType	backgroundFill;

	//void Deserialize(CFONode*) {};
};

/*
TODO:
generate shadow geometry
generate sizeable border geometry
border size parameters
generate border colors
background clipping mask
gradient rendering
background color parameters
shadow parameters
style deserialization
foolproof input corner radiuses

QUESTIONS:
how styles are managed and deserialized

*/

class GUIRenderable
{
public:
	GUIRenderable();
	virtual ~GUIRenderable();

	void Draw();

	void generateGeometry();
	void _generateShell(vector<Vector2>& points,
		Vector2 size,
		float r1,
		float r2,
		float r3,
		float r4,
		float offsetL,
		float offsetR,
		float offsetT,
		float offsetB);
	void _generateSolidFromPointSet(vector<Vector2>*,
		GLuint& indexarray,
		GLuint& vertexarray,
		unsigned int& indexcount);
	void _generateStripFromPointSet(vector<Vector2>*,
		vector<Vector2>*,
		GLuint& indexarray,
		GLuint& vertexarray,
		unsigned int& indexcount);

	GUIStyle*	style;
	Vector2		size;

	GLuint			bodyGeometryArray;
	GLuint			bodyIndexArray;
	unsigned int	bodyNumIndices;

	GLuint			borderGeometryArray;
	GLuint			borderIndexArray;
	unsigned int	borderNumIndices;

	GLuint			shadowGeometryArray;
	GLuint			shadowIndexArray;
	unsigned int	shadowNumIndices;
};

