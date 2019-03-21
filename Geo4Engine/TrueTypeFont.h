#pragma once

#include <cmath>
#include <map>
#include <list>
#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H

class TrueTypeActiveGlyph {
public:
	TrueTypeActiveGlyph() {}

	unsigned int charIndex;
	float offsetU;
	float offsetV;
	float width;
	float height;
};

/*

Init font
1.Load ttface
2.Create empty texture of predefined size
3.Create vertex and index buffers of predefinde size

Each time draving font
1.Generate text checksum
2.If checksum is same as before draw the font
3.If text has changed:
4.Generate nonexisting characters and save into texture
5.Regenerate vertex array vbo
6.Draw
7.Save checksum


*/
class TrueTypeFontFace
{
public:
	TrueTypeFontFace();
	virtual ~TrueTypeFontFace();

	bool Load(std::string filename,unsigned int size);

	std::vector<TrueTypeActiveGlyph> activeGlyphs;
	unsigned int textureMapCurrentLineU;
	unsigned int textureMapCurrentLineV;
	GLuint		faceTextureMap;
	FT_Face		ftFace;
};

class TrueTypeTextRenderable {
public:

	void Draw(std::string, TrueTypeFontFace& font) {}

	GLuint	vertexBufferId;
	GLuint	indexBufferId;

	unsigned int textChecksum;
};



