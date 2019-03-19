#include "Geo4.h"

GUIStyle::GUIStyle() : radius(0),
	radiusTopLeft(0),
	radiusTopRight(0),
	radiusBottomLeft(0),
	radiusBottomRight(0),
	borderSize(0),
	borderSizeTop(0),
	borderSizeBottom(0),
	borderSizeLeft(0),
	borderSizeRight(0),
	borderColor(0, 0, 0, 1),
	borderColorLeft(0, 0, 0, 1),
	borderColorRight(0, 0, 0, 1),
	borderColorTop(0, 0, 0, 1),
	borderColorBottom(0, 0, 0, 1),
	shadowSize(0),	
	shadowHardness(0),
	shadowX(0),
	shadowY(0),
	shadowColor(0,0,0,1),

	backgroundFill(FillType::NONE),
	backgroundColor(0,0,0,1)
{	}

GUIStyle::~GUIStyle()
{
}

GUIRenderable::GUIRenderable() : style(0),
	size(),
	vertexArrayId(0),
	bodyIndexArrayId(0),
	bodyNumIndices(0),
	shadowIndexArrayId(0),
	shadowNumIndices(0),
	borderIndexArrayId(0),
	borderNumIndices(0)
{


}


GUIRenderable::~GUIRenderable()
{
}

