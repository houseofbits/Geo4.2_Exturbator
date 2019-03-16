//*******************************************************************
//glfont2.h -- Header for glfont2.cpp
//Copyright (c) 1998-2002 Brad Fish
//See glfont.html for terms of use
//May 14, 2002
//*******************************************************************

#ifndef GLFONT2_H
#define GLFONT2_H

//#include "OpenGL.h"
#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include "Math/HVector2.h"
#include "ResourceManager.h"

using namespace std;

//*******************************************************************
//GLFont Interface
//*******************************************************************

//glFont namespace
namespace glfont
{
	class GLFont;	
}

//glFont class
class glfont::GLFont
{
//private:
public:
	//glFont character structure
	typedef struct
	{
		float dx, dy;
		float tx1, ty1;
		float tx2, ty2;
	} GLFontChar;

	//glFont header structure
	typedef struct
	{
		int tex;
		int tex_width, tex_height;
		int start_char, end_char;
		GLFontChar *chars;
	} head;
	
	head header;

public:
	int start_char;
	//Constructor
	GLFont ();

	//Destructor
	~GLFont ();

public:

	//Creates the glFont
	bool Create (const char *file_name, int tex);
	bool Create (const std::string &file_name, int tex);

	//Destroys the glFont
	void Destroy (void);

	//Texture size retrieval methods
	void GetTexSize (std::pair<int, int> *size);
	int GetTexWidth (void);
	int GetTexHeight (void);

	//Character interval retrieval methods
	void GetCharInterval (std::pair<int, int> *interval);
	int GetStartChar (void);
	int GetEndChar (void);

	//Character size retrieval methods
	void GetCharSize (int c, std::pair<int, int> *size);
	int GetCharWidth (int c);
	int GetCharHeight (int c);

	//Calculates the size in pixels of a character array
	template<class T> void GetStringSize (const T *text,
		std::pair<int, int> *size)
	{
		const T *i;
		GLFontChar *glfont_char;
		float width;
		
		//Height is the same for now...might change in future
		size->second = (int)(header.chars[header.start_char].dy *	header.tex_height);

		//Calculate width of string
		width = 0.0F;
		for (i = text; *i != (T)'\0'; i++)
		{
			//Make sure character is in range
			if (*i < header.start_char || *i > header.end_char)
				continue;

			//Get pointer to glFont character
			glfont_char = &header.chars[*i - header.start_char];

			//Get width and height
			width += glfont_char->dx * header.tex_width;		
		}

		//Save width
		size->first = (int)width;
	}
	
	//Template function to calculate size of a std::basic_string
	template<class T> void GetStringSize (
		const std::basic_string<T> &text, std::pair<int, int> *size)
	{
		unsigned int i;
		T *c;
		GLFontChar *glfont_char;
		float width;
		
		//Height is the same for now...might change in future
		size->second = (int)(header.chars[header.start_char].dy * header.tex_height);

		//Calculate width of string
		width = 0.0F;
		for (i = 0; i < text.size(); i++)
		{
			//Make sure character is in range
			c = text[i];
			if (c < header.start_char || c > header.end_char)
				continue;

			//Get pointer to glFont character
			glfont_char = &header.chars[c - header.start_char];

			//Get width and height
			width += glfont_char->dx * header.tex_width;		
		}

		//Save width
		size->first = (int)width;
	}

	//Begins text output with this font
	void Begin (void);

	//Template function to output a character array
	template<class T> void DrawString (const T *text, float x,
		float y)
	{
		const T *i;
		GLFontChar *glfont_char;
		float width, height;
		
		//Begin rendering quads
		glBegin(GL_QUADS);
		
		//Loop through characters
		for (i = text; *i != (T)'\0'; i++)
		{
			//Make sure character is in range
			if (*i < header.start_char || *i > header.end_char)
				continue;

			//Get pointer to glFont character
			glfont_char = &header.chars[*i - header.start_char];

			//Get width and height
			width = glfont_char->dx * header.tex_width;
			height = glfont_char->dy * header.tex_height;
			
			//Specify vertices and texture coordinates
			glTexCoord2f(glfont_char->tx1, glfont_char->ty1);
			glVertex3f(x, y, 0.0F);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty2);
			glVertex3f(x, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty2);
			glVertex3f(x + width, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty1);
			glVertex3f(x + width, y, 0.0F);
		
			//Move to next character
			x += width;
		}

