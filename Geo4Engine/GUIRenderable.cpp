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
	shadowSize(0),
	shadowSizeTop(0),
	shadowSizeBottom(0),
	shadowSizeLeft(0),
	shadowSizeRight(0),
	backgroundFill(FillType::NONE),
	backgroundColor(0,0,0,1)
{	}

GUIStyle::~GUIStyle()
{
}

GUIRenderable::GUIRenderable() : style(0),
	size(),
	bodyGeometryArray(0),
	bodyIndexArray(0),
	bodyNumIndices(0),
	borderGeometryArray(0),
	borderIndexArray(0),
	borderNumIndices(0),
	shadowGeometryArray(0),
	shadowIndexArray(0),
	shadowNumIndices(0)
{


}


GUIRenderable::~GUIRenderable()
{
}

void GUIRenderable::Draw() {
	
	glColor3f(1, 1, 1);

	if (shadowGeometryArray > 0) {
		glColor4f(0, 1, 0, 1);
		glBindBuffer(GL_ARRAY_BUFFER, shadowGeometryArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shadowIndexArray);
		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, (void*)0);
		//glColorPointer(3, GL_FLOAT, 0, (void*)cOffset);

		glDrawElements(GL_TRIANGLES, shadowNumIndices, GL_UNSIGNED_INT, (void*)0);
		//glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	
	if (bodyGeometryArray > 0 && style != 0) {
		if (style->backgroundFill = GUIStyle::FillType::SOLID) {
			glColor4f(style->backgroundColor.x, style->backgroundColor.y, style->backgroundColor.z, style->backgroundColor.w);
		}
		else {
			//gradient
		}
		glBindBuffer(GL_ARRAY_BUFFER, bodyGeometryArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bodyIndexArray);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, (void*)0);
		glDrawElements(GL_TRIANGLES, bodyNumIndices, GL_UNSIGNED_INT, (void*)0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if (borderGeometryArray > 0) {
		glColor3f(0, 0, 1);

		glBindBuffer(GL_ARRAY_BUFFER, borderGeometryArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, borderIndexArray);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, (void*)0);
		glDrawElements(GL_TRIANGLES, borderNumIndices, GL_UNSIGNED_INT, (void*)0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
	


}

void GUIRenderable::_generateShell(vector<Vector2>& points, 
	Vector2 size, 
	float r1, 
	float r2, 
	float r3, 
	float r4,
	float offsetL,
	float offsetR, 
	float offsetT, 
	float offsetB) {

	Vector2 halfSize = size * 0.5;
	Vector2 corner, centerPoint, quadrant;
	float radius = 0;
	Vector2 offset;
	//Generate points loop by walking counter clockwise and generating each corner radius	
	for (int i = 0; i < 4; i++) {
		switch (i) {
		case 0:
			radius = r1 + max(offsetL, offsetT);
			quadrant = Vector2(-1, 1);
			offset = Vector2(offsetL, offsetT);
			break;
		case 1:
			radius = r2 + max(offsetL, offsetB);
			quadrant = Vector2(-1, -1);
			offset = Vector2(offsetL, offsetB);
			break;
		case 2:
			radius = r3 + max(offsetR, offsetB);
			quadrant = Vector2(1, -1);
			offset = Vector2(offsetR, offsetB);
			break;
		case 3:
			radius = r4 + max(offsetR, offsetT);
			quadrant = Vector2(1, 1);
			offset = Vector2(offsetR, offsetT);
			break;
		};
		if (radius < 3)radius = 0;
		double step = Math::HALF_PI / max(4, Math::Ceil(Math::HALF_PI * radius / 5));
		corner = Vector2((halfSize.x + offset.x) * quadrant.x, (halfSize.y + offset.y) * quadrant.y);
		if (radius > 0) {
			centerPoint = corner - Vector2(radius * quadrant.x, radius * quadrant.y);
			for (double u = 0; u <= Math::HALF_PI; u += step) {
				float x = (float)cos(Math::HALF_PI + u + (Math::HALF_PI * i));
				float y = (float)sin(Math::HALF_PI + u + (Math::HALF_PI * i));
				x = centerPoint.x + (x * radius);
				y = centerPoint.y + (y * radius);
				points.push_back(Vector2(x, y));
			}
		}
		else points.push_back(corner);
	}
}

void GUIRenderable::generateGeometry() {

	if (style == 0)return;

	vector<Vector2> bodyPoints;
	float r1=0, r2=0, r3=0, r4=0;
	//[radius] overrides all other radiuses
	if (style->radius > 0) {
		r1 = r2 = r3 = r4 = style->radius;
	}
	else {
		r1 = style->radiusTopLeft;
		r2 = style->radiusBottomLeft;
		r3 = style->radiusBottomRight;
		r4 = style->radiusTopRight;
	}

	//Generate body geometry
	_generateShell(bodyPoints,
		size,
		r1, r2, r3, r4,
		0, 0, 0, 0);

	if (style->backgroundFill != GUIStyle::FillType::NONE) {
		_generateSolidFromPointSet(&bodyPoints, bodyIndexArray, bodyGeometryArray, bodyNumIndices);
	}
	//Generate border geometry
	vector<Vector2> innerPoints;
	//[borderSize] override all other sizes
	if (style->borderSize > 0) {
		_generateShell(innerPoints,
			size,
			r1, r2, r3, r4,
			-style->borderSize,
			-style->borderSize,
			-style->borderSize,
			-style->borderSize);
		_generateStripFromPointSet(&innerPoints, &bodyPoints, borderIndexArray, borderGeometryArray, borderNumIndices);
	}
	else if(style->borderSizeTop > 0 ||
		style->borderSizeBottom > 0 || 
		style->borderSizeLeft > 0 || 
		style->borderSizeRight > 0){

		_generateShell(innerPoints,
			size,
			r1, r2, r3, r4,
			-style->borderSizeLeft,
			-style->borderSizeRight,
			-style->borderSizeTop,
			-style->borderSizeBottom);
		_generateStripFromPointSet(&innerPoints, &bodyPoints, borderIndexArray, borderGeometryArray, borderNumIndices);
	}

	vector<Vector2> outerPoints;
	//[shadowSize] override all other sizes
	if (style->shadowSize > 0) {
		_generateShell(outerPoints,
			size,
			r1, r2, r3, r4,
			style->shadowSize,
			style->shadowSize,
			style->shadowSize,
			style->shadowSize);
		_generateStripFromPointSet(&bodyPoints, &outerPoints, shadowIndexArray, shadowGeometryArray, shadowNumIndices);
	}
	else if(style->shadowSizeTop > 0 ||
		style->shadowSizeBottom > 0 || 
		style->shadowSizeLeft > 0 || 
		style->shadowSizeRight > 0){

		_generateShell(outerPoints,
			size,
			r1, r2, r3, r4,
			style->shadowSizeLeft,
			style->shadowSizeRight,
			style->shadowSizeTop,
			style->shadowSizeBottom);
		_generateStripFromPointSet(&bodyPoints, &outerPoints, shadowIndexArray, shadowGeometryArray, shadowNumIndices);
	}

}

//Generate body VBO
void GUIRenderable::_generateSolidFromPointSet(vector<Vector2>* shell,
	GLuint& indexArray,
	GLuint& vertexArray,
	unsigned int& indexCount) {

	//Triangulate geometry outline into solid
	Triangulation triang;
	vector<unsigned int> indicesOut;
	triang.EarClipTrinagulate(*shell, indicesOut);

	//Generate vertex and indexbuffers
	GLfloat* tmpGeometry = new GLfloat[3 * shell->size()];
	GLuint* tmpIndices = new GLuint[indicesOut.size()];
	unsigned int vi = 0;
	for (unsigned int i = 0; i < shell->size(); i++) {
		tmpGeometry[vi] = (*shell)[i].x;
		tmpGeometry[vi + 1] = (*shell)[i].y;
		tmpGeometry[vi + 2] = 0;
		vi += 3;
	}
	for (unsigned int i = 0; i < indicesOut.size(); i++){
		tmpIndices[i] = indicesOut[i];
	}
	indexCount = indicesOut.size();

	//Generate VBO data and delete temporary vertex data
	if (vertexArray > 0)glDeleteBuffers(1, &vertexArray);
	if (indexArray > 0)glDeleteBuffers(1, &indexArray);

	glGenBuffers(1, &vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArray);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * vi, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * vi, tmpGeometry);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indexCount, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*indexCount, tmpIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] tmpGeometry;
	delete[] tmpIndices;
}

