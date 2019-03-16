#include "Geo4.h"

using namespace std;
/*
FMOD_RESULT F_CALLBACK fmod_out_getnumdrivers(
	FMOD_OUTPUT_STATE *output_state,
	int *numdrivers
) {
	return FMOD_OK;	
}

FMOD_RESULT F_CALLBACK fmod_out_getdriverinfo(
	FMOD_OUTPUT_STATE *output_state,
	int id,
	char *name,
	int namelen,
	FMOD_GUID *guid,
	int *systemrate,
	FMOD_SPEAKERMODE *speakermode,
	int *speakermodechannels
) {		
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK fmod_out_init(
	FMOD_OUTPUT_STATE *output_state, int selecteddriver, FMOD_INITFLAGS flags,
	int *outputrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels,
	FMOD_SOUND_FORMAT *outputformat, int dspbufferlength, int dspnumbuffers,
	void *extradriverdata
) {	
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK fmod_out_start(
	FMOD_OUTPUT_STATE *output_state
) {		
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK fmod_out_stop(
	FMOD_OUTPUT_STATE *output_state
) {		
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK fmod_out_close(
	FMOD_OUTPUT_STATE *output_state
) {	
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK fmod_out_update(
	FMOD_OUTPUT_STATE *output_state
) {	
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK fmod_out_gethandle(
	FMOD_OUTPUT_STATE *output_state,
	void **handle
) {	
	return FMOD_OK;
}

FMOD_RESULT F_CALLBACK fmod_out_mixer(
	FMOD_OUTPUT_STATE *output_state
) {	
	return FMOD_OK;
}

void createCustomOutput(FMOD_OUTPUT_DESCRIPTION& outputDesc) {

	//Callback: if the audio hardware provides a callback where you must provide a buffer of samples then set 'polling' to FALSE 
	//and directly call FMOD_OUTPUT_READFROMMIXER.
	
	//Synchronization : if the audio hardware provides a synchronization primitive to wait on then set 'polling' to FALSE 
	//and give a FMOD_OUTPUT_MIXER_CALLBACK pointer.FMOD will create a mixer thread and call you repeatedly once FMOD_OUTPUT_START_CALLBACK has finished, 
	//you must wait on your primitive in this callback and upon wake call FMOD_OUTPUT_READFROMMIXER.

	outputDesc.apiversion = FMOD_OUTPUT_PLUGIN_VERSION;
	outputDesc.name = "Custom output";
	outputDesc.version = 1;
	outputDesc.polling = false;
	outputDesc.getnumdrivers = &fmod_out_getnumdrivers;
	outputDesc.getdriverinfo = &fmod_out_getdriverinfo;
	outputDesc.init = &fmod_out_init;
	outputDesc.start = &fmod_out_start;
	outputDesc.stop = &fmod_out_stop;
	outputDesc.close = &fmod_out_close;
	outputDesc.update = &fmod_out_update;
	outputDesc.gethandle = &fmod_out_gethandle;
	outputDesc.mixer = &fmod_out_mixer;
}


void createTestPlugin() {

	FMOD_OUTPUT_DESCRIPTION customOutput;
	createCustomOutput(customOutput);

	FMOD::System     *system;
	FMOD::Sound      *sound1;
	FMOD::Channel    *channel = 0;
	void             *extradriverdata = 0;

	FMOD::System_Create(&system);

	system->init(32, FMOD_INIT_NORMAL, extradriverdata);

	system->createSound("sounds/voice_1.wav", FMOD_DEFAULT, 0, &sound1);

	system->playSound(sound1, 0, false, &channel);

}

*/


int main(int argc, char* argv[])
{	
#ifndef _DEBUG
	ofstream file;
	file.open("logfile.log");
	streambuf* sbuf = cout.rdbuf();
	cout.rdbuf(file.rdbuf());
#endif

	cout << RED << "GEO4 Engine" << endl;
	cout << "Copyright: Krists Pudzens (c) 2007 - 2018" << endl;
	cout << GREEN << "========================================" << WHITE << endl;

	SceneManager manager;

	manager.resource_manager.RegisterResourceType(Font());

	manager.RegisterObjectType(SDLWindow());
	manager.RegisterObjectType(Viewport());
	//manager.RegisterObjectType(ViewportGLES2());
	manager.RegisterObjectType(GUIViewport());
	manager.RegisterObjectType(GUISlider());
	manager.RegisterObjectType(GUISound());
	manager.RegisterObjectType(Hardware());
	manager.RegisterObjectType(SoundDevice());

	manager.LoadCFO("conf.cfo");

	while (manager.ProcessEvents()) {
		manager.DrawRecursive(&manager);
	}

	return 0;
}