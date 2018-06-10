#include "play.h"

float box_len = 5.12, box_width = 5.12, box_height = 0.44;
glm::vec3 warning_color[] = { glm::vec3(0, 0, 0), glm::vec3(0.3, 0, 0) };

Play::Play(GLFWwindow *win):window(win) {}

void Play::init() {
	platform.clear();

	camera = new Camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	main_shader = new Shader("./shader/main.vert", "./shader/main.frag", "main");
	main_shader->use_program();
	main_shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	sky_shader = new Shader("./shader/sky.vert", "./shader/sky.frag", "sky");
	light_shader = new Shader("./shader/light.vert", "./shader/light.frag", "light");

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.10f, 1000.0f);
	render = new Render(projection);

	landing_id = 0;
	is_tremor = 0;
	fps = 400;
	is_warning = false;
	is_flying = false;
	attach = false;
	buff = 0;
	load_object();
}

void Play::draw() {

	static float preFrame = glfwGetTime();
	static int cnt = 0;
	float currenFrame = glfwGetTime();
	cnt++;
	if( currenFrame - preFrame >= 0.5){
		preFrame = currenFrame;
		fps = cnt * 2;
		cnt = 0;
	}

	update_v();
	update_scene();
	update_object();
}

bool Play::is_lose() {
	if(sun->touch(camera->position)) {
		cout << "==================================\n";
		cout << "touch sun!\n";
		cout << "Score: " << -camera->position.y << endl;
		cout << "==================================\n";
		return true;
	}
	else if(is_flying and glfwGetTime() - start_flying >= MAX_TIME_IN_SPACE) {
		cout << "==================================\n";
		cout << "too long\n";
		cout << "Score: " << -camera->position.y << endl;
		cout << "==================================\n";
		return true;
	}
	return false;
}

void Play::update_v() {

	v_sun = (SUN_PLUNG_PRE_SECOND + buff /15 ) / fps;
	v_play = (PLAYER_PLUNG_PRE_SECOND + buff ) / fps;

	// cout << "fps = " << fps << ", v_sun = " << v_sun << ", v_play = " << v_play << endl;
}

void Play::set_warning() {

	float diff = glfwGetTime() - start_flying;
	int id = is_warning;
	if(is_warning and (MAX_TIME_IN_SPACE-diff) < 3) { // emergency
		static int cnt = 0;
		if(cnt >= 5) id = cnt = 0;
		else id =  1;
		cnt++;
	}
	else id = is_warning;

	main_shader->use_program();
	main_shader->setVec3("warning_color", warning_color[id]);

	sky_shader->use_program();
	sky_shader->setVec3("warning_color", warning_color[id]);
}

void Play::update_scene() {

	/*	update position of sun */
	glm::vec4 light_pos = sun->model->get_translation() * glm::vec4(sun->center, 1.0f);
	// cout << "light_pos = " << light_pos.x << ", " << light_pos.y << ", " << light_pos.z << endl;
	light_pos = glm::vec4(0, 400, 0, 0);
	main_shader->use_program();
	main_shader->setVec3("lightPos", light_pos);

	/*	update emergency mode*/
	set_warning();

	/* magic part T_T */
	glm::mat4 trans(1.0);
	trans = glm::translate(trans, camera->position);
	sky_shader->setMat4("camera_position", trans);

	render->render(camera->get_view_matrix());
}

void Play::update_object() {
	attach = check_attach();

	if(!attach) {
		camera->position -= glm::vec3(0, v_play, 0);
		is_warning = 1;
		is_tremor = 0;
		if(!is_flying) {
			is_flying = true;
			start_flying = glfwGetTime();
		}
	}
	else { // interesting tremor simulating landing
		is_flying = 0;
		is_warning = 0;
		static bool first_tremor = 1;
		static glm::vec3 original_pos;
		float diff_time = glfwGetTime() - last_landing;
		if( diff_time < TREMOR_TIME ) {
			if(first_tremor) original_pos = camera->position;

			float d = TREMOR_INIT/ diff_time;
			if(d > TREMOR_MAX_HE) d = TREMOR_MAX_HE;
			// cout << d << endl;

			if(rand()%2 or first_tremor) camera->position.y += d;
			else camera->position.y -= d;

			if( camera->position.y < original_pos.y ) camera->position.y = original_pos.y;
			first_tremor = 0;
		}
		else {
			if(is_tremor) camera->position.y = original_pos.y;
			is_tremor = 0;
			first_tremor = 1;
		}
	}

	sun->move(glm::vec3(0, -v_sun, 0 ));

	for(auto& box: platform) {
		// if(landing_id != box->id and box->center.y >= sun->center.y - sun->radius ) {
		if(landing_id != box->id and box->center.y >= camera->position.y + 5 ) {
			float angle = glm::radians( (float)(rand()%361) );
			float radius = rand()%40;
			glm::vec3 new_pos = glm::vec3(sin(angle)*radius, 0, cos(angle)*radius);
			new_pos.y = lowest_y -rand()%40;

			box->move_to( new_pos );
			lowest_y = std::min(box->center.y, lowest_y);
		}
	}
}

void Play::load_object() {

	sun = new Sun();
	sky_box = new SkyBox();

	//Model* box_template = Loader::load_box(box_len, box_width, box_height, 0);
	Model* box_template = Loader::load_model("./res/model/platform/platform.obj");

	render->add_models(light_shader, sun->model);
	render->add_models(sky_shader, sky_box->model);

	lowest_y = 0;
	for(int i=0; i<30; i++) {
		float angle = glm::radians( 360.0/10 * i );
		float he = rand()%400;
		float radius = rand()%30;
		he *= -1;
		glm::vec3 position = glm::vec3(sin(angle)*radius, he, cos(angle)*radius);

		if(he < lowest_y) lowest_y = he;
		Box* box = new Box(box_template, position, i);
		position = box->center;
		platform.push_back(box);

		render->add_models(main_shader, box->model);
	}
}

