
#include "game.h"


Game::Game() {}

void Game::init(GLFWwindow *win){ window = win;
	game_statue = MENU;
	play = new Play(win);
	menu = new Menu(win);
}

void Game::game_loop() {

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_keyboard();

		switch(game_statue) {
			case MENU:
				menu->draw();
				if (menu->next()) {
					game_statue = PLAYING;
					play->init();
				}
				break;
			case PLAYING:
				play->draw();
				if(play->is_lose()) {
					game_statue = MENU;
					menu->reset();
				}
				break;
			default:
				break;
		}

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void Game::process_keyboard() {

	switch(game_statue) {
		case PLAYING:
			play->process_keyboard();
			break;
		default:
			break;
	}
}

void Game::process_keyboard_change(int key, int scancode, int action, int mods) {
	switch(game_statue) {
		case MENU:
			menu->process_keyboard_change(key, scancode, action, mods);
			break;
		default:
			break;
	}
}

void Game::process_mouse(double xpos, double ypos) {

	switch(game_statue) {
		case PLAYING:
			play->process_mouse(xpos, ypos);
			break;
		default:
			break;
	}
}
