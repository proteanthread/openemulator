
/**
 * OpenEmulator
 * OpenEmulator OpenGL canvas
 * (C) 2010 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Implements an OpenEmulator OpenGL canvas.
 */

#ifndef _OEOPENGLCANVAS_H
#define _OEOPENGLCANVAS_H

#include <pthread.h>

#include <OpenGL/gl.h>

#include "OEComponent.h"
#include "HostCanvasInterface.h"

typedef enum
{
	OEGL_TEX_POWER,
	OEGL_TEX_PAUSE,
	OEGL_TEX_CAPTURE,
	OEGL_TEX_FRAME,
	OEGL_TEX_INTERLACE,
	OEGL_TEX_NUM,
} OEOpenGLTextureIndex;

class OEOpenGLCanvas : public OEComponent
{
public:
	OEOpenGLCanvas();
	~OEOpenGLCanvas();
	
	void initOpenGL();
	void draw(int width, int height);
	
	void setSystemKey(int usageId);
	void setKey(int usageId, bool value);
	void setUnicodeKey(int unicode);
	
	void setMouseButton(int index, bool value);
	void setMousePosition(float x, float y);
	void moveMouse(float rx, float ry);
	void sendMouseWheelEvent(int index, float value);
	
	void setJoystickButton(int deviceIndex, int index, bool value);
	void setJoystickPosition(int deviceIndex, int index, float value);
	void sendJoystickHatEvent(int deviceIndex, int index, float value);
	void moveJoystickBall(int deviceIndex, int index, float value);
	
	void resetKeysAndButtons();
	
private:
	GLuint textures[OEGL_TEX_NUM];
	
	pthread_mutex_t frameMutex;
	
	bool keyDown[HOST_CANVAS_KEYBOARD_KEY_NUM];
	int keyDownCount;
	bool mouseButtonDown[HOST_CANVAS_MOUSE_BUTTON_NUM];
	bool joystickButtonDown[HOST_CANVAS_JOYSTICK_NUM][HOST_CANVAS_JOYSTICK_BUTTON_NUM];
	
	bool mouseCaptured;
	bool mouseCaptureRelease;
	
	void postHIDNotification(int notification, int usageId, float value);
};

#endif