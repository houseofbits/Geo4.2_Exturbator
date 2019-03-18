#pragma once

#include <vector>
#include "Triangulation.h"

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
	float shadowHardness;
	float shadowX;
	float shadowY;

	Vector4 shadowColor;

	//GUIGradientColor backgroundGradientColor;
	Vector4		backgroundColor;
	FillType	backgroundFill;

	//void Deserialize(CFONode*) {};
};

/*
TODO:
v generate shadow geometry
v generate sizeable border geometry
- 0 size border geometry generating (triangulation tolerances)
v 0 size shadow geometry generating (triangulation tolerances)
v border size parameters
- generate border colors
- background clipping mask
- gradient rendering
- background color parameters
v shadow size parameters
- shadow color
- style deserialization
- foolproof input corner radiuses
- style resource loader class


*/

class GUIRenderable
{
public:
	GUIRenderable();
	virtual ~GUIRenderable();

	void Draw();

	void generateGeometry();

	void _generateGeometryData();
	void _generateVertexBuffer(vector<Vector2>&, vector<Vector4>&);
	void _generateIndexBuffer(vector<unsigned int>&, GLuint&, unsigned int&);

	GUIStyle*	style;
	Vector2		size;

	GLuint		vertexArrayId;

	GLuint			bodyIndexArrayId;
	unsigned int	bodyNumIndices;

	GLuint			shadowIndexArrayId;
	unsigned int	shadowNumIndices;

	GLuint			borderIndexArrayId;
	unsigned int	borderNumIndices;
};

