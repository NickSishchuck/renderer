#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>
#include <iostream>

class Framebuffer {
private:
    GLuint fbo;
    GLuint colorTexture;
    GLuint depthRenderbuffer;
    int width, height;

public:
    Framebuffer(int w, int h);
    ~Framebuffer();

    void Bind();
    void Unbind();
    void Resize(int w, int h);

    GLuint GetTexture() const { return colorTexture; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }

private:
    void CreateFramebuffer();
    void DeleteFramebuffer();
};

#endif // FRAMEBUFFER_H
