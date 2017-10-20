
#include "House.h"

House::House(ShaderIF* sIF, cryph::AffPoint houseBottom, float width, float length, float height, float roofHeight)
  :shaderIF(sIF), wallWidth(5)
{
	this->width = width;
	this->length = length;
	this->height = height;
  this->roofHeight = roofHeight;
	m_bottom = houseBottom;
  kd[0] = 0.796; kd[1] = 0.255; kd[2] = 0.329;
  defineHouse();
}

House::~House()
{
  delete floor1;
  delete wall1;
  delete wall2;
  delete wall3;
  delete wall4;
  delete door;
  delete doorKnob;
  delete roof;
}

void House::defineHouse()
{
	xyz[0] = m_bottom.x - (width/(float)2);
	xyz[1] = m_bottom.x + (width/(float)2);
	xyz[2] = m_bottom.y - (length/(float)2);
	xyz[3] = m_bottom.y + (length/(float)2);
	xyz[4] = m_bottom.z;
	xyz[5] = m_bottom.z + height + roofHeight;

  vec3 gray = {0.86, 0.86, 0.86};
  vec3 house = {0.862, 0.752, 0.415};
  vec3 red = {1, 0, 0.125};
  vec3 black = {0, 0, 0};
  vec3 brick = {0.796, 0.255, 0.329};

  cryph::AffPoint floorLoc(m_bottom.x-(width/2), m_bottom.y-(length/2), m_bottom.z);
  cryph::AffPoint wall1Loc(m_bottom.x-(width/2), m_bottom.y-(length/2), m_bottom.z+1);
  cryph::AffPoint wall2Loc(m_bottom.x-(width/2), m_bottom.y+(length/2), m_bottom.z+1);
  cryph::AffPoint wall3Loc((m_bottom.x+(width/2))-wallWidth, m_bottom.y-(length/2), m_bottom.z+1);

  float thirdWidth = width / 3;
  float thirdHeight = height / 3;
  cryph::AffPoint door1Loc(wall1Loc.x+thirdWidth, wall1Loc.y-1, wall1Loc.z);
  cryph::AffPoint knobLoc (door1Loc.x+thirdWidth-(0.3*thirdWidth), door1Loc.y, (2*thirdHeight)/2);

  floor1 = new Block(shaderIF, floorLoc.x, floorLoc.y, floorLoc.z, width, length, 1, gray); //floor
  wall1 = new Block(shaderIF, wall1Loc.x, wall1Loc.y, wall1Loc.z, wallWidth, length, height, house); //leftWall
  wall2 = new Block(shaderIF, wall2Loc.x, wall2Loc.y, wall2Loc.z, width, wallWidth, height, house); //rear wall
  wall3 = new Block(shaderIF, wall3Loc.x, wall3Loc.y, wall3Loc.z, wallWidth, length, height, house); //right wall
  wall4 = new Block(shaderIF, wall1Loc.x, wall1Loc.y, wall1Loc.z, width, wallWidth, height, house); //front wall
  door = new Block(shaderIF, door1Loc.x, door1Loc.y, door1Loc.z, thirdWidth, 1, 2*thirdHeight, red);
  doorKnob = new Block(shaderIF, knobLoc.x, knobLoc.y, knobLoc.z, 0.2*thirdWidth, -1*(0.2*thirdWidth), 0.2*thirdHeight, black);

  cryph::AffPoint roofBottom(m_bottom.x, m_bottom.y, m_bottom.z+height);
  roof = new Pyramid(shaderIF, roofBottom, roofHeight, width, brick);

}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void House::getMCBoundingBox(double* xyzLimits) const
{
  xyzLimits[0] = xyz[0]; xyzLimits[1] = xyz[1];
	xyzLimits[2] = xyz[2]; xyzLimits[3] = xyz[3];
	xyzLimits[4] = xyz[4]; xyzLimits[5] = xyz[5];
}


void House::render()
{
  floor1 -> render();
  wall1 -> render();
  wall2 -> render();
  wall3 -> render();
  wall4 -> render();
  door  -> render();
  doorKnob -> render();
  roof -> render();
}
