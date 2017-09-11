#version 410 core

// Replace the placeholder implementation here...

uniform vec3 myColor;

out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(myColor, 1.0);
}
