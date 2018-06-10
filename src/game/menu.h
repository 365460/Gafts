#ifndef MENU_H
#define MENU_H

#include "header.h"
#include "image.h"
#include "func.hpp"

class Menu {
public:
    Menu(GLFWwindow *window);
    bool next();
    void draw();
    void process_keyboard_change(int key, int scancode, int action, int mods);
    void reset();

private:
    const int change_rate = 20, stay_time = 15;
    int counter;
    bool start_playing, current_render;
    Image *title[2];
    GLFWwindow *window;
};

#endif
