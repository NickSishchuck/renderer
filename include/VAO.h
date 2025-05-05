#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <GL/eglew.h>
#include "VBO.h"
class VAO{
    public:
    GLuint ID;
    VAO();

    void LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLuint type, GLuint stride, void* offset);
    void Bind();
    void Unbind();
    void Delete();
};
#endif
