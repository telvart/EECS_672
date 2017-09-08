#version 410 core

// replace the placeholder implementation here

in vec2 MC;
uniform vec4 scaleTranslate;

void main()
{
	gl_Position = vec4(MC[0], MC[1], 0, 1);
}