		//Stop rendering quads
		glEnd();
	}

	//Template function to draw a std::basic_string
	template<class T> void DrawString (
		const std::basic_string<T> &text, float x, float y)
	{
		unsigned int i;
		T c;
		GLFontChar *glfont_char;
		float width, height;
		
		//Begin rendering quads
		glBegin(GL_QUADS);
		
		//Loop through characters
		for (i = 0; i < text.size(); i++)
		{
			//Make sure character is in range
			c = text[i];
			if (c < header.start_char || c > header.end_char)
				continue;

			//Get pointer to glFont character
			glfont_char = &header.chars[c - header.start_char];

			//Get width and height
			width = glfont_char->dx * header.tex_width;
			height = glfont_char->dy * header.tex_height;
			
			//Specify vertices and texture coordinates
			glTexCoord2f(glfont_char->tx1, glfont_char->ty1);
			glVertex3f(x, y, 0.0F);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty2);
			glVertex3f(x, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty2);
			glVertex3f(x + width, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty1);
			glVertex3f(x + width, y, 0.0F);
		
			//Move to next character
			x += width;
		}

		//Stop rendering quads
		glEnd();
	}

	//Template function to output a scaled character array
	template<class T> void DrawString (const T *text, float scalar,
		float x, float y)
	{
		const T *i;
		GLFontChar *glfont_char;
		float width, height;
		
		//Begin rendering quads
		glBegin(GL_QUADS);
		
		//Loop through characters
		for (i = text; *i != (T)'\0'; i++)
		{
			//Make sure character is in range
			if (*i < header.start_char || *i > header.end_char)
				continue;

			//Get pointer to glFont character
			glfont_char = &header.chars[*i - header.start_char];

			//Get width and height
			width = (glfont_char->dx * header.tex_width) * scalar;
			height = (glfont_char->dy * header.tex_height) * scalar;
			
			//Specify vertices and texture coordinates
			glTexCoord2f(glfont_char->tx1, glfont_char->ty1);
			glVertex3f(x, y, 0.0F);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty2);
			glVertex3f(x, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty2);
			glVertex3f(x + width, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty1);
			glVertex3f(x + width, y, 0.0F);
		
			//Move to next character
			x += width;
		}

		//Stop rendering quads
		glEnd();
	}

	//Template function to output a scaled std::basic_string
	void DrawString (const std::string &text, float scalar, float x, float y)
	{
//		cout<<text<<endl;cout<<text.size()<<endl;

		unsigned int i;
		char c;
		GLFontChar *glfont_char=0;
		float width, height;
		
		//Begin rendering quads
		glBegin(GL_QUADS);
		
		//Loop through characters
		for (i = 0; i < text.size(); i++)
		{
			//Make sure character is in range
			c = text[i];
		
			if (c < header.start_char || c > header.end_char)
				continue;

		//	cout<<c<<" "<<int(c)<<" "<<start_char<<endl;
			//Get pointer to glFont character
			glfont_char = &header.chars[c - header.start_char];

			//Get width and height
			width = (glfont_char->dx * header.tex_width) * scalar;
			height = (glfont_char->dy * header.tex_height) * scalar;
			
			//Specify vertices and texture coordinates
			glTexCoord2f(glfont_char->tx1, glfont_char->ty1);
			glVertex3f(x, y, 0.0F);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty2);
			glVertex3f(x, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty2);
			glVertex3f(x + width, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty1);
			glVertex3f(x + width, y, 0.0F);
		
			//Move to next character
			x += width;
		}

		//Stop rendering quads
		glEnd();
	}

