#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "glew/glew.h"
#include <GLFW/glfw3.h>
#include "Display.h"
#include "Player.h"

class InputHandler
{
public:
	InputHandler();
	virtual ~InputHandler();

	static void Key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void MouseControls(Display *display, Player *player, float elapsedTime);
	void KeyboardControls(Display *display, Player *player, float elapsedTime);
private:
	static bool mouseLock;
};

#endif