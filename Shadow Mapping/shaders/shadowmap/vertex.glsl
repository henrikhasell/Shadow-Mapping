#version 130

in vec3 in_Position;

out vec2 ex_TexCoord;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 modelMatrix = mat4(1.0);

void main()
{
    gl_Position = projectionMatrix * modelMatrix * vec4(in_Position, 1);
}
