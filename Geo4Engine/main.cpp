#include "Geo4.h"
#include <fstream>
#include <chrono>
#include <ctime>
#include <locale>

using namespace std;

int main(int argc, char* argv[])
{	

#ifndef _DEBUG
	ofstream file;
	file.open("logfile.log");
	streambuf* sbuf = cout.rdbuf();
	cout.rdbuf(file.rdbuf());
#endif

	chrono::system_clock::time_point p = chrono::system_clock::now();
	time_t t = chrono::system_clock::to_time_t(p);
	char str[26];
	ctime_s(str, sizeof str, &t);

	cout << RED << "GEO4.2 Engine - Exturbator Prototype" << endl;
	cout << "Copyright: Krists Pudzens (c) 2007 - 2019" << endl;
	cout << RED << str << WHITE;
	cout << GREEN << "========================================" << WHITE << endl;

	SceneManager manager("conf.cfo");

	while (manager.process()) {	}

	return 0;
}