#include "Geo4.h"

using namespace std;

int main(int argc, char* argv[])
{	
	/*
	struct PacketType {
		unsigned int someValue;
		float someFloatValue;
		int someNegativeValue;
	};

	struct SimplePacket {
		unsigned char c1;
		unsigned char c2;
	};

	return 1;
	*/

#ifndef _DEBUG
	ofstream file;
	file.open("logfile.log");
	streambuf* sbuf = cout.rdbuf();
	cout.rdbuf(file.rdbuf());
#endif

	cout << RED << "GEO4.2 Engine - Exturbator Prototype" << endl;
	cout << "Copyright: Krists Pudzens (c) 2007 - 2019" << endl;
	cout << GREEN << "========================================" << WHITE << endl;

	SceneManager manager;

	manager.resource_manager.RegisterResourceType(TrueTypeFontFace());
	manager.resource_manager.RegisterResourceType(GUIStyleResource());

	manager.RegisterObjectType(SDLWindow());
	manager.RegisterObjectType(Viewport());
	manager.RegisterObjectType(GUIViewport());
	manager.RegisterObjectType(GUISlider());
	manager.RegisterObjectType(GUIWindow());
	manager.RegisterObjectType(GUIButton());
	manager.RegisterObjectType(GUIText());
	manager.RegisterObjectType(GUIList());
	manager.RegisterObjectType(GUILayer());
	manager.RegisterObjectType(GUIListItem());
	manager.RegisterObjectType(Hardware());

	manager.LoadCFO("conf.cfo");

	while (manager.ProcessEvents()) {
		manager.DrawRecursive(&manager);
	}

	return 0;
}