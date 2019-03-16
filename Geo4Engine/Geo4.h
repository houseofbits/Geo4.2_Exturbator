/*
Geo4 engine

	2017/01/01	
		Refactoring core classes

*/
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <list>
#include <vector>

#include "math/HVector2.h"
#include "math/HVector3.h"
#include "math/HMatrix4.h"

#include "StringUtils.h"

#include "Serial.h"

#include "Renderer.h"

#include "CFODocument.h"

#include "ObjectFactory.h"
#include "ResourceManager.h"
#include "Entity.h"

#include "EventReceiver.h"

#include "events/WindowEvent.h"
#include "events/SceneManagerEvent.h"
#include "events/InputEvent.h"
#include "GUIEvent.h"

#include "objects/SceneManager.h"
#include "objects/Window.h"
#include "objects/Viewport.h"
//#include "objects/ViewportGLES2.h"

#include "GUIEntity.h"
#include "GUIViewport.h"
#include "GUIButton.h"
#include "GUISlider.h"
#include "GUISound.h"
#include "SoundDevice.h"

#include "Hardware.h"



#include "Font.h"
