#pragma once

#include <vector>
#include "Triangulation.h"

class GUIGradientColor {
public:
	GUIGradientColor();
	class GradientPoint {
	public:
		GradientPoint() {}
		GradientPoint(Vector4 c, float p):color(c),position(p) {}
		Vector4 color;
		float	position;
	};
	vector<GradientPoint> points;

	void add(Vector4 c, float p) { points.push_back(GradientPoint(c,p)); }

	void draw();

	void generate(Vector2&);

	GLuint			vertexArrayId;
	GLuint			indexArrayId;
	unsigned int	numIndices;

	void Deserialize(CFONode*);
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

	float radius;				//ONLY FOR SERIALIZER. MIGHT BE REMOVED
	float radiusTopLeft;
	float radiusTopRight;
	float radiusBottomLeft;
	float radiusBottomRight;
	
	float borderSize;			//ONLY FOR SERIALIZER. MIGHT BE REMOVED
	float borderSizeTop;
	float borderSizeBottom;
	float borderSizeLeft;
	float borderSizeRight;

	Vector4 borderColor;		//ONLY FOR SERIALIZER. MIGHT BE REMOVED
	Vector4 borderColorLeft;
	Vector4 borderColorRight;
	Vector4 borderColorTop;
	Vector4 borderColorBottom;

	float shadowSize;		
	float shadowHardness;
	float shadowX;
	float shadowY;
	Vector4 shadowColor;

	GUIGradientColor backgroundGradientColor;
	Vector4		backgroundColor;
	FillType	backgroundFill;

	void Deserialize(CFONode*);
};

/*
TODO:
v generate shadow geometry
v generate sizeable border geometry
- 0 size border geometry generating (triangulation tolerances)
v 0 size shadow geometry generating (triangulation tolerances)
v border size parameters
v generate border colors
- background clipping mask
- gradient rendering
- background color parameters
v shadow size parameters
v shadow color
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


	static GLuint stencilIndexCounter;
	static GLuint createStencilIndex() {
		stencilIndexCounter = (stencilIndexCounter + 1) % 0xFF;
		if (stencilIndexCounter == 0)stencilIndexCounter++;
		return stencilIndexCounter;
	}

	GLuint		stencilIndex;

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

