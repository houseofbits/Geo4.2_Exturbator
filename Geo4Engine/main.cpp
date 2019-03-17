#include "Geo4.h"

using namespace std;

int main(int argc, char* argv[])
{	
/*
	Triangulation triang;

	vector<Vector2> points;
	points.push_back(Vector2(-1, 1));
	points.push_back(Vector2(-1, -1));
	points.push_back(Vector2(1, -1));
	points.push_back(Vector2(1, 1));

	vector<unsigned int> indicesOut;

	triang.EarClipTrinagulate(points, indicesOut);

	return 1;
*/

#ifndef _DEBUG
	ofstream file;
	file.open("logfile.log");
	streambuf* sbuf = cout.rdbuf();
	cout.rdbuf(file.rdbuf());
#endif

	cout << RED << "GEO4.4 Engine - Exturbator Prototype" << endl;
	cout << "Copyright: Krists Pudzens (c) 2007 - 2019" << endl;
	cout << GREEN << "========================================" << WHITE << endl;

	SceneManager manager;

	manager.resource_manager.RegisterResourceType(Font());

	manager.RegisterObjectType(SDLWindow());
	manager.RegisterObjectType(Viewport());
	//manager.RegisterObjectType(ViewportGLES2());
	manager.RegisterObjectType(GUIViewport());
	manager.RegisterObjectType(GUISlider());
	manager.RegisterObjectType(GUIButton());
	manager.RegisterObjectType(Hardware());

	manager.LoadCFO("conf.cfo");

	while (manager.ProcessEvents()) {
		manager.DrawRecursive(&manager);
	}

	return 0;
}