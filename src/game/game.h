#ifndef GAME_H
#define GAME_H

#include "header.h"
#include "camera.h"
#include "loader.h"
#include "shader.h"
#include "render.h"
#include "rawModel.h"
#include "model.h"
#include "play.h"
#include "menu.h"
#include "ending.h"
#include "func.hpp"

enum GameStatue{
	MENU,
	PLAYING,
	ENDING
};

class Game {

public:
	Game();

	void game_loop();
	void init(GLFWwindow *win);

	GameStatue get_game_statue(){ return game_statue;  }

	void process_keyboard();
	void process_keyboard_change(int key, int scancode, int action, int mods);
	void process_mouse(double xpos, double ypos);

private:

	Play *play;
	Menu *menu;
	Ending *ending;
	GLFWwindow* window;
	GameStatue game_statue;
};

#endif
