#version 430 core

out vec4 FragColor;

in vec3 worldPos;

uniform vec2 u_worldMin;
uniform vec2 u_worldSize;
uniform sampler2D u_texture;


void main()
{
    vec2 uv = (worldPos.xy - u_worldMin)/u_worldSize;
    vec4 c = texture(u_texture, uv);
    FragColor = vec4(c.rgb, 1.0);
}

