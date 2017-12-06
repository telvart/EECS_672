#ifndef BLOCK_H
#define BLOCK_H

#include "SceneElement.h"

typedef float vec3[3];
typedef float vec2[2];

class Block : public SceneElement
{
public:
	Block(ShaderIF* sIF, float cx, float cy, float cz, // lower left corner
	      float lx, float ly, float lz, PhongMaterial& mat);// lengths in 3 directions

	Block(ShaderIF* sIF, float cx, float cy, float cz, // lower left corner
	      float lx, float ly, float lz, PhongMaterial& mat, const char* texImgName);

	virtual ~Block();

	void getMCBoundingBox(double* xyzLimits) const;
	bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
	void render();

private:

	void defineBlock();
	void renderBlock();
	void renderBlockWithTexture();
	void sendTextureData(vec2 tex[]);
	bool hasTexture;

	GLuint vao[1];
	GLuint vbo[2];
	GLuint ebo[3];

	float xmin, xmax, ymin, ymax, zmin, zmax;
	static GLuint indexList[3][4];
};


#endif
