#ifndef RAWMODEL_H
#define RAWMODEL_H

#include "header.h"
#include "shader.h"

/*  store basic information of models */

struct Texture{
	unsigned int id;
	string type;
	string path;
};

enum Draw_Method{
	DRAW_ELEMENT = 0,
	DRAW_ARRAY
};

class RawModel {

public:
    RawModel(vector<float>& vertices, vector<float>& normal,
        vector<float>& vec_texture, vector<unsigned int>& faces);

	RawModel() {
		for(auto& t: textures) {
			delete t;
		}
	}

	void set_draw_method(Draw_Method method);

    void add_texture(Texture* texture);
    void add_texture(const vector<Texture*>& texture);

    GLuint get_VAO();
    int get_vertice_num();
    int get_face_num();
    unsigned int get_texture_id();
	Draw_Method get_draw_method();

	void bind_texture(Shader* shader);
	void print();

    vector<float> vertices;
    vector<float> normal;
    vector<float> vec_texture;
    vector<unsigned int> faces;
	vector<Texture*> textures;

private:
    GLuint texture_id;
    GLuint VAO, VBO_ver, VBO_text, VBO_nor, EBO;
	Draw_Method draw_method;
};

#endif
