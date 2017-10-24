// TreeTop.h

#ifndef TreeTop_H
#define TreeTop_H

#include "ModelView.h"
#include "ShaderIF.h"

typedef float vec3[3];

class TreeTop : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	TreeTop(ShaderIF* sIF, cryph::AffPoint bottom, double baseRadius, double height, vec3 color);
	virtual ~TreeTop();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:

	void defineTreeTop();
	const int PointsAroundBase;
	const int radius;

	GLuint vao[1];
	GLuint vbo[2];

	float xyz[6];


	cryph::AffPoint m_bottom;
	cryph::AffPoint m_top;
	cryph::AffVector axis;


	ShaderIF* shaderIF;
	float kd[3];
	float ka[3];
};

#endif