//Template function to output a scaled std::basic_string
	void DrawString (const std::string &text, float scalarx, float scalary, float x, float y)
	{
//		cout<<text<<endl;cout<<text.size()<<endl;

		unsigned int i;
		char c;
		GLFontChar *glfont_char=0;
		float width, height;
		
		//Begin rendering quads
		glBegin(GL_QUADS);
		
		//Loop through characters
		for (i = 0; i < text.size(); i++)
		{
			//Make sure character is in range
			c = text[i];
		
			if (c < header.start_char || c > header.end_char)
				continue;

		//	cout<<c<<" "<<int(c)<<" "<<start_char<<endl;
			//Get pointer to glFont character
			glfont_char = &header.chars[c - header.start_char];

			//Get width and height
			width = (glfont_char->dx * header.tex_width) * scalarx;
			height = (glfont_char->dy * header.tex_height) * scalary;
			
			//Specify vertices and texture coordinates
			glTexCoord2f(glfont_char->tx1, glfont_char->ty1);
			glVertex3f(x, y, 0.0F);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty2);
			glVertex3f(x, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty2);
			glVertex3f(x + width, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty1);
			glVertex3f(x + width, y, 0.0F);
		
			//Move to next character
			x += width;
		}

		//Stop rendering quads
		glEnd();
	}

	//Template function to output a colored character array
	template<class T> void DrawString (const T *text, float x,
		float y, const float *top_color,
		const float *bottom_color)
	{
		const T *i;
		GLFontChar *glfont_char;
		float width, height;
		
		//Begin rendering quads
		glBegin(GL_QUADS);
		
		//Loop through characters
		for (i = text; *i != '\0'; i++)
		{
			//Make sure character is in range
			if (*i < header.start_char || *i > header.end_char)
				continue;

			//Get pointer to glFont character
			glfont_char = &header.chars[*i - header.start_char];

			//Get width and height
			width = glfont_char->dx * header.tex_width;
			height = glfont_char->dy * header.tex_height;
			
			//Specify colors, vertices, and texture coordinates
			glColor3fv(top_color);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty1);
			glVertex3f(x, y, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty1);
			glVertex3f(x + width, y, 0.0F);
			glColor3fv(bottom_color);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty2);
			glVertex3f(x + width, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty2);
			glVertex3f(x, y - height, 0.0F);		
		
			//Move to next character
			x += width;
		}

		//Stop rendering quads
		glEnd();
	}

	//Template function to output a colored std::basic_string
	template<class T> void DrawString (
		const std::basic_string<T> &text, float x, float y,
		const float *top_color, const float *bottom_color)
	{
		unsigned int i;
		T c;
		GLFontChar *glfont_char;
		float width, height;
		
		//Begin rendering quads
		glBegin(GL_QUADS);
		
		//Loop through characters
		for (i = 0; i < text.size(); i++)
		{
			//Make sure character is in range
			c = text[i];
			if (c < header.start_char || c > header.end_char)
				continue;

			//Get pointer to glFont character
			glfont_char = &header.chars[c - header.start_char];

			//Get width and height
			width = glfont_char->dx * header.tex_width;
			height = glfont_char->dy * header.tex_height;
			
			//Specify colors, vertices, and texture coordinates
			glColor3fv(top_color);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty1);
			glVertex3f(x, y, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty1);
			glVertex3f(x + width, y, 0.0F);
			glColor3fv(bottom_color);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty2);
			glVertex3f(x + width, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty2);
			glVertex3f(x, y - height, 0.0F);		
		
			//Move to next character
			x += width;
		}

		//Stop rendering quads
		glEnd();
	}

	//Template function to output a scaled, colored character array
	template<class T> void DrawString (const T *text, float scalar,
		float x, float y, const float *top_color,
		const float *bottom_color)
	{
		const T *i;
		GLFontChar *glfont_char;
		float width, height;
		
		//Begin rendering quads
		glBegin(GL_QUADS);
		
		//Loop through characters
		for (i = text; *i != '\0'; i++)
		{
			//Make sure character is in range
			if (*i < header.start_char || *i > header.end_char)
				continue;

			//Get pointer to glFont character
			glfont_char = &header.chars[*i - header.start_char];

			//Get width and height
			width = (glfont_char->dx * header.tex_width) * scalar;
			height = (glfont_char->dy * header.tex_height) * scalar;
			
			//Specify colors, vertices, and texture coordinates
			glColor3fv(top_color);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty1);
			glVertex3f(x, y, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty1);
			glVertex3f(x + width, y, 0.0F);
			glColor3fv(bottom_color);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty2);
			glVertex3f(x + width, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty2);
			glVertex3f(x, y - height, 0.0F);		
		
			//Move to next character
			x += width;
		}

		//Stop rendering quads
		glEnd();
	}

	//Template function to output a scaled, colored std::basic_string
	template<class T> void DrawString (
		const std::basic_string<T> &text, float scalar, float x,
		float y, const float *top_color, const float *bottom_color)
	{
		unsigned int i;
		T c;
		GLFontChar *glfont_char;
		float width, height;
		
		//Begin rendering quads
		glBegin(GL_QUADS);
		
		//Loop through characters
		for (i = 0; i < text.size(); i++)
		{
			//Make sure character is in range
			c = text[i];
			if (c < header.start_char || c > header.end_char)
				continue;

			//Get pointer to glFont character
			glfont_char = &header.chars[c - header.start_char];

			//Get width and height
			width = (glfont_char->dx * header.tex_width) * scalar;
			height = (glfont_char->dy * header.tex_height) * scalar;
			
			//Specify colors, vertices, and texture coordinates
			glColor3fv(top_color);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty1);
			glVertex3f(x, y, 0.0F);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty1);
			glVertex3f(x + width, y, 0.0F);
			glColor3fv(bottom_color);
			glTexCoord2f(glfont_char->tx2, glfont_char->ty2);
			glVertex3f(x + width, y - height, 0.0F);
			glTexCoord2f(glfont_char->tx1, glfont_char->ty2);
			glVertex3f(x, y - height, 0.0F);		
		
			//Move to next character
			x += width;
		}

		//Stop rendering quads
		glEnd();
	}
};

