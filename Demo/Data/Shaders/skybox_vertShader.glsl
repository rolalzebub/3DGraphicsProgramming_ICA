#version 330

uniform mat4 view_xform;
uniform mat4 projection_xform;


layout (location=0) in vec3 vertex_position;

out vec3 varying_texcoord;

void main(void)
{ 
	varying_texcoord = vertex_position;

	vec4 pos = projection_xform * view_xform * vec4(vertex_position, 1.0);

	gl_Position = pos.xyww;
}