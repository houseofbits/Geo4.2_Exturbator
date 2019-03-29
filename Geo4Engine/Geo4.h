/*
Geo4 engine

	2017/01/01	
		Refactoring core classes
	2019/03/20
		Creating GUI classes

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
#include "gui/GUIEvent.h"
#include "gui/GUIInputEvent.h"

#include "objects/SceneManager.h"
#include "objects/Window.h"
#include "objects/Viewport.h"

#include "ExturbatorInterface.h"

#include "gui/GUIRenderable.h"
#include "gui/GUIEntity.h"
#include "gui/GUIViewport.h"
#include "gui/GUIWindow.h"
#include "gui/GUIButton.h"
#include "gui/GUIText.h"
#include "gui/GUIList.h"
#include "gui/GUIListItem.h"
#include "gui/GUILayer.h"
#include "gui/GUISlider.h"

#include "Hardware.h"
#include "ExturbatorHardware.h"
#include "DataPacketReceiver.h"

#include "TrueTypeFont.h"
#include "gui/GUIStyleResource.h"