#version 420

layout (location = 0) in vec3 vVertex;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;

out vec4 glPos;

void main()
{
	gl_Position = vProjection * vView * vModel * vec4(vVertex, 1.0);
        glPos = gl_Position;
}
