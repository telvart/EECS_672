// Tree.h
/*
#ifndef TREE_H
#define TREE_H

#include "ModelView.h"
#include "ShaderIF.h"

typedef float vec2[2];
typedef float vec3[3];

class Tree : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	Tree(ShaderIF* sIF);
	virtual ~Tree();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:

	void defineTree();
  GLuint vao[1];
  GLuint vbo[2];



	ShaderIF* shaderIF;
	int numPoints;
	float kd[3];
};

#endif
*/
// Block.h

#ifndef BLOCK_H
#define BLOCK_H

#include <GL/gl.h>

#include "ModelView.h"
#include "ShaderIF.h"

class Block : public ModelView
{
public:
	Block(ShaderIF* sIF, float cx, float cy, float cz, // lower left corner
	      float lx, float ly, float lz);// lengths in 3 directions
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
