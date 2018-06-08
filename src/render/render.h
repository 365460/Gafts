#ifndef RENDER_H
#define RENDER_H

#include "header.h"
#include "shader.h"
#include "model.h"

/* in the same render class , the models have the same
 1. projection
 2. way to render( drawArrays or drawElement or ...), may be we can bind
    this information to every model
*/
class Render {

    vector<pair<Shader*, Model*>> models;
    glm::mat4 projection;

public:
    Render(glm::mat4 projection);
    void add_models(Shader* shader, Model* model);
    void render(glm::mat4 camera_view);

};
#endif
