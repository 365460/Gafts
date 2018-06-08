#include "camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up):
    position(pos), front(front),up_direction(up) {

    yaw = -90;
    pitch = 0;
    roll = 0;
}

glm::mat4 Camera::get_view_matrix() {
    return glm::lookAt(position, position + front, up_direction);
}

void Camera::add_yaw(float val) {
    yaw += val;
}

void Camera::add_pitch(float val) {
    pitch += val;
    if(pitch>=89) pitch = 89;
    if(pitch<=-89) pitch = -89;
}

void Camera::update_vector() {

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(front);

    /*  use right vector to update up vector*/
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0 , 1 , 0)));
    up_direction = glm::normalize(glm::cross(right, front));
}
