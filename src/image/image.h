#ifndef IMAGE_H
#define IMAGE_H

#include "header.h"
#include "shader.h"
#include "loader.h"
#include "func.hpp"
#include "image.h"

class Image {
public:
    Image(){}
    void load(const char *texture);
    void show();
private:
    Texture *texture;
    Shader *shader;
    unsigned int VAO, VBO, EBO;
};

#endif
