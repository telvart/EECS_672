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


class Block : public ModelView
{
public:
	Block(ShaderIF* sIF, float cx, float cy, float cz, // lower left corner
	      float lx, float ly, float lz, vec3 color);// lengths in 3 directions
	Block(ShaderIF* sIF, double Vdx, double Vdy, double Vdz, vec3 dir);
	virtual ~Block();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
	void render();
private:
	ShaderIF* shaderIF;
	GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[3];

	float xmin, xmax, ymin, ymax, zmin, zmax;
	float kd[3];

	static GLuint indexList[3][4];

	void defineBlock();
	void renderBlock();
};


#endif
