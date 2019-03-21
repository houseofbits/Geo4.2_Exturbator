#include "Geo4.h"

FT_Library library_;

TrueTypeFontFace::TrueTypeFontFace()
{

}

TrueTypeFontFace::~TrueTypeFontFace()
{
}

bool TrueTypeFontFace::Load(std::string filename, unsigned int size) {

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

	//FT_UInt index = FT_Get_Char_Index(ftFace, 'a');

	FT_ULong  charcode;
	FT_UInt   gindex;

	charcode = FT_Get_First_Char(ftFace, &gindex);
	unsigned int char_count = 0;
	while (gindex != 0){
		char_count++;
		//cout << charcode << " / " << gindex << endl;
		
		charcode = FT_Get_Next_Char(ftFace, charcode, &gindex);
	}
	cout << "char count " << char_count << endl;



	FT_Done_FreeType(library_);

	return true;
}