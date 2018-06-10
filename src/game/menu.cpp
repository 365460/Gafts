#include "menu.h"

Menu::Menu(GLFWwindow *window) {
    /* Initialize members */
    this->window = window;
    this->title[0] = new Image();
    this->title[1] = new Image();
    this->reset();

    /* Initialize image */
    this->title[0]->load("./res/image/title1.jpg", "./shader/menu.vert", "./shader/menu.frag");
    this->title[1]->load("./res/image/title2.jpg", "./shader/menu.vert", "./shader/menu.frag");
}

void Menu::reset() {
    start_playing = false;
    counter = 0;
    current_render = 0;
}

bool Menu::next() {
    return start_playing;
}

void Menu::process_keyboard_change(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
        start_playing = true;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Menu::draw() {
    /* Alternating */
    counter++;
    if (counter > change_rate + current_render * stay_time) {
        current_render ^= 1;
        counter = 0;
    }

    /* Rendering */
    title[current_render]->show();
}
