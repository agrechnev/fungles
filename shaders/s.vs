#version 300 es

layout(location = 0) in vec3 pos;
layout(location = 2) in vec2 tex;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
   gl_Position = proj*view*model*vec4(pos, 1.0f);
   texCoord = vec2(tex.x, 1.0f-tex.y);
}
