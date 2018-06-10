#ifndef PLAY_H

#define PLAY_H
#include "header.h"
#include "camera.h"
#include "render.h"
#include "shader.h"
#include "loader.h"
#include "model.h"
#include "func.hpp"

class Sun;
class SkyBox;
class Box;

class Play {

public:
	Play(GLFWwindow *win);
	~Play() {}


	void draw();
	void process_keyboard();
	void process_mouse(double xpos, double ypos);
	void init();
	bool is_lose();

private:
	void load_object();
	void update_object();
	void update_v();
	void update_scene();
	bool check_inside(glm::vec3 pos);
	bool check_attach();
	void set_warning();


	vector<Box*> platform;
	Sun* sun;
	SkyBox* sky_box;

	bool is_warning;
	bool attach;
	float fps;
	float v_sun;
	float v_play;

	Camera *camera;
	Render *render;
	Shader *main_shader, *sky_shader, *light_shader;

	int landing_id;
	float last_landing;
	bool is_tremor;
	bool is_flying;
	float start_flying;

	float buff;

	float lowest_y;
	GLFWwindow *window;
};

class Sun {
public:
	Sun();

	Model* model;
	glm::vec3 center;
	float radius;
	// float scale;
	void move(glm::vec3 val);

	bool touch(glm::vec3 pos);
};

class SkyBox {
public:
	SkyBox();

	Model* model;
	float len;
};

class Box {
public:
	Box(Model* box_template, glm::vec3 position,int id);

	Model* model;
	glm::vec3 center;
	int id;

	void move_to(glm::vec3 new_pos);
};

#endif
