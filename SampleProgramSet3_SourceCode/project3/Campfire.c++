// Campfire.c++

#include "Campfire.h"

Campfire::Campfire(ShaderIF* sIF, cryph::AffPoint location, float height,
   float radius, float logRadius, float smokeH, PhongMaterial& matl)
 : SceneElement(sIF, matl), fireRadius(radius), logR(logRadius), fireHeight(height)
{
  this->smokeH = smokeH;
  this -> location = location;
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
  glDeleteVertexArrays(1, vao);
  glDeleteBuffers(2, vbo);
}

void Campfire::defineCampFire()
{
  xyz[0] = location.x - fireRadius;
  xyz[1] = location.x + fireRadius;
  xyz[2] = location.y - fireRadius;
  xyz[3] = location.y + fireRadius;
  xyz[4] = location.z;
  xyz[5] = location.z + fireHeight + smokeH;

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

  logs[0] = BasicShape::makeBoundedCylinder(vert1, fireTop, logR, 30, 2, BasicShape::CAP_AT_NEITHER);
  logsR[0] = new BasicShapeRenderer(shaderIF, logs[0]);

  logs[1] = BasicShape::makeBoundedCylinder(vert2, fireTop, logR, 30, 2, BasicShape::CAP_AT_NEITHER);
  logsR[1] = new BasicShapeRenderer(shaderIF, logs[1]);

  logs[2] = BasicShape::makeBoundedCylinder(vert3, fireTop, logR, 30, 2, BasicShape::CAP_AT_NEITHER);
  logsR[2] = new BasicShapeRenderer(shaderIF, logs[2]);

  logs[3] = BasicShape::makeBoundedCylinder(vert4, fireTop, logR, 30, 2, BasicShape::CAP_AT_NEITHER);
  logsR[3] = new BasicShapeRenderer(shaderIF, logs[3]);

  logs[4] = BasicShape::makeBoundedCylinder(vert5, fireTop, logR, 30, 2, BasicShape::CAP_AT_NEITHER);
  logsR[4] = new BasicShapeRenderer(shaderIF, logs[4]);

  logs[5] = BasicShape::makeBoundedCylinder(vert6, fireTop, logR, 30, 2, BasicShape::CAP_AT_NEITHER);
  logsR[5] = new BasicShapeRenderer(shaderIF, logs[5]);

  logs[6] = BasicShape::makeBoundedCylinder(vert7, fireTop, logR, 30, 2, BasicShape::CAP_AT_NEITHER);
  logsR[6] = new BasicShapeRenderer(shaderIF, logs[6]);

  logs[7] = BasicShape::makeBoundedCylinder(vert8, fireTop, logR, 30, 2, BasicShape::CAP_AT_NEITHER);
  logsR[7] = new BasicShapeRenderer(shaderIF, logs[7]);

  logs[8] = BasicShape::makeBoundedCone(location, fireTop, fireRadius-(2*logR), 0.1, 35, 2, BasicShape::CAP_AT_NEITHER);
  logsR[8] = new BasicShapeRenderer(shaderIF, logs[8]);

  fire = BasicShape::makeBoundedCone(fireTop, flameTop, 2*logR, 0.1, 35, 2, BasicShape::CAP_AT_NEITHER);
  logsR[8] = new BasicShapeRenderer(shaderIF, fire);

  //NOTE: Had everything above, decided to add in smoke trails from the top

  cryph::AffVector U = cryph::AffVector::xu; // x and y axes
  cryph::AffVector V = cryph::AffVector::yu;

  numPointsinSmoke = 60;
  float deltaHeight = ((flameTop.z + smokeH) - flameTop.z) / numPointsinSmoke;
  float currentHeight = flameTop.z;

  double theta = 0;
  double dTheta = 2 * M_PI / numPointsinSmoke;

  vec3* coords = new vec3[3*numPointsinSmoke];
  vec3* norms = new vec3[3*numPointsinSmoke];

  cryph::AffPoint last = flameTop;

  for(int i=0; i<numPointsinSmoke; i++)
  {

    cryph::AffPoint p = flameTop + (0.3 * fireRadius) * (cos(theta)*U + sin(theta)*V);
    p.z = currentHeight;
    cryph::AffVector lastToP = p - last;
    cryph::AffVector normal;

    lastToP.arbitraryNormal(normal);
    norms[i][0] = normal.dx; norms[i][1] = normal.dy; norms[i][2] = normal.dz;
    p.aCoords(coords, i);

    last.assign(coords[i][0], coords[i][1], coords[i][2]);
    theta += dTheta;
    currentHeight += deltaHeight;
  }

  currentHeight = flameTop.z;
  cryph::AffVector dir(1, 1, 0);
  flameTop = flameTop + dir;
  theta = M_PI;
  last = flameTop;

  for(int i=numPointsinSmoke; i<2*numPointsinSmoke; i++)
  {
    cryph::AffPoint p = flameTop + 3 * (cos(theta)*U + sin(theta)*V);
    p.z = currentHeight;
    cryph::AffVector lastToP = p - last;
    cryph::AffVector normal;

    lastToP.arbitraryNormal(normal);
    norms[i][0] = normal.dx; norms[i][1] = normal.dy; norms[i][2] = normal.dz;
    p.aCoords(coords, i);

    last.assign(coords[i][0], coords[i][1], coords[i][2]);
    theta += dTheta;
    currentHeight += deltaHeight;
  }

  currentHeight = flameTop.z;
  dir = cryph::AffVector(-1, -1, 0);
  flameTop = flameTop + dir;
  theta = M_PI/2;
  last = flameTop;

  for(int i=2*numPointsinSmoke; i<3*numPointsinSmoke; i++)
  {
    cryph::AffPoint p = flameTop + 3 * (cos(theta)*U + sin(theta)*V);
    p.z = currentHeight;
    cryph::AffVector lastToP = p - last;
    cryph::AffVector normal;

    lastToP.arbitraryNormal(normal);
    norms[i][0] = normal.dx; norms[i][1] = normal.dy; norms[i][2] = normal.dz;
    p.aCoords(coords, i);

    last.assign(coords[i][0], coords[i][1], coords[i][2]);
    theta += dTheta;
    currentHeight += deltaHeight;
  }

  glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, 3*numPointsinSmoke * sizeof(vec3), coords, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 3*numPointsinSmoke * sizeof(vec3), norms, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

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

  /*
    NOTE: I am aware these values are supposed to be between 0 and 1. I couldn't
    figure out a way to find a correct normal to the three lines, so i made it that
    not much color would be accrued from diffuse or specular, but since the global
    ambient is a white light, and i'm trying to make my smoke gray (a shade of white),
    and that my implementation of phong prevents any component of Iq being > 1,
    I figured it was safe to do this to achieve a good looking result
  */
  float ka[3] = {2.9607, 2.9607, 2.9607};
  float kd[3] = {0.1, 0.1, 0.1};
  float ks[3] = {0.1, 0.1, 0.1};

  glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd); //need different color to draw smoke trails
  glUniform3fv(shaderIF->ppuLoc("ka"), 1, ka);
  glUniform3fv(shaderIF->ppuLoc("ks"), 1, ks);

  glBindVertexArray(vao[0]);
	glDrawArrays(GL_LINE_STRIP, 0, numPointsinSmoke);
  glDrawArrays(GL_LINE_STRIP, numPointsinSmoke, numPointsinSmoke);
  glDrawArrays(GL_LINE_STRIP, 2*numPointsinSmoke, numPointsinSmoke);

  ka[0] = 1; ka[1] = 0; ka[2] = 0;
  kd[0] = 1; ka[1] = 0; kd[2] = 0;
  ks[0] = 1; ks[1] = 0; ks[2] = 0;

  glUniform3fv(shaderIF->ppuLoc("kd"), 1, kd); //need different color to draw the fire cone on top
  glUniform3fv(shaderIF->ppuLoc("ka"), 1, ka);
  glUniform3fv(shaderIF->ppuLoc("ks"), 1, ks);

  logsR[8] ->drawShape();

	glUseProgram(program);

}
