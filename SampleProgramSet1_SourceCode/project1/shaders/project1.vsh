#version 410 core

// replace the placeholder implementation here

in vec2 MC;
uniform vec4 scaleTranslate;

void main()
{
	 float ldsx = scaleTranslate[0]*MC[0] + scaleTranslate[1];
	 float ldsy = scaleTranslate[2]*MC[1] + scaleTranslate[3];
	 gl_Position = vec4(ldsx, ldsy, 0, 1);

	//gl_Position = vec4(MC[0], MC[1], 0, 1);
}