bool Play::check_inside(glm::vec3 new_pos) {

	float x = new_pos.x;
	float y = new_pos.y;
	float z = new_pos.z;

	float esp = 0.0;
	for(auto& box: platform) {
		float diff_x = abs(box->center.x - x) - esp;
		float diff_y = abs(box->center.y - y) - esp;
		float diff_z = abs(box->center.z - z) - esp;

		if( diff_x < box_len and diff_z < box_width and diff_y <= box_height+ 0.2) {
			return true;
		}
	}
	return false;

}

bool Play::check_attach() {

	if(is_tremor) {
		return true;
	}
	float x = camera->position.x;
	float y = camera->position.y;
	float z = camera->position.z;

	float esp = 0.8;
	for(auto& box: platform) {
		float diff_x = abs(box->center.x - x);
		float diff_y = abs(box->center.y - y);
		float diff_z = abs(box->center.z - z);
		bool above = y > box->center.y;

		//cout << "diff= " << diff_x << " " << diff_y << " " << diff_z << endl;
		//cout << abs(diff_y - box_height) << endl;
		if( above and diff_x < box_len and diff_z < box_width  and diff_y<=box_height + esp) {
			// cout << "camera: " << x << " " << y << " " << z << endl;
			// cout << "box center " << box->center.x << ", " << box->center.y << ", " << box->center.z << endl;
			// cout << "diff " << diff_x << ", " << diff_y << ", " << diff_z << endl;
			// cout << "box len " << box_len << " " << box_height << " " << box_width << endl;
			if(box->id != landing_id) {
				is_tremor = true;
				last_landing = glfwGetTime();
				landing_id = box->id;
			}
			return true;
		}
	}
	landing_id = -1;
	return false;
}

void Play::process_keyboard() {
	static float lastFrame = 0;

	float currenFrame = glfwGetTime();
	float deltaTime = currenFrame - lastFrame;
	lastFrame = currenFrame;

	buff -= 0.1;
	buff = std::max(buff , 0.0f);
	glm::vec3 vec(0);
    float speed = (CAMERA_INIT + buff ) * deltaTime;
    /* move camer froward/backward/left/right*/
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		vec = camera->front * speed;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		vec = camera->front * -speed;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        vec = glm::normalize(glm::cross(camera->front, camera->up_direction)) * -speed;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        vec = glm::normalize(glm::cross(camera->front, camera->up_direction)) * speed;
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)  buff += 0.5;

	cout << "buff = " << buff << endl;
	if(!check_inside(vec + camera->position)) {
		camera->position += vec;
		camera->update_vector();
	}
	else if(attach) {
		vec.y = 0;
		camera->position += vec;
		camera->update_vector();
	}
}

void Play::process_mouse(double xpos, double ypos) {
    static float lastX = 0, lastY = 0;
    static bool firstMouse = true;

    if(firstMouse) {
        firstMouse = false;
        lastX = xpos;
        lastY = ypos;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= MOUSE_SENSITIVITY;
    yoffset *= MOUSE_SENSITIVITY;

    camera->add_yaw( xoffset );
    camera->add_pitch( yoffset );
    camera->update_vector();
}

Sun::Sun() {
	model = Loader::load_model("./res/model/planet/planet.obj");

	radius = 2.60687 * SUN_RADIUS_SCALE;

	glm::mat4 trans(1.0);
	trans = glm::scale( trans, glm::vec3(SUN_RADIUS_SCALE, SUN_RADIUS_SCALE, SUN_RADIUS_SCALE) );
	trans = glm::translate(trans, glm::vec3(0, 10, 0)  );
	model->set_translation(trans);

	center = model->get_center();

}

bool Sun::touch(glm::vec3 pos) {
	if( pos.y >= center.y - radius) return true;
	return false;
}

void Sun::move(glm::vec3 val) {
	model->move( val );
	center = model->get_center();
	// cout << "sun move " << " y = " << center.y << endl;
}

SkyBox::SkyBox() {
	model = Loader::load_box(SKY_LEN, SKY_LEN, SKY_LEN, Loader::load_texture("./res/image/sky.jpg"));
}

Box::Box(Model* box_template, glm::vec3 position, int id):id(id) {

	model = new Model(box_template->get_raw_models());

	move_to( position );
	//glm::mat4 trans(1.0);
	//trans = glm::translate(trans, position);

	//float angle = glm::radians(270.0);

	//// trans = glm::scale(trans, glm::vec3(10, 4, 10));
	//trans = glm::scale(trans, glm::vec3(10, 4, 10));
	//trans = glm::rotate(trans, angle, glm::vec3(1.0f, 0.0f, 0.0f));;
	//model->set_translation( trans );
	//center = model->get_center();

	glm::vec3 size = model->get_size();
	box_len = size.x / 2;
	box_width = size.z / 2;
	box_height = size.y / 2;
}

void Box::move_to(glm::vec3 position) {
	glm::mat4 trans(1.0);
	trans = glm::translate(trans, position);

	float angle = glm::radians(270.0);

	trans = glm::scale(trans, glm::vec3(10, 4, 10));
	trans = glm::rotate(trans, angle, glm::vec3(1.0f, 0.0f, 0.0f));;
	model->set_translation( trans );

	center = model->get_center();
}
