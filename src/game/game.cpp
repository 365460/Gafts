
#include "game.h"


Game::Game() {}

void Game::init(GLFWwindow *win){ window = win; 
	game_statue = PLAYING;
	play = new Play(win);
}

void Game::game_loop() {

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_keyboard();

		switch(game_statue) {
			case PLAYING:
				play->draw();
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
		default:
			break;
	}
}

void Game::process_mouse(double xpos, double ypos) {

	switch(game_statue) {
		case PLAYING:
			play->process_mouse(xpos, ypos);
		default:
			break;
	}
}
