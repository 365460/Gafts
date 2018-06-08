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
#include "func.hpp"

enum GameStatue{
	MENU,
	PLAYING
};

class Game {

public:
	Game();

	void game_loop();
	void init(GLFWwindow *win);

	GameStatue get_game_statue(){ return game_statue;  }

	void process_keyboard();
	void process_mouse(double xpos, double ypos);

private:

	Play *play;
	GLFWwindow* window;
	GameStatue game_statue;
};

#endif
