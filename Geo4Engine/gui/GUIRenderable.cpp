#include "../Geo4.h"

GLuint GUIGradientColor::gradientTextureMap = 0;
unsigned int GUIGradientColor::gradientMapPositionIterator = 0;
const unsigned int GUIGradientColor::gradientMapWidth = 512;
const unsigned int GUIGradientColor::gradientMapHeight = 16;
const unsigned int GUIGradientColor::gradientEntryWidth = 4;

GUIGradientColor::GUIGradientColor() : points(), 
	vertexArrayId(0),
	indexArrayId(0),
	numIndices(0),
	angle(0),
	textureU(0),
	textureU1(1)
{	}

void GUIGradientColor::draw() {
	glPushMatrix();	
	glBindTexture(GL_TEXTURE_2D, gradientTextureMap);
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(textureU, 0);
	glVertex2f(-0.5f, -0.5f);
	glTexCoord2f(textureU, 1);
	glVertex2f(-0.5f, 0.5f);
	glTexCoord2f(textureU1, 1);
	glVertex2f(0.5f, 0.5f);
	glTexCoord2f(textureU1, 0);
	glVertex2f(0.5f, -0.5f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void GUIGradientColor::generate() {

	if (points.size() < 2)return;

	//0.Sort
	std::sort(points.begin(), points.end(),
		[](GradientPoint const& a, GradientPoint const& b) { return a.position < b.position; });

	//Generate empty texture
	if (!glIsTexture(gradientTextureMap)) {
		unsigned int size = gradientMapWidth * gradientMapHeight;
		unsigned char* data = new unsigned char[size * 4];
		for (unsigned int i = 0, a = 0; i < size; i++, a += 4) {
			data[a] = 255;
			data[a + 1] = 255;
			data[a + 2] = 255;
			data[a + 3] = 255;
		}
		glGenTextures(1, &gradientTextureMap);
		glBindTexture(GL_TEXTURE_2D, gradientTextureMap);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glTexImage2D(GL_TEXTURE_2D, 0, 4, gradientMapWidth, gradientMapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		delete[] data;
	}
	unsigned int size = gradientMapHeight;
	unsigned char* data = new unsigned char[size * 4];
	unsigned int buffIncr = 0;
	for (unsigned int i = 0; i < gradientMapHeight; i++){
		float f = (float)i / (float)gradientMapHeight;
		Vector4 c = _getColor(1.0f - f);

		data[buffIncr] = (unsigned char)(255.0f * c.x);
		data[buffIncr + 1] = (unsigned char)(255.0f * c.y);
		data[buffIncr + 2] = (unsigned char)(255.0f * c.z);
		data[buffIncr + 3] = (unsigned char)(255.0f * c.w);
		buffIncr += 4;
	}

	glBindTexture(GL_TEXTURE_2D, gradientTextureMap);
	glTexSubImage2D(GL_TEXTURE_2D, 0, gradientMapPositionIterator, 0, 1, gradientMapHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	gradientMapPositionIterator++;

	textureU = (float)gradientMapPositionIterator / (float)gradientMapWidth;

	glBindTexture(GL_TEXTURE_2D, gradientTextureMap);
	glTexSubImage2D(GL_TEXTURE_2D, 0, gradientMapPositionIterator, 0, 1, gradientMapHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	gradientMapPositionIterator++;

	textureU1 = (float)gradientMapPositionIterator / (float)gradientMapWidth;

	glBindTexture(GL_TEXTURE_2D, gradientTextureMap);
	glTexSubImage2D(GL_TEXTURE_2D, 0, gradientMapPositionIterator, 0, 1, gradientMapHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	gradientMapPositionIterator++;
}

Vector4 GUIGradientColor::_getColor(float f) {
	for (unsigned int i = 0; i < points.size() - 1; i++) {
		float d = f - points[i].position;		
		if (points[i].position <= f && points[i + 1].position >= f) {
			Vector4 c;			
			c.x = Math::Interpolate(points[i+1].color.x, points[i].color.x, d);
			c.y = Math::Interpolate(points[i+1].color.y, points[i].color.y, d);
			c.z = Math::Interpolate(points[i+1].color.z, points[i].color.z, d);
			c.w = Math::Interpolate(points[i+1].color.w, points[i].color.w, d);
			return c;
		}
		else if (f == points[i + 1].position) {
			return points[i + 1].color;
		}
	}
	return Vector4(1,1,1,1);
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
	shadowPosition(0,0),
	shadowColor(0,0,0,1),
	backgroundFill(FillType::NONE),
	backgroundColor(0,0,0,1),
	fontName(""),
	fontSize(12),
	fontColor(1,1,1,1),
	fontShadowPosition(),
	fontShadowColor(0,0,0,1),

	_fontValid(0),
	_fontHasShadow(0),

	fontHandle()

{	}

GUIStyle::~GUIStyle()
{
}

void GUIGradientColor::Deserialize(CFONode* node)
{
	node->getValueFloat("angle", angle);

	CFONode* object = node->GetFirstChild();
	while (object) {
		string classname = object->GetName();
		if (classname != "angle") {
			string value = object->GetValue();
			Vector4 color = Utils::StringToVector4(value);
			float pos = (float)atof(classname.c_str());
			add(color, pos);
		}
		object = object->GetNextNode();
	}

}

void GUIStyle::Deserialize(CFONode* node, ResourceManager* resourceManager)
{
	if (node->getValueFloat("radius", radius)) {
		radiusTopLeft = radiusTopRight = radiusBottomLeft = radiusBottomRight = radius;
	}

	node->getValueFloat("radiusTopLeft", radiusTopLeft);
	node->getValueFloat("radiusTopRight", radiusTopRight);
	node->getValueFloat("radiusBottomLeft", radiusBottomLeft);
	node->getValueFloat("radiusBottomRight", radiusBottomRight);

	if (node->getValueFloat("borderSize", borderSize)) {
		borderSizeTop = borderSizeBottom = borderSizeLeft = borderSizeRight = borderSize;
	}

	node->getValueFloat("borderSizeTop", borderSizeTop);
	node->getValueFloat("borderSizeBottom", borderSizeBottom);
	node->getValueFloat("borderSizeLeft", borderSizeLeft);
	node->getValueFloat("borderSizeRight", borderSizeRight);

	if (node->getValueVector4("borderColor", borderColor)) {
		borderColorLeft = borderColorRight = borderColorTop = borderColorBottom = borderColor;
	}

	node->getValueVector4("borderColorLeft", borderColorLeft);
	node->getValueVector4("borderColorRight", borderColorRight);
	node->getValueVector4("borderColorTop", borderColorTop);
	node->getValueVector4("borderColorBottom", borderColorBottom);

	node->getValueFloat("shadowSize", shadowSize);
	node->getValueFloat("shadowHardness", shadowHardness);
	node->getValueVector2("shadowPosition", shadowPosition);
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

	node->getValueFloat("lineHeight", lineHeight);

	if (node->getValueString("fontName", fontName)) {

		resourceManager->Get(fontHandle, fontName);

		node->getValueFloat("fontSize", fontSize);
		node->getValueVector4("fontColor", fontColor);
		node->getValueVector2("fontShadowPosition", fontShadowPosition);
		if (node->getValueVector4("fontShadowColor", fontShadowColor)) {
			_fontHasShadow = true;
		}
		
		_fontValid = true;

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

		if (shadowIndexArrayId > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 8, (void*)0);
			glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 8, (float*)(sizeof(GLfloat) * 2));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shadowIndexArrayId);
			glDrawElements(GL_TRIANGLES, shadowNumIndices, GL_UNSIGNED_INT, (void*)0);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, GUIGradientColor::gradientTextureMap);
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 8, (void*)0);
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 8, (float*)(sizeof(GLfloat) * 2));
		glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 8, (float*)(sizeof(GLfloat) * 6));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bodyIndexArrayId);
		glDrawElements(GL_TRIANGLES, bodyNumIndices, GL_UNSIGNED_INT, (void*)0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisable(GL_TEXTURE_2D);

		if (borderIndexArrayId > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 8, (void*)0);
			glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 8, (float*)(sizeof(GLfloat) * 2));
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
		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 8, (void*)0);
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 8, (float*)(sizeof(GLfloat) * 2));

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
	if (style->_fontValid && splitText.size() > 0) {
		glEnable(GL_TEXTURE_2D);
		float offset = 0;
		float line = max(style->lineHeight, textLineHeight);
		for (unsigned int i = 0; i < splitText.size(); i++) {
			if (style->_fontHasShadow) {
				glPushMatrix();
				glTranslatef(style->fontShadowPosition.x, style->fontShadowPosition.y - offset, 0);
				glColor4f(style->fontShadowColor.x, style->fontShadowColor.y, style->fontShadowColor.z, style->fontShadowColor.w);
				style->fontHandle->Draw(splitText[i], (unsigned int)style->fontSize);
				glPopMatrix();
			}
			glPushMatrix();
			glTranslatef(0, -offset, 0);
			glColor4f(style->fontColor.x, style->fontColor.y, style->fontColor.z, style->fontColor.w);
			style->fontHandle->Draw(splitText[i], (unsigned int)style->fontSize);
			glPopMatrix();
			offset += line;
		}
		glDisable(GL_TEXTURE_2D);
	}
}

