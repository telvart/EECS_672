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
  for(int i=0; i<9; i++)
  {
    delete logs[i];
    delete logsR[i];
  }
  delete fire;
}

void Campfire::defineCampFire()
{
  xyz[0] = location.x - fireRadius;
  xyz[1] = location.x + fireRadius;
  xyz[2] = location.y - fireRadius;
  xyz[3] = location.y + fireRadius;
  xyz[4] = location.z;
  xyz[5] = location.z + fireHeight;

  cryph::AffPoint fireTop(location.x, location.y, location.z + fireHeight);
  cryph::AffPoint flameTop(fireTop.x, fireTop.y, fireTop.z + (0.25 * fireHeight));

  cryph::AffPoint vert1(location.x - fireRadius, location.y - fireRadius, location.z);
  double dist = location.distanceTo(vert1);
  cryph::AffPoint vert2(location.x + fireRadius, location.y + fireRadius, location.z);
  cryph::AffPoint vert3(location.x - fireRadius, location.y + fireRadius, location.z);
  cryph::AffPoint vert4(location.x + fireRadius, location.y - fireRadius, location.z);
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

  logs[8] = BasicShape::makeBoundedCone(location, fireTop, fireRadius-(2*logR), 0.1, 35, 2, BasicShape::CAP_AT_NEITHER);
  logsR[8] = new BasicShapeRenderer(shaderIF, logs[8]);

  fire = BasicShape::makeBoundedCone(fireTop, flameTop, 2*logR, 0.1, 35, 2, BasicShape::CAP_AT_NEITHER);
  logsR[8] = new BasicShapeRenderer(shaderIF, fire);
}


void Campfire::getMCBoundingBox(double* xyzLimits) const
{
  xyzLimits[0] = xyz[0]; xyzLimits[1] = xyz[1];
  xyzLimits[2] = xyz[2]; xyzLimits[3] = xyz[3];
  xyzLimits[4] = xyz[4]; xyzLimits[5] = xyz[5];
}

void Campfire::render()
{

  GLint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  glUseProgram(shaderIF->getShaderPgmID());

  establishMaterial();
  establishLightingEnvironment();
  establishView();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  for(int i=0; i<8; i++)
  {
    logsR[i] -> drawShape();
  }

  float ka[3] = {1.0, 0.0, 0.0};
  float kd[3] = {1.0, 0.0, 0.0};
  float ks[3] = {1.0, 0.0, 0.0};

  glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd); //need different color to draw the fire cone on top
  glUniform3fv(shaderIF->ppuLoc("ka"), 1, ka);
  glUniform3fv(shaderIF->ppuLoc("ks"), 1, ks);

  logsR[8] ->drawShape();

	glUseProgram(program);

}