void GUIRenderable::Draw() {

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, (void*)0);
	glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, (float*)(sizeof(GLfloat) * 2));

	if (shadowIndexArrayId > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shadowIndexArrayId);
		glDrawElements(GL_TRIANGLES, shadowNumIndices, GL_UNSIGNED_INT, (void*)0);
	}
	if (bodyIndexArrayId > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bodyIndexArrayId);
		glDrawElements(GL_TRIANGLES, bodyNumIndices, GL_UNSIGNED_INT, (void*)0);
	}
	if (borderIndexArrayId > 0) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, borderIndexArrayId);
		glDrawElements(GL_TRIANGLES, borderNumIndices, GL_UNSIGNED_INT, (void*)0);
	}

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void GUIRenderable::_generateGeometryData() {

	Vector2 halfSize = size * 0.5;
	Vector2 radiusCenterPoint;
	float radiusShadowOuterEdge = 0;
	float radiusBorderInnerEdge = 0;
	Vector2 cornerBorderInnerEdge;

	Vector2 shadowPos(style->shadowX, style->shadowY);

	vector<Vector2>			bodyOutline;
	vector<Vector2>			shadowInline;
	vector<Vector2>			shadowOutline;
	vector<Vector2>			borderInline;
	vector<Vector4>			borderInlineColor;

	vector<Vector2>			vertexArray;
	vector<Vector4>			colorArray;
	vector<unsigned int>	indexArrayBody;
	vector<unsigned int>	indexArrayBorder;
	vector<unsigned int>	indexArrayShadow;
	
	float radiusBody[] = {
		style->radiusTopLeft,
		style->radiusBottomLeft,
		style->radiusBottomRight,
		style->radiusTopRight
	};
	Vector2 corners[] = {
		Vector2(-halfSize.x, halfSize.y),	//top left
		Vector2(-halfSize.x, -halfSize.y),	//bottom left
		Vector2(halfSize.x, -halfSize.y),	//bottom right
		Vector2(halfSize.x, halfSize.y)		//top right
	};
	Vector2 quadrants[] = {
		Vector2(-1, 1),
		Vector2(-1, -1),
		Vector2(1, -1),
		Vector2(1, 1)
	};
	/*
	float borderSizes[] = {
		style->borderSizeLeft,
		style->borderSizeBottom,
		style->borderSizeRight,
		style->borderSizeTop
	};*/

	Vector4 borderColors[] = {
		style->borderColorLeft,
		style->borderColorBottom,
		style->borderColorRight,
		style->borderColorTop
	};

	Vector2 borderInnerScale = Vector2(0.8, 0.8);
	Vector2 borderInnerTranslate = Vector2(10,10);

	unsigned int borderColorIndex = 0;

	for (int i = 0; i < 4; i++) {

		double step = Math::HALF_PI / max(4.0f, Math::Ceil(Math::HALF_PI * radiusBody[i] / 5));
		
		float shadowOuterSize = style->shadowSize;
		float shadowInnerSize = (style->shadowSize * style->shadowHardness) - 1.0f;

		//1. Generate body siluette conventional way
		if (radiusBody[i] > 0) {
			radiusCenterPoint = corners[i] - Vector2(radiusBody[i] * quadrants[i].x, radiusBody[i] * quadrants[i].y);
			for (double u = 0; u <= Math::HALF_PI; u += step) {

				if (u > (Math::HALF_PI * 0.5f)) {
					borderColorIndex = (i + 1) % 4;
				}

				float ux = (float)cos(Math::HALF_PI + u + (Math::HALF_PI * i));
				float uy = (float)sin(Math::HALF_PI + u + (Math::HALF_PI * i));
				float x = radiusCenterPoint.x + (ux * radiusBody[i]);
				float y = radiusCenterPoint.y + (uy * radiusBody[i]);

				//Body outer edge
				bodyOutline.push_back(Vector2(x, y));

				//Border outer edge
				borderInline.push_back(Vector2(borderInnerTranslate.x + (x * borderInnerScale.x), borderInnerTranslate.y + (y * borderInnerScale.y)));
				borderInlineColor.push_back(borderColors[i]);

				//Shadow inner edge
				x = radiusCenterPoint.x + (ux * (radiusBody[i] + shadowInnerSize));
				y = radiusCenterPoint.y + (uy * (radiusBody[i] + shadowInnerSize));
				shadowInline.push_back(Vector2(x, y) + shadowPos);

				//Shadow outer edge
				x = radiusCenterPoint.x + (ux * (radiusBody[i] + shadowOuterSize));
				y = radiusCenterPoint.y + (uy * (radiusBody[i] + shadowOuterSize));
				shadowOutline.push_back(Vector2(x, y) + shadowPos);
			}
		}
		else {
			bodyOutline.push_back(corners[i]);
			borderInline.push_back(Vector2(borderInnerTranslate.x + (corners[i].x * borderInnerScale.x),
				borderInnerTranslate.y + (corners[i].y * borderInnerScale.y)));
			borderInlineColor.push_back(borderColors[i]);
			shadowInline.push_back(Vector2((halfSize.x + shadowInnerSize) * quadrants[i].x, (halfSize.y + shadowInnerSize) * quadrants[i].y) + shadowPos);
			shadowOutline.push_back(Vector2((halfSize.x + shadowOuterSize) * quadrants[i].x, (halfSize.y + shadowOuterSize) * quadrants[i].y) + shadowPos);
		}
	}

	//4. Triangulate body
	if (style->backgroundFill != GUIStyle::FillType::NONE) {
		unsigned int offset = vertexArray.size();
		unsigned int nexti = 0;
		unsigned int midpointIndex = offset;
		vertexArray.push_back(Vector2(0, 0));
		colorArray.push_back(Vector4(1, 1, 1, 1));
		for (unsigned int i = 0; i < bodyOutline.size(); i++) {
			nexti = (i + 1) % bodyOutline.size();
			indexArrayBody.push_back(midpointIndex);
			indexArrayBody.push_back(midpointIndex + i + 1);
			indexArrayBody.push_back(midpointIndex + nexti + 1);
			vertexArray.push_back(bodyOutline[i]);
			colorArray.push_back(Vector4(1, 1, Math::RangeRandom(0, 1), 1));
		}
		_generateIndexBuffer(indexArrayBody, bodyIndexArrayId, bodyNumIndices);
	}

	//5. Generate shadow geometry
	if (style->shadowSize > 0) {
		unsigned int nexti = 0;
		unsigned int midpointIndex = vertexArray.size();
		vertexArray.push_back(Vector2(0, 0));
		colorArray.push_back(Vector4(1, 1, 1, 1));
		unsigned int offset = vertexArray.size();
		for (unsigned int i = 0; i < shadowInline.size(); i++) {
			vertexArray.push_back(shadowInline[i]);
			vertexArray.push_back(shadowOutline[i]);
			colorArray.push_back(style->shadowColor);
			colorArray.push_back(Vector4(style->shadowColor.xyz(), 0));
			nexti = (i + 1) % bodyOutline.size();
			indexArrayShadow.push_back(midpointIndex);
			indexArrayShadow.push_back(offset + (i * 2));
			indexArrayShadow.push_back(offset + (nexti * 2));
			indexArrayShadow.push_back(offset + (i * 2));
			indexArrayShadow.push_back(offset + (nexti * 2));
			indexArrayShadow.push_back(offset + (i * 2) + 1);
			indexArrayShadow.push_back(offset + (nexti * 2));
			indexArrayShadow.push_back(offset + (i * 2) + 1);
			indexArrayShadow.push_back(offset + (nexti * 2) + 1);
		}
		_generateIndexBuffer(indexArrayShadow, shadowIndexArrayId, shadowNumIndices);
	}


	//6. Triangulate border geometry
	unsigned int offset = vertexArray.size();
	unsigned int nexti = 0;
	for (unsigned int i = 0; i < bodyOutline.size(); i++) {
		nexti = (i + 1) % bodyOutline.size();
		vertexArray.push_back(bodyOutline[i]);
		vertexArray.push_back(borderInline[i]);
		vertexArray.push_back(bodyOutline[nexti]);
		vertexArray.push_back(borderInline[nexti]);

		colorArray.push_back(borderInlineColor[i]);
		colorArray.push_back(borderInlineColor[i]);
		colorArray.push_back(borderInlineColor[nexti]);
		colorArray.push_back(borderInlineColor[nexti]);
		
		indexArrayBorder.push_back(offset);
		indexArrayBorder.push_back(offset + 1);
		indexArrayBorder.push_back(offset + 2);
		indexArrayBorder.push_back(offset + 2);
		indexArrayBorder.push_back(offset + 1);
		indexArrayBorder.push_back(offset + 3);
		offset = vertexArray.size();
	}
	_generateIndexBuffer(indexArrayBorder, borderIndexArrayId, borderNumIndices);

	//7. Generate VBO data
	_generateVertexBuffer(vertexArray, colorArray);
}

