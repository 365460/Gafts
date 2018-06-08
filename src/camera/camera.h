#ifndef CAMERA_H
#define CAMERA_H

#include "header.h"

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up_direction;
    float yaw, pitch, roll;

    Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up);

    glm::mat4 get_view_matrix();

    void add_yaw(float val);
    void add_pitch(float val);

    void update_vector();
};

#endif
