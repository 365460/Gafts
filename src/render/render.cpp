#include "render.h"

Render::Render(glm::mat4 projection):projection(projection) {
}

void Render::add_models(Shader* shader, Model* model) {
    shader->use_program();
    shader->setMat4("projection", projection);
    shader->disable_program();

    models.emplace_back(shader, model);
}

void Render::render(glm::mat4 camera_view) {

	//cout << "models size = " << models.size() << endl;
    for(auto& par: models) {
        Shader* shader = par.first;
        Model* model = par.second;
        const auto& raw_models = model->get_raw_models();

        shader->use_program();
        shader->setMat4("view", camera_view);
        shader->setMat4("model_transform", model->get_translation());
        for(int i=0; i<raw_models.size(); i++) {
            model->prepare_rendering(shader, i);

			if(raw_models[i]->get_draw_method() == DRAW_ELEMENT)
				glDrawElements(GL_TRIANGLES, raw_models[i]->get_face_num(), GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(GL_TRIANGLES, 0, raw_models[i]->get_vertice_num());
        }
		shader->disable_program();
    }
}
