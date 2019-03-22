#include "Geo4.h"

FT_Library library_;

const unsigned int TrueTypeFontFace::maxCharactersCount = 500;
const unsigned int TrueTypeFontFace::textureMapWidth = 512;
const unsigned int TrueTypeFontFace::textureMapHeight = 512;
const unsigned int TrueTypeFontFace::fontMapPadding = 1;

const unsigned int innerFontSize = 10;

TrueTypeFontFace::TrueTypeFontFace() : activeGlyphs(),
	textureMapCurrentLineU(0),
	textureMapCurrentLineV(0),
	textureMapNextLineV(0),
	faceTextureMap(0),
	ftFace(),
	currentIndexCount(0),
	indexArrayId(0),
	vertexArrayId(0),
	tmpVertexArray(0)
{

}

TrueTypeFontFace::~TrueTypeFontFace()
{
	FT_Done_FreeType(library_);

	if(tmpVertexArray)delete[] tmpVertexArray;
}

bool TrueTypeFontFace::Load(std::string filename) {

	FT_Error error = FT_Init_FreeType(&library_);

	if (error != 0) {
		cout << "Could not initialize the FreeType library. Exiting." << endl;
		return false;
	}

	error = FT_New_Face(library_, filename.c_str(), 0, &ftFace);

	if (error != 0) {
		cout << "Error: "<< error << endl;
		return false;
	}

	if (ftFace->charmap == 0 && ftFace->num_charmaps > 0)
		FT_Select_Charmap(ftFace, ftFace->charmaps[0]->encoding);

	_createBufferObjects();

	_createEmptyTextureMap();

	Draw("Test", 30);

	return true;
}

void TrueTypeFontFace::Draw(std::string text, float size) {
	currentIndexCount = 0;
	FT_UInt charIndex;
	TrueTypeActiveGlyph glyph;
	unsigned int vertexCounter = 0;
	float positionX = 0;
	for (unsigned int i = 0; i < text.length(); i++){
		
		if (i > maxCharactersCount)break;

		charIndex = FT_Get_Char_Index(ftFace, text[i]);
		if (!_getGlyph(charIndex, glyph)) {
			if (!_addGlyph(charIndex, glyph)) {
				continue;
			}
		}

		float width = glyph.size.x * size;
		float height = glyph.size.y * size;
		float bearingX = glyph.bearing.x * size;
		float bearingY = glyph.bearing.y * size;

		
		//Calculate glyph position
		//https://www.libsdl.org/projects/SDL_ttf/docs/metrics.png

		//Add glyph to vertex array data
		//LU
		tmpVertexArray[vertexCounter] = positionX + bearingX;					//x
		tmpVertexArray[vertexCounter + 1] = bearingY;							//y
		tmpVertexArray[vertexCounter + 2] = glyph.uvPos.x;						//u
		tmpVertexArray[vertexCounter + 3] = glyph.uvPos.y + glyph.uvSize.y;		//v
		vertexCounter += 4;
		//LD
		tmpVertexArray[vertexCounter] = positionX + bearingX;				//x
		tmpVertexArray[vertexCounter + 1] = bearingY - height;				//y
		tmpVertexArray[vertexCounter + 2] = glyph.uvPos.x;					//u
		tmpVertexArray[vertexCounter + 3] = glyph.uvPos.y;					//v
		vertexCounter += 4;
		//RD
		tmpVertexArray[vertexCounter] = positionX + width + bearingX;		//x
		tmpVertexArray[vertexCounter + 1] = bearingY - height;				//y
		tmpVertexArray[vertexCounter + 2] = glyph.uvPos.x + glyph.uvSize.x;	//u
		tmpVertexArray[vertexCounter + 3] = glyph.uvPos.y;	//v
		vertexCounter += 4;
		//RU
		tmpVertexArray[vertexCounter] = positionX + width + bearingX;		//x
		tmpVertexArray[vertexCounter + 1] = bearingY;						//y
		tmpVertexArray[vertexCounter + 2] = glyph.uvPos.x + glyph.uvSize.x;	//u
		tmpVertexArray[vertexCounter + 3] = glyph.uvPos.y + glyph.uvSize.y;	//v
		vertexCounter += 4;

		currentIndexCount += 4;		

		positionX += glyph.advance * size;
	}

	if (currentIndexCount > 0 && (maxCharactersCount * 4 * 4) >= vertexCounter) {
		//Update vbo with vertex data
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vertexCounter, tmpVertexArray);
	}
	DrawCached();
	
}

