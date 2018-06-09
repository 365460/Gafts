#ifndef IMAGE_H
#define IMAGE_H

#include "header.h"
#include "camera.h"
#include "render.h"
#include "shader.h"
#include "loader.h"
#include "model.h"
#include "menu.h"
#include "func.hpp"
#include "image.h"

class Image {
public:
    Image(){}
    void load(const char *text, const char *vert, const char *frag);
    void show();
private:
    Texture *texture;
    Shader *shader;
    unsigned int VAO, VBO, EBO;
};

#endif
