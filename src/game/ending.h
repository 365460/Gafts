#ifndef ENDING_H
#define ENDING_H

#include "header.h"
#include "image.h"
#include "func.hpp"

typedef enum {LOST_IN_THE_UNIVERSE, BURNT_INTO_ASHES} DEATH_TYPE;

class Ending {
public:
    Ending(GLFWwindow *window);
    void game_over(DEATH_TYPE death);
    void process_keyboard_change(int key, int scancode, int action, int mods);
    void draw();
    bool next();
private:
    GLFWwindow *window;
    static const int STEP = 5, CHANGE_RATE = 70;
    int counter, mode;
    bool end_of_ending;
    Image *image[STEP << 1];
};

#endif
