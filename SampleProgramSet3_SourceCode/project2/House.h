// House.h

#ifndef HOUSE_H
#define HOUSE_H

#include "ModelView.h"
#include "ShaderIF.h"
#include "Plane.h"
#include "Pyramid.h"

typedef float vec3[3];

class House : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	House(ShaderIF* sIF, cryph::AffPoint houseBottom, float width, float length, float height, float roofHeight);
	virtual ~House();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();

	void defineHouse();
private:

	void renderRoof(cryph::AffVector n);

	GLuint vao[1];
	GLuint vbo[2];



	ShaderIF* shaderIF;

	Block *floor1, *wall1, *wall2, *wall3, *wall4, *door, *doorKnob;
	Pyramid* roof;

	float width, length, height, roofHeight;
	const float wallWidth;
	cryph::AffPoint m_bottom;

	float kd[3];
	float xyz[6];
};

#endif
