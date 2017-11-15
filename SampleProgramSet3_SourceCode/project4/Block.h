#ifndef BLOCK_H
#define BLOCK_H

#include "SceneElement.h"

typedef float vec3[3];

class Block : public SceneElement
{
public:
	Block(ShaderIF* sIF, float cx, float cy, float cz, // lower left corner
	      float lx, float ly, float lz, PhongMaterial& mat);// lengths in 3 directions
				
	virtual ~Block();

	void getMCBoundingBox(double* xyzLimits) const;
	bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
	void render();

private:

	void defineBlock();
	void renderBlock();

	GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[3];

	float xmin, xmax, ymin, ymax, zmin, zmax;
	static GLuint indexList[3][4];
};


#endif
