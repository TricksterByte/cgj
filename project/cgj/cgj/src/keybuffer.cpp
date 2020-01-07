#include "keybuffer.h"

KeyBuffer* KeyBuffer::instance = nullptr;

KeyBuffer* KeyBuffer::getInstance() {
	if (instance == nullptr) {
		instance = new KeyBuffer();
	}
	return instance;
}

void KeyBuffer::freeInstance() {
	delete instance;
	instance = nullptr;
}

void KeyBuffer::pressKey(int key) {
	keyStates[key] = true;
}

bool KeyBuffer::isKeyPressed(int key) {
	return keyStates[key];
}

void KeyBuffer::releaseKey(int key) {
	keyStates[key] = false;
}