void GUIRenderable::_generateGeometryData() {

	if (style->backgroundFill == GUIStyle::FillType::GRADIENT) {
		style->backgroundGradientColor.generate();
	}

	Vector2 halfSize = size * 0.5;
	Vector2 radiusCenterPoint;
	float radiusShadowOuterEdge = 0;
	float radiusBorderInnerEdge = 0;
	Vector2 cornerBorderInnerEdge;

	vector<Vector2>			bodyOutline;
	vector<Vector2>			shadowInline;
	vector<Vector2>			shadowOutline;
	vector<Vector2>			borderInline;
	vector<bool>			borderChangeColor;

	vector<Vector2>			vertexArray;
	vector<Vector4>			colorArray;
	vector<Vector2>			uvArray;

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
				shadowInline.push_back(Vector2(x, y) + style->shadowPosition);

				//Shadow outer edge
				x = radiusCenterPoint.x + (ux * (radiusBody[i] + shadowOuterSize));
				y = radiusCenterPoint.y + (uy * (radiusBody[i] + shadowOuterSize));
				shadowOutline.push_back(Vector2(x, y) + style->shadowPosition);
			}
		}
		else {
			bodyOutline.push_back(corners[i]);
			borderInline.push_back(Vector2(borderInnerTranslate.x + (corners[i].x * borderInnerScale.x),
				borderInnerTranslate.y + (corners[i].y * borderInnerScale.y)));
			borderChangeColor.push_back(true);
			shadowInline.push_back(Vector2((halfSize.x + shadowInnerSize) * quadrants[i].x, (halfSize.y + shadowInnerSize) * quadrants[i].y) + style->shadowPosition);
			shadowOutline.push_back(Vector2((halfSize.x + shadowOuterSize) * quadrants[i].x, (halfSize.y + shadowOuterSize) * quadrants[i].y) + style->shadowPosition);
		}
	}
	//4. Triangulate body
	if (style->backgroundFill != GUIStyle::FillType::NONE) {
		unsigned int offset = vertexArray.size();
		unsigned int nexti = 0;
		unsigned int midpointIndex = offset;
		float umin = 0, umax = 1;
		if (style->backgroundFill == GUIStyle::FillType::GRADIENT) {
			umin = style->backgroundGradientColor.textureU;
			umax = style->backgroundGradientColor.textureU1;
		}
		vertexArray.push_back(Vector2(0, 0));
		colorArray.push_back(Vector4(1, 1, 1, 1));
		uvArray.push_back(Vector2(umin + ((umax - umin) * 0.5f), 0.5f));
		for (unsigned int i = 0; i < bodyOutline.size(); i++) {
			nexti = (i + 1) % bodyOutline.size();
			indexArrayBody.push_back(midpointIndex);
			indexArrayBody.push_back(midpointIndex + i + 1);
			indexArrayBody.push_back(midpointIndex + nexti + 1);
			vertexArray.push_back(bodyOutline[i]);
			
			float ux = 0.5f + (bodyOutline[i].x / size.x);
			float uy = 0.5f + (bodyOutline[i].y / size.y);
			ux = umin + ((umax - umin) * ux);
			uvArray.push_back(Vector2(ux,uy));

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
		uvArray.push_back(Vector2(0, 0));
		unsigned int offset = vertexArray.size();
		for (unsigned int i = 0; i < shadowInline.size(); i++) {
			vertexArray.push_back(shadowInline[i]);
			vertexArray.push_back(shadowOutline[i]);
			colorArray.push_back(style->shadowColor);
			colorArray.push_back(Vector4(style->shadowColor.xyz(), 0));
			uvArray.push_back(Vector2(0, 0));
			uvArray.push_back(Vector2(0, 0));
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
		unsigned int colorIndex = 3;
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

			uvArray.push_back(Vector2(0, 0));
			uvArray.push_back(Vector2(0, 0));
			uvArray.push_back(Vector2(0, 0));
			uvArray.push_back(Vector2(0, 0));

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
	_generateVertexBuffer(vertexArray, colorArray, uvArray);
}

void GUIRenderable::update() {

	if (style == 0)return;

	_generateGeometryData();
}

void GUIRenderable::_generateVertexBuffer(vector<Vector2>& vertices, vector<Vector4>& colors, vector<Vector2>& uv) {

	if (vertexArrayId > 0)glDeleteBuffers(1, &vertexArrayId);

	unsigned int vertexCount = vertices.size();

	GLfloat* tmpVertexArray = new GLfloat[8 * vertexCount];

	unsigned int vi = 0;
	for (unsigned int i = 0; i < vertexCount; i++){
		tmpVertexArray[vi] = vertices[i].x;
		tmpVertexArray[vi + 1] = vertices[i].y;
		tmpVertexArray[vi + 2] = colors[i].x;
		tmpVertexArray[vi + 3] = colors[i].y;
		tmpVertexArray[vi + 4] = colors[i].z;
		tmpVertexArray[vi + 5] = colors[i].w;
		tmpVertexArray[vi + 6] = uv[i].x;
		tmpVertexArray[vi + 7] = uv[i].y;
		vi += 8;
	}

	glGenBuffers(1, &vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * vertexCount, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 8 * vertexCount, tmpVertexArray);
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

//TODO Split long words
void GUIRenderable::setTextWithWrapping(string text) {
	splitText.clear();
	//cout << "set text: "<<text << endl;
	if(style && style->_fontValid){
		vector<string> words;
		int count = Utils::Explode(' ', text, words);
		if (count > 1) {
			float counter = 0;
			string line = "";
			//cout << "------------split:" << text << endl;
			for (unsigned int i = 0; i < words.size(); i++) {
				//Vector2 s = Vector2();	// style->font.measure(words[i]);
				//textLineHeight = s.y;
				float width = style->fontHandle->getWidth(words[i], (unsigned int)style->fontSize);
				//cout << words[i] << " / " << width <<" / "<< size.x << endl;
				if (counter + width > size.x) {
					splitText.push_back(line);
					line = words[i];
					counter = width;
				}
				else {
					counter += width;
					if (line.size() > 0)line = line + " " + words[i];
					else line = line + words[i];
				}
			}
			if (line.length() > 0)splitText.push_back(line);
		}
		else {
			splitText.push_back(text);
		}
	}
}
void GUIRenderable::setText(string text) {
	splitText.clear();
	splitText.push_back(text);
}