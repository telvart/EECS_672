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




  float fireRadius;
  float logR;
  float fireHeight;
  float xyz[6];

  BasicShape* logs[4];
  BasicShapeRenderer* logsR[4];



  cryph::AffPoint location;

  void defineCampFire();
	// IMPORTANT NOTE:
	// The ShaderIF and kd (and other) material properties will be
	// stored with the SceneElement piece of this object instance.
	// You only need add instance variables here that are unique
	// to the new subclass you are creating.
};

#endif
