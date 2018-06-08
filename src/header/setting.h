#ifndef SETTING_H
#define SETTING_H

// program size
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// object
const float SKY_LEN = 500;
const float SUN_RADIUS_SCALE = 20;
const glm::vec3 PLATFORM_SCALE = glm::vec3(10, 4, 10);

// object speed
const float SUN_PLUNG_PRE_SECOND = 0.5;
const float PLAYER_PLUNG_PRE_SECOND = 10;

// keyboard camera
const float CAMERA_INIT = 2.5;
const float MOUSE_SENSITIVITY = 0.1;

// tremor
const float TREMOR_INIT= 0.002;
const float TREMOR_MAX_HE = 0.03;
const float TREMOR_TIME = 0.3;

#endif
