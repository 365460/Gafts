#include "ending.h"

Ending::Ending(GLFWwindow *window) {
    this->window = window;
    this->end_of_ending = true;
    for (int i = 0; i < STEP + STEP; i++) {
        image[i] = new Image();
    }

    image[0 << 1]->load("./res/image/ending1_1.jpg");
    image[1 << 1]->load("./res/image/ending1_2.jpg");
    image[2 << 1]->load("./res/image/ending1_3.jpg");
    image[3 << 1]->load("./res/image/ending1_4.jpg");
    image[4 << 1]->load("./res/image/ending1_5.jpg");

    image[0 << 1 | 1]->load("./res/image/ending1_1.jpg");
    image[1 << 1 | 1]->load("./res/image/ending1_2.jpg");
    image[2 << 1 | 1]->load("./res/image/ending1_3.jpg");
    image[3 << 1 | 1]->load("./res/image/ending1_4.jpg");
    image[4 << 1 | 1]->load("./res/image/ending1_5.jpg");
}

bool Ending::next() {
    return end_of_ending;
}

void Ending::game_over(DEATH_TYPE death) {
    this->mode = death;
    this->end_of_ending = false;
    this->counter = 0;
}

void Ending::process_keyboard_change(int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    int state = counter / CHANGE_RATE;
    if (state >= STEP - 1) {
        end_of_ending = true;
    }
}

void Ending::draw() {
    int state = std::min(STEP - 1, counter / CHANGE_RATE);
    counter++;
    image[(state << 1) | mode]->show();
}
