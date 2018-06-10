#include "model.h"


Model::Model(RawModel* raw_model, glm::mat4 trans):translation(trans) {
    add_raw_model( raw_model );
}

Model::Model(const vector<RawModel*>& _raw_models) {
	for(auto& raw: _raw_models) add_raw_model(raw);
	translation = glm::mat4(1.0);
}

void Model::add_raw_model(RawModel *raw_model) {
    raw_models.emplace_back(raw_model);
}

void Model::prepare_rendering(Shader* shader, size_t index) {

    if(index > raw_models.size()) {
        // log something , QQ no logging now
        return;
    }
    RawModel* raw_model = raw_models[index];
    glBindVertexArray(raw_model->get_VAO());
	raw_model->bind_texture(shader);
}

void Model::move(glm::vec3 val) {
    translation = glm::translate(translation, val);
}

const vector<RawModel*>& Model::get_raw_models() {
    return raw_models;
}

const glm::mat4& Model::get_translation() {
    return translation;
}

void Model::set_translation(glm::mat4 &trans) {
	translation = trans;
}

void Model::print() {
	for(auto &x : raw_models) {
		x->print();
	}
}

glm::vec3 Model::get_position() {
    return get_center();
}

glm::vec3 Model::get_center() {
	float min_val[3] = { 100000000, 100000000, 100000000 };
	float max_val[3] = { -100000000, -100000000, -100000000};
	// float MAX = std::numeric_limits<float>::max();
    // float MIN = std::numeric_limits<float>::min();
	// float min_val[3] = { MAX, MAX, MAX};
	// float max_val[3] = { MIN, MIN, MIN};
	for(auto&raw : this->raw_models) {
		for(int i=0; i<raw->vertices.size(); i+=3) {
			glm::vec4 pos(raw->vertices[i], raw->vertices[i+1], raw->vertices[i+2], 1.0);
			pos = translation * pos;
			min_val[0] = std::min(min_val[0], pos.x);
			min_val[1] = std::min(min_val[1], pos.y);
			min_val[2] = std::min(min_val[2], pos.z);

			max_val[0] = std::max(max_val[0], pos.x);
			max_val[1] = std::max(max_val[1], pos.y);
			max_val[2] = std::max(max_val[2], pos.z);
		}
	}

    // cout << MAX << " " << MIN << endl;
    // cout << min_val[0] << " " << min_val[1] << " " << min_val[2] << endl;
    // cout << max_val[0] << " " << max_val[1] << " " << max_val[2] << endl << endl;
	glm::vec3 center;
	center.x = (min_val[0] + max_val[0])/2;
	center.y = (min_val[1] + max_val[1])/2;
	center.z = (min_val[2] + max_val[2])/2;

	// cout << "max: "; for(int i=0; i<3; i++) cout << max_val[i] << ", "; cout << endl;
	// cout << "min: "; for(int i=0; i<3; i++) cout << min_val[i] << ", "; cout << endl;

	return center;
}


glm::vec3 Model::get_size() {
	float min_val[3] = { 100000000, 100000000, 100000000 };
	float max_val[3] = { -100000000, -100000000, -100000000};
	// float MAX = std::numeric_limits<float>::max();
    // float MIN = std::numeric_limits<float>::min();
	// float min_val[3] = { MAX, MAX, MAX};
	// float max_val[3] = { MIN, MIN, MIN};
	for(auto&raw : this->raw_models) {
		for(int i=0; i<raw->vertices.size(); i+=3) {
			glm::vec4 pos(raw->vertices[i], raw->vertices[i+1], raw->vertices[i+2], 1.0);
			pos = translation * pos;
			min_val[0] = std::min(min_val[0], pos.x);
			min_val[1] = std::min(min_val[1], pos.y);
			min_val[2] = std::min(min_val[2], pos.z);

			max_val[0] = std::max(max_val[0], pos.x);
			max_val[1] = std::max(max_val[1], pos.y);
			max_val[2] = std::max(max_val[2], pos.z);
		}
	}

	glm::vec3 size;
	size.x = max_val[0] - min_val[0];
	size.y = max_val[1] - min_val[1];
	size.z = max_val[2] - min_val[2];

	return size;
}
