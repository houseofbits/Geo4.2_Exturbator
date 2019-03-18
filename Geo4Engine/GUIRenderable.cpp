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
	float radiusBorder[] = {
		max(style->borderSizeTop, style->borderSizeLeft),
		max(style->borderSizeBottom, style->borderSizeLeft),
		max(style->borderSizeBottom, style->borderSizeRight),
		max(style->borderSizeTop, style->borderSizeRight)
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

	for (int i = 0; i < 4; i++) {

		double step = Math::HALF_PI / max(4.0f, Math::Ceil(Math::HALF_PI * radiusBody[i] / 5));
		
		float shadowOuterSize = style->shadowSize;
		float shadowInnerSize = (style->shadowSize * style->shadowHardness) - 1.0f;

		//1. Generate body siluette conventional way
		if (radiusBody[i] > 0) {
			radiusCenterPoint = corners[i] - Vector2(radiusBody[i] * quadrants[i].x, radiusBody[i] * quadrants[i].y);
			for (double u = 0; u <= Math::HALF_PI; u += step) {
				float ux = (float)cos(Math::HALF_PI + u + (Math::HALF_PI * i));
				float uy = (float)sin(Math::HALF_PI + u + (Math::HALF_PI * i));
				float x = radiusCenterPoint.x + (ux * radiusBody[i]);
				float y = radiusCenterPoint.y + (uy * radiusBody[i]);
				bodyOutline.push_back(Vector2(x, y));

				x = radiusCenterPoint.x + (ux * (radiusBody[i] + shadowInnerSize));
				y = radiusCenterPoint.y + (uy * (radiusBody[i] + shadowInnerSize));
				shadowInline.push_back(Vector2(x,y) + shadowPos);

				x = radiusCenterPoint.x + (ux * (radiusBody[i] + shadowOuterSize));
				y = radiusCenterPoint.y + (uy * (radiusBody[i] + shadowOuterSize));
				shadowOutline.push_back(Vector2(x, y) + shadowPos);
			}
		}
		else {
			bodyOutline.push_back(corners[i]);
			shadowInline.push_back(Vector2((halfSize.x + shadowInnerSize) * quadrants[i].x, (halfSize.y + shadowInnerSize) * quadrants[i].y) + shadowPos);
			shadowOutline.push_back(Vector2((halfSize.x + shadowOuterSize) * quadrants[i].x, (halfSize.y + shadowOuterSize) * quadrants[i].y) + shadowPos);
		}
		//2. Generate border corner cases
		if (radiusBody > 0) {
			if (radiusBorderInnerEdge < radiusBody[i]) {

			}
			else {
				//solid corner
			}
		}
	}

	//3. Generate border side triangles
	if (style->borderSizeLeft > 0) {
		unsigned int offset = vertexArray.size();
		vertexArray.push_back(corners[0] - Vector2(0, radiusBody[0]));
		vertexArray.push_back(corners[0] - Vector2(-style->borderSizeLeft, radiusBody[0]));
		vertexArray.push_back(corners[1] - Vector2(-style->borderSizeLeft, -radiusBody[1]));
		vertexArray.push_back(corners[1] - Vector2(0, -radiusBody[1]));
		
		colorArray.push_back(Vector4(0, 0, 1, 1));
		colorArray.push_back(Vector4(0, 0, 1, 1));
		colorArray.push_back(Vector4(0, 0, 1, 1));
		colorArray.push_back(Vector4(0, 0, 1, 1));

		indexArrayBorder.push_back(offset);
		indexArrayBorder.push_back(offset + 2);
		indexArrayBorder.push_back(offset + 3);

		indexArrayBorder.push_back(offset);
		indexArrayBorder.push_back(offset + 1);
		indexArrayBorder.push_back(offset + 2);
	}
	if (style->borderSizeRight > 0) {
		unsigned int offset = vertexArray.size();
		vertexArray.push_back(corners[2] - Vector2(0, radiusBody[0]));
		vertexArray.push_back(corners[2] - Vector2(style->borderSizeLeft, radiusBody[0]));
		vertexArray.push_back(corners[3] - Vector2(style->borderSizeLeft, radiusBody[1]));
		vertexArray.push_back(corners[3] - Vector2(0, radiusBody[1]));

		colorArray.push_back(Vector4(0, 0, 1, 1));
		colorArray.push_back(Vector4(0, 0, 1, 1));
		colorArray.push_back(Vector4(0, 0, 1, 1));
		colorArray.push_back(Vector4(0, 0, 1, 1));

		indexArrayBorder.push_back(offset);
		indexArrayBorder.push_back(offset + 2);
		indexArrayBorder.push_back(offset + 3);

		indexArrayBorder.push_back(offset);
		indexArrayBorder.push_back(offset + 1);
		indexArrayBorder.push_back(offset + 2);
	}
	if (style->borderSizeTop > 0) {

	}
	if (style->borderSizeBottom > 0) {

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

	_generateIndexBuffer(indexArrayBorder, borderIndexArrayId, borderNumIndices);

	//6. Generate VBO data
	_generateVertexBuffer(vertexArray, colorArray);
}
/*
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
*/

void GUIRenderable::generateGeometry() {

	if (style == 0)return;

	_generateGeometryData();

	/*
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
	*/
}
/*
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
*/
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