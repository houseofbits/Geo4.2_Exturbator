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

#include "../Lib/GLEW/include/GL/glew.h" 
#include <gl/GL.h>
#include <gl/GLU.h>

#include "math/HMath.h"
#include "math/HVector2.h"
#include "math/HVector3.h"
#include "math/HMatrix4.h"
#include "math/HColor.h"

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
#include "GUIInputEvent.h"

#include "objects/SceneManager.h"
#include "objects/Window.h"
#include "objects/Viewport.h"
//#include "objects/ViewportGLES2.h"

#include "GUIRenderable.h"
#include "GUIEntity.h"
#include "GUIViewport.h"
#include "GUIWindow.h"
#include "GUIButton.h"
#include "GUIText.h"
#include "GUISlider.h"

#include "Hardware.h"

//#include "Font.h"
//#include "TTFont.h"
#include "TrueTypeFont.h"
#include "GUIStyleResource.h"