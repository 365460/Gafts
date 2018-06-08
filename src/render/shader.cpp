#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath, string name) {
    char* vertex_source = open_file(vertexPath);
    char* fragment_source = open_file(fragmentPath);

	shader_name = name;

   //std::cout << "Vertex: \n" <<  vertex_source << "\n";
   //std::cout << "fragment: \n" <<  fragment_source << "\n";

    vertexShader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader_id, 1, &vertex_source, NULL);
    glCompileShader(vertexShader_id);

    fragmentShader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader_id, 1, &fragment_source, NULL);
    glCompileShader(fragmentShader_id);

    program_id = glCreateProgram();
    glAttachShader(program_id, vertexShader_id);
    glAttachShader(program_id, fragmentShader_id);
    glLinkProgram(program_id);

    glDeleteShader(vertexShader_id);
    glDeleteShader(fragmentShader_id);

    check_ok();
    delete [] vertex_source;
    delete [] fragment_source;
}

char* Shader::open_file(const char* filename) {
    std::ifstream in(filename, std::ios::binary);

    if(!in.good()) std::cout << "[x] can't open " << filename << "\n";

    in.seekg(0, in.end);
    int length = in.tellg();
    in.seekg(0, in.beg);

    char* file = new char[length+10];
    in.read(file, length);
    in.close();
	file[length] = '\0';

    return file;
}

void Shader::check_ok() {
    GLint success;
    GLchar infoLog[1024];

    bool ok = true;
    glGetShaderiv(vertexShader_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        ok = false;
        glGetShaderInfoLog(vertexShader_id, 1024, NULL, infoLog);
        std::cout << "[x] " << shader_name << ": compile vertex shader fail : " << infoLog << "\n";
    }

    glGetShaderiv(fragmentShader_id, GL_COMPILE_STATUS, &success);
    if(!success) {
        ok = false;
        glGetShaderInfoLog(fragmentShader_id, 1024, NULL, infoLog);
        std::cout << "[x] " << shader_name  << ": compile frag shader fail : " << infoLog << "\n";
    }

    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if(!success) {
        ok = false;
        glGetProgramInfoLog(program_id, 1024, NULL, infoLog);
        std::cout << "[x] program linking error : " << infoLog << "\n";
    }

    if(ok) std::cout << "[v] " << shader_name << " shader ok\n";
}

void Shader::use_program() {
    glUseProgram(program_id);
}

void Shader::disable_program() {
    glUseProgram(0);
}
