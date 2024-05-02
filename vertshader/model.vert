#version 330 core

in vec3 position;
uniform mat4x4 rotationMatrix;

void main()
{
    // Define vertex position in space
    gl_Position = rotationMatrix * vec4(position, 1.0f);
}
