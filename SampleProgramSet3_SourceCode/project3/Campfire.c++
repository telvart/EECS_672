// Campfire.c++

#include "Campfire.h"

Campfire::Campfire(ShaderIF* sIF, cryph::AffPoint location, float height, float radius, float logRadius, PhongMaterial& matl)
 : SceneElement(sIF, matl), fireRadius(radius), logR(logRadius), fireHeight(height)
{
  this -> location = location;
  //fireTop = cryph::AffPoint(location.x, location.y, location.z + fireHeight);
  defineCampFire();
}

Campfire::~Campfire()
{
}


// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}

void Campfire::defineCampFire()
{
  xyz[0] = location.x - fireRadius;
  xyz[1] = location.x + fireRadius;
  xyz[2] = location.y - fireRadius;
  xyz[3] = location.y + fireRadius;
  xyz[4] = location.z;
  xyz[5] = location.z + fireHeight;

  cryph::AffPoint fireTop(location.x, location.y, location.z + fireHeight);

  cryph::AffPoint vert1(location.x - fireRadius, location.y - fireRadius, location.z);
  cryph::AffPoint vert2(location.x + fireRadius, location.y + fireRadius, location.z);

  cryph::AffPoint vert3(location.x - fireRadius, location.y + fireRadius, location.z);
  cryph::AffPoint vert4(location.x + fireRadius, location.y - fireRadius, location.z);

  double dist = location.distanceTo(vert1);

  cryph::AffPoint vert5(location.x - dist, location.y, location.z);
  cryph::AffPoint vert6(location.x + dist, location.y, location.z);

  cryph::AffPoint vert7(location.x, location.y-dist, location.z);
  cryph::AffPoint vert8(location.x, location.y+dist, location.z);

  logs[0] = BasicShape::makeBoundedCylinder(vert1, fireTop, logR, 20, 2, BasicShape::CAP_AT_NEITHER);
  logsR[0] = new BasicShapeRenderer(shaderIF, logs[0]);

  logs[1] = BasicShape::makeBoundedCylinder(vert2, fireTop, logR, 20, 2, BasicShape::CAP_AT_NEITHER);
  logsR[1] = new BasicShapeRenderer(shaderIF, logs[1]);

  logs[2] = BasicShape::makeBoundedCylinder(vert3, fireTop, logR, 20, 2, BasicShape::CAP_AT_NEITHER);
  logsR[2] = new BasicShapeRenderer(shaderIF, logs[2]);

  logs[3] = BasicShape::makeBoundedCylinder(vert4, fireTop, logR, 20, 2, BasicShape::CAP_AT_NEITHER);
  logsR[3] = new BasicShapeRenderer(shaderIF, logs[3]);

  logs[4] = BasicShape::makeBoundedCylinder(vert5, fireTop, logR, 20, 2, BasicShape::CAP_AT_NEITHER);
  logsR[4] = new BasicShapeRenderer(shaderIF, logs[4]);

  logs[5] = BasicShape::makeBoundedCylinder(vert6, fireTop, logR, 20, 2, BasicShape::CAP_AT_NEITHER);
  logsR[5] = new BasicShapeRenderer(shaderIF, logs[5]);

  logs[6] = BasicShape::makeBoundedCylinder(vert7, fireTop, logR, 20, 2, BasicShape::CAP_AT_NEITHER);
  logsR[6] = new BasicShapeRenderer(shaderIF, logs[6]);

  logs[7] = BasicShape::makeBoundedCylinder(vert8, fireTop, logR, 20, 2, BasicShape::CAP_AT_NEITHER);
  logsR[7] = new BasicShapeRenderer(shaderIF, logs[7]);
}


void Campfire::getMCBoundingBox(double* xyzLimits) const
{
  xyzLimits[0] = xyz[0]; xyzLimits[1] = xyz[1];
  xyzLimits[2] = xyz[2]; xyzLimits[3] = xyz[3];
  xyzLimits[4] = xyz[4]; xyzLimits[5] = xyz[5];
}

void Campfire::render()
{
	// 1. Save current and establish new current shader program
	// ...

  GLint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  glUseProgram(shaderIF->getShaderPgmID());


  establishMaterial();
  establishLightingEnvironment();
  establishView();


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  logsR[0] -> drawShape();
  logsR[1] -> drawShape();
  logsR[2] -> drawShape();
  logsR[3] -> drawShape();
  logsR[4] -> drawShape();
  logsR[5] -> drawShape();
  logsR[6] -> drawShape();
  logsR[7] -> drawShape();

  	glUseProgram(program);

	// 2. Establish "mc_ec" and "ec_lds" matrices
	//    call SceneElement::establishView(); to do this

	// 3. Set GLSL's "kd" variable using this object's "kd" instance variable
	//    complete the implementation of SceneElement::establishMaterial and then
	//    call it from here.

	// 4. Establish any other attributes and make one or more calls to
	//    glDrawArrays and/or glDrawElements
	//    If all or part of this model involves texture mapping, complete the
	//    implementation of SceneElement::establishTexture and call it from
	//    here as needed immediately before any glDrawArrays and/or glDrawElements
	//    calls to which texture is to be applied.

	// 5. Reestablish previous shader program
	// ...
}
