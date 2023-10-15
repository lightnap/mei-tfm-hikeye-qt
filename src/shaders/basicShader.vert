#version 430 core

in vec3 vertex;

uniform mat4 transformMatrix;

out vec3 fcolor;

void main()  {
    gl_Position = transformMatrix * vec4 (vertex, 1.0);
    fcolor = vertex;
}
