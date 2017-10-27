// Pyramid.h

#ifndef Pyramid_H
#define Pyramid_H

#include "SceneElement.h"
#include "ShaderIF.h"

typedef float vec3[3];

class Pyramid : public SceneElement
{
public:
	// As before: you will likely want to add parameters to the constructor
	Pyramid(ShaderIF* sIF, cryph::AffPoint bottom, float height, float width, PhongMaterial& mat);
	virtual ~Pyramid();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:

	void definePyramid();

	GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[4];

	cryph::AffPoint m_bottom, m_top, m_bl, m_br, m_tl, m_tr;

	static GLuint indexList[4][3];

	float xyz[6];
	float height, width;
};

#endif
