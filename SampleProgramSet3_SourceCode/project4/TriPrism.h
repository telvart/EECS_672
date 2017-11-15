#ifndef TRIPRISM_H
#define TRIPRISM_H

#include "SceneElement.h"

typedef float vec3[3];

class TriPrism: public SceneElement
{

public:
  TriPrism(ShaderIF* sIF, PhongMaterial& mat, cryph::AffPoint loc, float height, float width, float length, bool onXaxis);
  ~TriPrism();


  void getMCBoundingBox(double* xyzLimitsF) const;
	void render();

private:

void definePrism();
void renderAlongX();
void renderAlongy();

GLuint vao[1];
GLuint vbo[2];
GLuint ebo[4];

static GLuint xTris[2][3];
static GLuint yTris[2][3];
static GLuint xFaces[2][4];
static GLuint yFaces[2][4];

float height;
float width;
float length;
float xyz[6];
cryph::AffPoint loc, bl, br, tl, tr, tt1, tt2;

bool alongX;


};

#endif
