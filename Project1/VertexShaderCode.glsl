#version 430  // GLSL version your computer supports

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;

uniform mat4 modelTransformMatrix;
uniform mat4 projectionMatrix;

out vec3 theColor;

void main()
{
	vec4 v = vec4(position, 1.0);
	vec4 v2 = modelTransformMatrix * v;
	vec4 v3 = projectionMatrix * v2;
	gl_Position = v3;//projectedPosition;
	theColor = vertexColor;
}