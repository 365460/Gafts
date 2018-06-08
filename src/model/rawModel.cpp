#include "rawModel.h"

RawModel::RawModel(vector<float>& vertices, vector<float>& normal,
        vector<float>& vec_texture, vector<unsigned int>& faces)
:vertices(vertices), normal(normal), vec_texture(vec_texture), faces(faces), draw_method(DRAW_ELEMENT)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO_ver);
    glGenBuffers(1, &VBO_text);
    glGenBuffers(1, &VBO_nor);

    glBindVertexArray(VAO);

    /* bind vertices */
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ver);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	/* bind faces */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*faces.size(), &faces[0], GL_STATIC_DRAW);

	/* bind texture */
	glBindBuffer(GL_ARRAY_BUFFER, VBO_text);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vec_texture.size(), &vec_texture[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	/* bind normal */
	glBindBuffer(GL_ARRAY_BUFFER, VBO_nor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normal.size(), &normal[0], GL_STATIC_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

    glBindVertexArray(0);

}

void RawModel::add_texture(Texture* texture) {
	this->textures.push_back(texture);
}

void RawModel::add_texture(const vector<Texture*>& texture) {
	for(const auto& x: texture) {
		this->textures.push_back(x);
	}
}

GLuint RawModel::get_VAO() {
    return VAO;
}

int RawModel::get_vertice_num() {
    return vertices.size();
}

int RawModel::get_face_num() {
    return faces.size();
}

unsigned int RawModel::get_texture_id() {
    return texture_id;
}

void RawModel::bind_texture(Shader* shader) {

	// bind appropriate textures
	unsigned int diffuseNr  = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr   = 1;
	unsigned int heightNr   = 1;
	for(unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i]->type;
		if(name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if(name == "texture_specular")
			number = std::to_string(specularNr++); // transfer unsigned int to stream
		else if(name == "texture_normal")
			number = std::to_string(normalNr++); // transfer unsigned int to stream
		else if(name == "texture_height")
			number = std::to_string(heightNr++); // transfer unsigned int to stream

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader->program_id, (name + number).c_str()), i);
		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, textures[i]->id);
	}
}

void RawModel::print() {
	cout << "num vertices: " << vertices.size() << endl;
	cout << "num textures: " << textures.size() << endl;
	for(auto& x: textures) {
		cout << x->path << ": " << x->type << endl;
	}
	cout << endl;
}

Draw_Method RawModel::get_draw_method() {
	return draw_method;
}

void RawModel::set_draw_method(Draw_Method method) {
	draw_method = method;
}

