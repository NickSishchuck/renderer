#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform int shapeType; // 0 = rectangle, 1 = circle
uniform float smoothness; // For antialiasing

void main()
{
    if (shapeType == 1) { // Circle
        // Distance from center (texCoord goes from -1 to 1)
        float dist = length(texCoord);

        // Create smooth circle edge
        float alpha = 1.0 - smoothstep(1.0 - smoothness, 1.0, dist);

        FragColor = vec4(color, alpha);
    } else { // Rectangle or other shapes
        FragColor = vec4(color, 1.0);
    }
}
