#version 330 core
layout(location = 0) in vec2 aPos; // 2D position
layout(location = 1) in vec3 aColor; // Color
layout(location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

uniform mat3 projection2D; // 2D projection matrix (3x3 for 2D homogeneous coords)
uniform mat3 model2D; // 2D model matrix

void main()
{
    vec3 pos = projection2D * model2D * vec3(aPos, 1.0);
    gl_Position = vec4(pos.xy, 0.0, 1.0);

    color = aColor;
    texCoord = aTexCoord;
}
