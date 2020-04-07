#ifndef INPUT_H

// mouse
struct MouseEvent {
	float x{ 0.0f }, y{ 0.0f };	// coordinates
};

// keyboard
struct KeyEvent {
	char key{ 0 };		// which key
	bool ctrl{ false }; // modifiers
};

#define INPUT_H
#endif