//*******************************************************************


//wraperis glfont'am
class Font : public Resource{
public:
       Font():tex(0),font(){}

		//Resource impl
       bool             Load(string filename){
			glGenTextures(1, &tex);
			return font.Create(filename.c_str(), tex);                 
       }    
	   void	Unload(){
			if(glIsTexture(tex)){
				glDeleteTextures(1, &tex);
			}
	   }

		std::string		getExtenionStr(unsigned int n=0){return "glf";}
		unsigned int	getNumExt(){return 1;}

       void  Draw(string text, float size=1.0f, Vector2 pos = Vector2(0,0)){
		   glDisable(GL_FOG);
            glPushMatrix();
				glScalef(1,-1,1);
				glEnable(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        		glEnable(GL_BLEND);
        		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        		font.Begin();
        		font.DrawString(text, size, pos.x, pos.y);

        		glDisable(GL_BLEND);
        		glDisable(GL_TEXTURE_2D);
			glPopMatrix();
       }
       void  DrawCenter(string text, float size=1.0f, Vector2 pos = Vector2(0,0)){
		//   glDisable(GL_FOG);
            glPushMatrix();
			//	glScalef(size,-size,1);
				glEnable(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        		glEnable(GL_BLEND);
        		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				Vector2 sz = GetStringSize(text);
				sz *= size;
				//Vector2 pos = pos2;// + (pos2 * (1-size));

					font.Begin();
        			font.DrawString(text, size, -size, pos.x - sz.x/2, pos.y - sz.y/2);

        		glDisable(GL_BLEND);
        		glDisable(GL_TEXTURE_2D);
			glPopMatrix();
       }

	   Vector2 GetStringSize(string text, float scale = 1.0f){
			pair<int, int> result;
			font.GetStringSize(text.c_str(), &result);
			return Vector2((float)result.first * scale, (float)result.second  * scale);
	   }
//private:
       GLuint tex;
       glfont::GLFont font;  

	   static Font m_DefaultInstance;
};

//extern Font system_font;

typedef ResourceHandle<Font> FontHandle;

#endif

//End of file

