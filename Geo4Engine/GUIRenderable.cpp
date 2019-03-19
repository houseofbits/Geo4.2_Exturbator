#include "Geo4.h"

GUIGradientColor::GUIGradientColor() : vertexArrayId(0),
	indexArrayId(0),
	numIndices(0)
{	}

void GUIGradientColor::draw() {
	if (indexArrayId > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, (void*)0);
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, (float*)(sizeof(GLfloat) * 2));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayId);
		glDrawElements(GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, (void*)0);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

void GUIGradientColor::generate(Vector2& size) {

	//0.Sort
	std::sort(points.begin(), points.end(),
		[](GradientPoint const& a, GradientPoint const& b) { return a.position < b.position; });

	vector<Vector2> verts;
	vector<Vector4> colors;
	vector<unsigned int> indices;

	//1.generate
	unsigned int offset = 0;
	for (unsigned int i = 0; i < points.size(); i++){		
		verts.push_back(Vector2(0, points[i].position));
		verts.push_back(Vector2(1, points[i].position));
		colors.push_back(points[i].color);
		colors.push_back(points[i].color);
		indices.push_back(offset);
		indices.push_back(offset+1);
		offset = verts.size();
	}
	//2.scale/translate/rotate
	for (unsigned int i = 0; i < verts.size(); i++) {
		verts[i] = Vector2(
			(0.5f - verts[i].x),// * size.x,
			(0.5f - verts[i].y)// * size.y
		);
	}

	//3.generate vbo
	if (vertexArrayId > 0)glDeleteBuffers(1, &vertexArrayId);
	if (indexArrayId > 0)glDeleteBuffers(1, &indexArrayId);

	unsigned int vertexCount = verts.size();

	GLfloat* tmpVertexArray = new GLfloat[6 * vertexCount];

	unsigned int vi = 0;
	for (unsigned int i = 0; i < vertexCount; i++) {
		tmpVertexArray[vi] = verts[i].x;
		tmpVertexArray[vi + 1] = verts[i].y;
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

	numIndices = indices.size();

	GLuint* tmpIndicesArray = new GLuint[numIndices];

	for (unsigned int i = 0; i < numIndices; i++) {
		tmpIndicesArray[i] = indices[i];
	}

	glGenBuffers(1, &indexArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numIndices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*numIndices, tmpIndicesArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] tmpIndicesArray;
	delete[] tmpVertexArray;
}

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

void GUIGradientColor::Deserialize(CFONode* node)
{
	CFONode* object = node->GetFirstChild();
	while (object) {
		string classname = object->GetName();
		string value = object->GetValue();
		Vector4 color = Utils::StringToVector4(value);
		float pos = (float)atof(classname.c_str());
		add(color, pos);
		object = object->GetNextNode();
	}

}

void GUIStyle::Deserialize(CFONode* node)
{
	if (node->getValueFloat("radius", radius)) {
		radiusTopLeft = radiusTopRight = radiusBottomLeft = radiusBottomRight = radius;
	}
	else {
		node->getValueFloat("radiusTopLeft", radiusTopLeft);
		node->getValueFloat("radiusTopRight", radiusTopRight);
		node->getValueFloat("radiusBottomLeft", radiusBottomLeft);
		node->getValueFloat("radiusBottomRight", radiusBottomRight);
	}

	if (node->getValueFloat("borderSize", borderSize)) {
		borderSizeTop = borderSizeBottom = borderSizeLeft = borderSizeRight = borderSize;
	}
	else {
		node->getValueFloat("borderSizeTop", borderSizeTop);
		node->getValueFloat("borderSizeBottom", borderSizeBottom);
		node->getValueFloat("borderSizeLeft", borderSizeLeft);
		node->getValueFloat("borderSizeRight", borderSizeRight);
	}

	if (node->getValueVector4("borderColor", borderColor)) {
		borderColorLeft = borderColorRight = borderColorTop = borderColorBottom = borderColor;
	}
	else {
		node->getValueVector4("borderColorLeft", borderColorLeft);
		node->getValueVector4("borderColorRight", borderColorRight);
		node->getValueVector4("borderColorTop", borderColorTop);
		node->getValueVector4("borderColorBottom", borderColorBottom);
	}

	node->getValueFloat("shadowSize", shadowSize);
	node->getValueFloat("shadowHardness", shadowHardness);
	node->getValueFloat("shadowX", shadowX);
	node->getValueFloat("shadowY", shadowY);
	node->getValueVector4("shadowColor", shadowColor);

	if (node->getValueVector4("backgroundColor", backgroundColor)) {
		backgroundFill = FillType::SOLID;
	}
	else if (node->GetFirstChild("backgroundGradientColor")) {
		backgroundFill = FillType::GRADIENT;
		backgroundGradientColor.Deserialize(node->GetFirstChild("backgroundGradientColor"));
	}
	else {
		backgroundFill = FillType::NONE;
	}
}

GLuint GUIRenderable::stencilIndexCounter = 0;

GUIRenderable::GUIRenderable() : style(0),
	size(),
	vertexArrayId(0),
	bodyIndexArrayId(0),
	bodyNumIndices(0),
	shadowIndexArrayId(0),
	shadowNumIndices(0),
	borderIndexArrayId(0),
	borderNumIndices(0),
	stencilIndex(0)
{


}


GUIRenderable::~GUIRenderable()
{
}

void GUIRenderable::Draw() {

	if (bodyIndexArrayId > 0 && style->backgroundFill == GUIStyle::FillType::GRADIENT) {

		if (stencilIndex == 0) {
			stencilIndex = createStencilIndex();
		}
		//GLuint stencilIndex = createStencilIndex();

		//Disable rendering to the color buffer
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, stencilIndex, stencilIndex);
		glStencilMask(stencilIndex);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, (void*)0);
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, (float*)(sizeof(GLfloat) * 2));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bodyIndexArrayId);
		glDrawElements(GL_TRIANGLES, bodyNumIndices, GL_UNSIGNED_INT, (void*)0);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);


		if (shadowIndexArrayId > 0) {
			glStencilFunc(GL_NOTEQUAL, stencilIndex, stencilIndex);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			//Keep the pixel
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, (void*)0);
			glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, (float*)(sizeof(GLfloat) * 2));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shadowIndexArrayId);
			glDrawElements(GL_TRIANGLES, shadowNumIndices, GL_UNSIGNED_INT, (void*)0);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		glStencilFunc(GL_EQUAL, stencilIndex, stencilIndex);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		//Keep the pixel
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		glPushMatrix();
		glScalef(size.x, size.y, 1.0f);
		style->backgroundGradientColor.draw();
		glPopMatrix();

		glDisable(GL_STENCIL_TEST);

		if (borderIndexArrayId > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, (void*)0);
			glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, (float*)(sizeof(GLfloat) * 2));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, borderIndexArrayId);
			glDrawElements(GL_TRIANGLES, borderNumIndices, GL_UNSIGNED_INT, (void*)0);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	else {

		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 6, (void*)0);
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 6, (float*)(sizeof(GLfloat) * 2));

		if (shadowIndexArrayId > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shadowIndexArrayId);
			glDrawElements(GL_TRIANGLES, shadowNumIndices, GL_UNSIGNED_INT, (void*)0);
		}
		if (bodyIndexArrayId > 0 && style->backgroundFill == GUIStyle::FillType::SOLID) {
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
	vector<bool>			borderChangeColor;

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
	Vector4 borderColors[] = {
		style->borderColorLeft,
		style->borderColorBottom,
		style->borderColorRight,
		style->borderColorTop
	};

	float offx = style->borderSizeLeft - style->borderSizeRight;
	float offy = style->borderSizeBottom - style->borderSizeTop;
	float w = size.x - style->borderSizeLeft - style->borderSizeRight;
	float h = size.y - style->borderSizeBottom - style->borderSizeTop;

	Vector2 borderInnerScale = Vector2(w/size.x, h/size.y);
	Vector2 borderInnerTranslate = Vector2(offx / 2, offy / 2);

	for (int i = 0; i < 4; i++) {

		double step = Math::HALF_PI / max(4.0f, Math::Ceil(Math::HALF_PI * radiusBody[i] / 5));
		
		float shadowOuterSize = style->shadowSize;
		float shadowInnerSize = (style->shadowSize * style->shadowHardness) - 1.0f;

		//1. Generate body siluette conventional way
		if (radiusBody[i] > 0) {
			radiusCenterPoint = corners[i] - Vector2(radiusBody[i] * quadrants[i].x, radiusBody[i] * quadrants[i].y);
			bool changeColor = false;
			for (double u = 0; u <= Math::HALF_PI; u += step) {

				if (u > (Math::HALF_PI * 0.5f) && !changeColor) {
					changeColor = true;
					borderChangeColor.push_back(true);
				}
				else {
					borderChangeColor.push_back(false);
				}

				float ux = (float)cos(Math::HALF_PI + u + (Math::HALF_PI * i));
				float uy = (float)sin(Math::HALF_PI + u + (Math::HALF_PI * i));
				float x = radiusCenterPoint.x + (ux * radiusBody[i]);
				float y = radiusCenterPoint.y + (uy * radiusBody[i]);

				//Body outer edge
				bodyOutline.push_back(Vector2(x, y));

				//Border outer edge
				borderInline.push_back(Vector2(borderInnerTranslate.x + (x * borderInnerScale.x), borderInnerTranslate.y + (y * borderInnerScale.y)));

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
			borderChangeColor.push_back(true);
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
			if(style->backgroundFill == GUIStyle::FillType::SOLID)
				colorArray.push_back(style->backgroundColor);
			else
				colorArray.push_back(Vector4(1,1,1,1));
		}
		_generateIndexBuffer(indexArrayBody, bodyIndexArrayId, bodyNumIndices);
	}

	//5. Generate shadow geometry
	if (style->shadowSize > 0) {
		unsigned int nexti = 0;
		unsigned int midpointIndex = vertexArray.size();
		vertexArray.push_back(Vector2(0, 0));
		colorArray.push_back(style->shadowColor);
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
	if (style->borderSizeBottom > 0 || 
		style->borderSizeTop > 0 || 
		style->borderSizeLeft > 0 || 
		style->borderSizeRight > 0) {
		unsigned int offset = vertexArray.size();
		unsigned int nexti = 0;
		unsigned int colorIndex = 0;
		for (unsigned int i = 0; i < bodyOutline.size(); i++) {
			if (borderChangeColor[i]) {
				colorIndex = (colorIndex + 1) % 4;
			}
			nexti = (i + 1) % bodyOutline.size();
			vertexArray.push_back(bodyOutline[i]);
			vertexArray.push_back(borderInline[i]);
			vertexArray.push_back(bodyOutline[nexti]);
			vertexArray.push_back(borderInline[nexti]);

			colorArray.push_back(borderColors[colorIndex]);
			colorArray.push_back(borderColors[colorIndex]);
			colorArray.push_back(borderColors[colorIndex]);
			colorArray.push_back(borderColors[colorIndex]);

			indexArrayBorder.push_back(offset);
			indexArrayBorder.push_back(offset + 1);
			indexArrayBorder.push_back(offset + 2);
			indexArrayBorder.push_back(offset + 2);
			indexArrayBorder.push_back(offset + 1);
			indexArrayBorder.push_back(offset + 3);
			offset = vertexArray.size();
		}
		_generateIndexBuffer(indexArrayBorder, borderIndexArrayId, borderNumIndices);
	}

	//7. Generate VBO data
	_generateVertexBuffer(vertexArray, colorArray);

	if (style->backgroundFill == GUIStyle::FillType::GRADIENT) {
		style->backgroundGradientColor.generate(size); //TODO other params
	}
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