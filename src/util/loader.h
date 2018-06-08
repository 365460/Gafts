#ifndef LOADER_H
#define LOADER_H

#include "header.h"
#include "model.h"

class Loader {
public:
    static Texture* load_texture(const char* filename, string type="texture_diffuse");
	static Model* load_model(const char* filename);

	static Model* load_box(float, float, float, bool sky_box=false);

};

#endif
