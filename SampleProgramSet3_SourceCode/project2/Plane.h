// Plane.h

#ifndef Plane_H
#define Plane_H

#include "ModelView.h"
#include "ShaderIF.h"
#include <iostream>

typedef float vec3[3];

class Plane : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Plane(ShaderIF* sIF, cryph::AffPoint tl, cryph::AffPoint tr,
		 cryph::AffPoint bl, cryph::AffPoint br, vec3 color);
	virtual ~Plane();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void definePlane();
	void render();
private:

	ShaderIF* shaderIF;

	GLuint vao[1];
	GLuint vbo[2];

	float xyz[6];

	cryph::AffPoint tLeft, tRight, bLeft, bRight;
	cryph::AffVector normal;
	float kd[3];
};

#endif
