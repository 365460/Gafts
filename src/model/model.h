#ifndef MODEL_H
#define MODEL_H

#include "rawModel.h"
#include "shader.h"

class Model {
    vector<RawModel*> raw_models;
    glm::mat4 translation;          // model position in the world
    glm::vec3 position;

    // boundary object

public:
    Model(RawModel* raw_model, glm::mat4 translation=glm::mat4(1.0));
    Model(const vector<RawModel*>& _raw_models);
    void add_raw_model(RawModel *raw_model);
    void prepare_rendering(Shader* shader, size_t index);     // is going to render inder RawModel
    void move(glm::vec3 val);
	void print();

	void set_translation(glm::mat4 &trans);

    const vector<RawModel*>& get_raw_models();
    const glm::mat4& get_translation();
	glm::vec3 get_position();
	glm::vec3 get_center();
	glm::vec3 get_size();
};

#endif
