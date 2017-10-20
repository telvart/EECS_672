// Pyramid.h

#ifndef Pyramid_H
#define Pyramid_H

#include "ModelView.h"
#include "ShaderIF.h"

typedef float vec3[3];

class Pyramid : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Pyramid(ShaderIF* sIF, cryph::AffPoint bottom, float height, float width, vec3 color);
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
	ShaderIF* shaderIF;
	float kd[3];
};

#endif