//Draw previously prepared data.
//This is useful for repeated text with multiple effect layers
void TrueTypeFontFace::DrawCached() {
	if (currentIndexCount > 0) {
		//Render vbo
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 4, (void*)0);
		glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 4, (float*)(sizeof(GLfloat) * 2));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayId);
		glDrawElements(GL_QUADS, currentIndexCount, GL_UNSIGNED_INT, (void*)0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

GLubyte* invertPixmap(const FT_Bitmap& bitmap)
{
	unsigned int width = bitmap.width;
	unsigned int height = bitmap.rows;

	GLubyte* inverse = new GLubyte[2 * width * height];
	GLubyte* inverse_ptr = inverse;

	for (unsigned int r = 0; r < bitmap.rows; r++) {

		GLubyte* bitmap_ptr = &bitmap.buffer[bitmap.pitch * (bitmap.rows - r - 1)];

		for (unsigned int p = 0; p < bitmap.width; p++) {
			*inverse_ptr++ = 0xff;
			*inverse_ptr++ = *bitmap_ptr++;
		}

		inverse_ptr += 2 * (width - bitmap.pitch);
	}
	return inverse;
}

bool TrueTypeFontFace::_addGlyph(unsigned int charIndex, TrueTypeActiveGlyph& out) {

	FT_Error error = FT_Load_Glyph(ftFace, charIndex, FT_LOAD_DEFAULT);

	if (error != 0)return false;

	error = FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL);

	if (error != 0)return false;

	unsigned int width = ftFace->glyph->bitmap.width;
	unsigned int height = ftFace->glyph->bitmap.rows;

	GLubyte* inverted_pixmap = invertPixmap(ftFace->glyph->bitmap);

	if ((textureMapCurrentLineU + width + (fontMapPadding * 2)) > textureMapWidth) {
		textureMapCurrentLineU = 0;
		textureMapCurrentLineV = textureMapNextLineV;
		textureMapNextLineV = 0;
	}
	if ((textureMapCurrentLineV + height + (fontMapPadding * 2)) > textureMapHeight) {
		cout << "True Type font texture run out of space" << endl;
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, faceTextureMap);
	glTexSubImage2D(GL_TEXTURE_2D, 0, textureMapCurrentLineU + fontMapPadding, textureMapCurrentLineV + fontMapPadding, width, height, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, inverted_pixmap);

	TrueTypeActiveGlyph* glyph = new TrueTypeActiveGlyph(charIndex);

	//glyph->fontSize = fontSize;

	glyph->uvPos = Vector2(((float)textureMapCurrentLineU + (float)fontMapPadding) / (float)textureMapWidth, ((float)textureMapCurrentLineV + (float)fontMapPadding) / (float)textureMapHeight);
	glyph->uvSize = Vector2((float)width / (float)textureMapWidth, (float)height / (float)textureMapHeight);

	glyph->size = Vector2((float)ftFace->glyph->metrics.width / 64.0f / innerFontSize, (float)ftFace->glyph->metrics.height / 64.0f / innerFontSize);
	glyph->bearing = Vector2((float)ftFace->glyph->metrics.horiBearingX / 64.0f / innerFontSize, (float)ftFace->glyph->metrics.horiBearingY / 64.0f / innerFontSize);
	glyph->advance = (float)ftFace->glyph->metrics.horiAdvance / 64.0f / innerFontSize;

	textureMapCurrentLineU += (width + (fontMapPadding * 2));
	if (textureMapCurrentLineV + height > textureMapNextLineV)textureMapNextLineV = textureMapCurrentLineV + height + (fontMapPadding * 2);

	activeGlyphs.push_back((*glyph));

	out = activeGlyphs.back();

	delete glyph;

	std::sort(activeGlyphs.begin(), activeGlyphs.end(),
		[](TrueTypeActiveGlyph const& a, TrueTypeActiveGlyph const& b) { return a.charIndex < b.charIndex; });

	return true;
}

bool TrueTypeFontFace::_getGlyph(unsigned int charIndex, TrueTypeActiveGlyph& out) {

	std::vector<TrueTypeActiveGlyph>::iterator it = std::find_if(activeGlyphs.begin(), activeGlyphs.end(),
		[&](TrueTypeActiveGlyph const& p) { return p.charIndex == charIndex; });

	if (it != activeGlyphs.end()) {
		out = (*it);
		return true;
	}
	return false;
}

void TrueTypeFontFace::_setSize(unsigned int size) {

	FT_Error error = FT_Set_Char_Size(ftFace,
		(FT_F26Dot6)(size * 64),
		(FT_F26Dot6)(size * 64),
		100,
		100);

	if (error != 0) {
		cout << "Error: " << error << endl;
		return;
	}
}

void TrueTypeFontFace::_createBufferObjects() {

	unsigned int numIndices = maxCharactersCount * 4;

	if(tmpVertexArray == 0)tmpVertexArray = new GLfloat[maxCharactersCount * 4 * 4];

	GLuint* tmpIndicesArray = new GLuint[numIndices];

	for (unsigned int i = 0; i < numIndices; i++){
		tmpIndicesArray[i] = i;
	}
	for (unsigned int i = 0; i < numIndices * 4; i++) {
		tmpVertexArray[i] = 0.0f;
	}

	glGenBuffers(1, &vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * numIndices, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 4 * numIndices, tmpVertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numIndices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*numIndices, tmpIndicesArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] tmpIndicesArray;
}
void TrueTypeFontFace::_createEmptyTextureMap() {

	if (faceTextureMap > 0)glDeleteTextures(1, &faceTextureMap);
	unsigned int size = textureMapWidth * textureMapHeight;
	unsigned char* data = new unsigned char[size * 4];
	for (unsigned int i = 0, a=0; i < size; i++, a+=4) {
		data[a] = 255;
		data[a + 1] = 255;
		data[a + 2] = 255;
		data[a + 3] = 0;
	}
	glGenTextures(1, &faceTextureMap);
	glBindTexture(GL_TEXTURE_2D, faceTextureMap);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, textureMapWidth, textureMapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	delete[] data;
}

Vector2 TrueTypeFontFace::getSize(std::string text, float size) {
	
	//TODO 

	return Vector2(); 
}