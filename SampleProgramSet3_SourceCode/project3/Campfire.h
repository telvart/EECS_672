// Campfire.h

#ifndef Campfire_H
#define Campfire_H

#include "SceneElement.h"
#include "ShaderIF.h"
#include "BasicShapeRenderer.h"

class Campfire : public SceneElement
{

public:
	Campfire(ShaderIF* sIF, cryph::AffPoint location, float height, float radius, float logRadius, PhongMaterial& matl);
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

  BasicShape* logs[9];
  BasicShapeRenderer* logsR[9];

	BasicShape* fire;


  cryph::AffPoint location;

};

#endif