void GUIRenderable::_generateStripFromPointSet(vector<Vector2>* inner,
	vector<Vector2>* outer,
	GLuint& indexArray,
	GLuint& vertexArray,
	unsigned int& indexCount) {

	//Triangulate geometry outline into solid
	vector<Vector2> shell = *outer;
	shell.push_back((*outer)[0]);
	shell.push_back((*inner)[0]);
	for (int  i = inner->size()-1; i >= 0; i--){
		shell.push_back((*inner)[i]);
	}

	Triangulation triang;
	vector<unsigned int> indicesOut;

	triang.EarClipTrinagulate(shell, indicesOut);

	//Generate vertex and indexbuffers
	GLfloat* tmpGeometry = new GLfloat[3 * shell.size()];
	GLuint* tmpIndices = new GLuint[indicesOut.size()];
	unsigned int vi = 0;
	for (unsigned int i = 0; i < shell.size(); i++) {
		tmpGeometry[vi] = shell[i].x;
		tmpGeometry[vi + 1] = shell[i].y;
		tmpGeometry[vi + 2] = 0;
		vi += 3;
	}
	for (unsigned int i = 0; i < indicesOut.size(); i++) {
		tmpIndices[i] = indicesOut[i];
	}
	indexCount = indicesOut.size();

	//Generate VBO data and delete temporary vertex data
	if (vertexArray > 0)glDeleteBuffers(1, &vertexArray);
	if (indexArray > 0)glDeleteBuffers(1, &indexArray);

	glGenBuffers(1, &vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArray);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * vi, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * vi, tmpGeometry);

	//glBufferSubData(GL_ARRAY_BUFFER, vSize, cSize, colors);          // copy cols after norms

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArray);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indexCount, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*indexCount, tmpIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] tmpGeometry;
	delete[] tmpIndices;
}