void GUIRenderable::generateGeometry() {

	if (style == 0)return;

	_generateGeometryData();
}

void GUIRenderable::_generateVertexBuffer(vector<Vector2>& vertices, vector<Vector4>& colors) {

	if (vertexArrayId > 0)glDeleteBuffers(1, &vertexArrayId);

	unsigned int vertexCount = vertices.size();

	GLfloat* tmpVertexArray = new GLfloat[6 * vertexCount];

	unsigned int vi = 0;
	for (unsigned int i = 0; i < vertexCount; i++){
		tmpVertexArray[vi] = vertices[i].x;
		tmpVertexArray[vi + 1] = vertices[i].y;
		tmpVertexArray[vi + 2] = colors[i].x;
		tmpVertexArray[vi + 3] = colors[i].y;
		tmpVertexArray[vi + 4] = colors[i].z;
		tmpVertexArray[vi + 5] = colors[i].w;
		vi += 6;
	}

	glGenBuffers(1, &vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * vertexCount, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 6 * vertexCount, tmpVertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] tmpVertexArray;
}

void GUIRenderable::_generateIndexBuffer(vector<unsigned int>& indices, GLuint& indexArrayId, unsigned int& indexCount) {

	if (indexArrayId > 0)glDeleteBuffers(1, &indexArrayId);

	indexCount = indices.size();

	GLuint* tmpIndicesArray = new GLuint[indexCount];
	
	for (unsigned int i = 0; i < indexCount; i++) {
		tmpIndicesArray[i] = indices[i];
	}

	glGenBuffers(1, &indexArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indexCount, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*indexCount, tmpIndicesArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] tmpIndicesArray;
}