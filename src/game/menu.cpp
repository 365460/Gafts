#include "menu.h"

Menu::Menu(GLFWwindow *window) {
    this->window = window;
    this->menu_shader = new Shader("./shader/menu.vert", "./shader/menu.frag", "menu");
    this->texture[0] = Loader::load_texture("./res/image/title1.jpg");
    this->texture[1] = Loader::load_texture("./res/image/title2.jpg");
    this->reset();
}

void Menu::reset() {
    start_playing = false;
    counter = 0;
    current_render = 0;
}

bool Menu::next() {
    return start_playing;
}

void Menu::process_keyboard_change(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
        start_playing = true;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Menu::draw() {
    /* Use two triangle to form the menu */
    float vertices[] = {
        // position           // color            // texture coordinate
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    /* Alternating */
    counter++;
    if (counter > change_rate + current_render * stay_time) {
        current_render ^= 1;
        counter = 0;
    }

    /* Rendering */
    glBindTexture(GL_TEXTURE_2D, texture[current_render]->id);
    menu_shader->use_program();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
