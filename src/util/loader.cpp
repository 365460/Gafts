#include "loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture* Loader::load_texture(const char* filename, string type) {

	cout << "loading " << filename << endl;
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if(!data) {
		cout << "[x] fail open " << filename << "\n";
	}

	GLenum format;
	if(nrChannels==1) format = GL_RED;
	else if(nrChannels == 3) format = GL_RGB;
	else if(nrChannels == 4) format = GL_RGBA;

	unsigned int texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	cout << "width: " << width << ", height: " << height << endl;
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);

	Texture* texture = new Texture();
	texture->id = texture_id;
	texture->type = type;
	texture->path = string(filename);

	return texture;
}

Model* Loader::load_model(const char* filename) {
	vector<RawModel*> raw_models;
	vector<Texture*> textures_loaded;
	string dir;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, ::aiProcess_Triangulate|::aiProcess_FlipUVs|::aiProcess_CalcTangentSpace);

	string path(filename);
	dir = path.substr(0,  path.find_last_of('/') );

	if(!scene || !scene->mRootNode) {
		printf("[x] fail load obj: %s\n", filename);
	}

	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	auto loadMaterialTextures = [&](aiMaterial *mat, aiTextureType type, string typeName)
	{
		vector<Texture*> textures;
		for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for(unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if(std::strcmp(textures_loaded[j]->path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if(!skip)
			{   // if texture hasn't been loaded already, load it
				Texture* texture =Loader::load_texture( (dir + "/" + string(str.C_Str())).c_str() , typeName);
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	};

	auto process_mesh = [&](aiMesh* mesh) {

		vector<float> vertices, normals, vec_textures;
		vector<unsigned int> faces;
		vector<Texture*> textures;

		for(int i=0; i<mesh->mNumVertices; i++) {
			vertices.push_back(mesh->mVertices[i].x);
			vertices.push_back(mesh->mVertices[i].y);
			vertices.push_back(mesh->mVertices[i].z);

			normals.push_back(mesh->mNormals[i].x);
			normals.push_back(mesh->mNormals[i].y);
			normals.push_back(mesh->mNormals[i].z);

			if(mesh->mTextureCoords[0]) {
				vec_textures.push_back(mesh->mTextureCoords[0][i].x);
				vec_textures.push_back(mesh->mTextureCoords[0][i].y);
			}
			else vec_textures.push_back(0), vec_textures.push_back(0);
		}

		for(int i=0; i<mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for(int j=0; j<face.mNumIndices; j++)
				faces.push_back(face.mIndices[j]);
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// 1. diffuse maps
		vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		vector<Texture*> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		vector<Texture*> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		RawModel *raw_model = new RawModel(vertices, normals, vec_textures, faces);
		raw_model->add_texture( textures );
		return raw_model;
	};

	std::function<void(aiNode*)> process_node = [&](aiNode* node) -> void {

		for(int i=0; i<node->mNumMeshes; i++) {
			raw_models.push_back( process_mesh(scene->mMeshes[node->mMeshes[i]]) );
		}
		for(int i=0; i<node->mNumChildren; i++) {
			process_node(node->mChildren[i]);
		}
	};

	process_node(scene->mRootNode);

	return new Model(raw_models);
}

Model* Loader::load_box(float l,float w,float h, bool sky_box) {

	vector<float> vertices {
		-l, -h, -w,
		l,  -h, -w,
		l,   h, -w,
		l,   h, -w,
		-l,  h, -w,
		-l, -h, -w,

		-l, -h,  w,
		l,  -h,  w,
		l,   h,  w,
		l,   h,  w,
		-l,  h,  w,
		-l, -h,  w,

		-l,  h,  w,
		-l,  h, -w,
		-l, -h, -w,
		-l, -h, -w,
		-l, -h,  w,
		-l,  h,  w,

		l,  h,  w,
		l,  h, -w,
		l, -h, -w,
		l, -h, -w,
		l, -h,  w,
		l,  h,  w,

		-l, -h, -w,
		l,  -h, -w,
		l,  -h,  w,
		l,  -h,  w,
		-l, -h,  w,
		-l, -h, -w,

		-l,  h, -w,
		l,   h, -w,
		l,   h,  w,
		l,   h,  w,
		-l,  h,  w,
		-l,  h, -w
	};


	vector<float> vec_texture{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	vector<float> normal = {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	vector<unsigned int> faces;
	RawModel* raw_model = new RawModel(vertices, normal, vec_texture, faces);
	if( sky_box ) raw_model->add_texture( Loader::load_texture("./res/image/sky.jpg") );
	else raw_model->add_texture( Loader::load_texture("./res/image/container.jpg") );

	raw_model->set_draw_method(DRAW_ARRAY);

	return new Model(raw_model);
}
