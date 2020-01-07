#pragma once
#ifndef __KEYBUFFER_H__
#define __KEYBUFFER_H__

class KeyBuffer {
	static KeyBuffer* instance;
	bool keyStates[256] = { false };

public:
	static KeyBuffer* getInstance();
	static void freeInstance();
	void pressKey(int key);
	bool isKeyPressed(int key);
	void releaseKey(int key);
};

#endif