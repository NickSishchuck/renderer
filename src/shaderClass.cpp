#include "../include/shaderClass.h"

std::string get_file_contents(const char* filename){
std::ifstream in(filename, std::ios::binary);
if(in){
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
}
throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile){
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

   GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
   const char* vertexShaderCStr = vertexSource;
   glShaderSource(vertexShader, 1, &vertexShaderCStr, NULL);
   glCompileShader(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
   const char* fragmentShaderCStr = fragmentSource;
   glShaderSource(fragmentShader, 1, &fragmentShaderCStr, NULL);
   glCompileShader(fragmentShader);

   //wrapping our shaders into a so called shader program
   ID = glCreateProgram();
   glAttachShader(ID, vertexShader);
   glAttachShader(ID, fragmentShader);

   glLinkProgram(ID);

   //for some reason a guy in the guide told me that the shaders are already in the program, and we can delete them
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);
}

void Shader::Activate(){
    glUseProgram(ID);
}

void Shader::Delete(){
    glDeleteProgram(ID);
}
