#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

uniform mat4 projection2D;
uniform mat4 view2D;
uniform mat3 model2D;

void main() {
    // Apply 2D transformation
    vec3 worldPos = model2D * vec3(aPos, 1.0);

    // Convert to clip space
    gl_Position = projection2D * view2D * vec4(worldPos.xy, 0.0, 1.0);

    color = aColor;
    texCoord = aTexCoord;
}
