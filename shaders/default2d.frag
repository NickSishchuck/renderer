#version 330 core

in vec3 color;
in vec2 texCoord;

out vec4 FragColor;

uniform int shapeType; // 0 = rectangle, 1 = circle
uniform float smoothness;

void main() {
    if (shapeType == 1) {
        // Circle rendering
        float dist = length(texCoord);
        float alpha = 1.0 - smoothstep(1.0 - smoothness, 1.0, dist);
        FragColor = vec4(color, alpha);
    } else {
        // Rectangle rendering
        FragColor = vec4(color, 1.0);
    }
}
