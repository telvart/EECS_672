// Campfire.h

#ifndef Campfire_H
#define Campfire_H

#include "SceneElement.h"
#include "ShaderIF.h"
#include "BasicShapeRenderer.h"

typedef float vec3[3];

class Campfire : public SceneElement
{

public:
	Campfire(ShaderIF* sIF, cryph::AffPoint location, float height, float radius,
		 float logRadius, float smokeH, PhongMaterial& matl);
	virtual ~Campfire();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:

  void defineCampFire();

  float fireRadius;
  float logR;
  float fireHeight;
  float xyz[6];

	int numPointsinSmoke;
	float smokeH;

	GLuint vao[1];
	GLuint vbo[2];

  BasicShape* logs[9], *fire;
  BasicShapeRenderer* logsR[9];

  cryph::AffPoint location;

};

#endif
