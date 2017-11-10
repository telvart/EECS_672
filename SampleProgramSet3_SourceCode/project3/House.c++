
#include "House.h"

House::House(ShaderIF* sIF, cryph::AffPoint houseBottom, float width, float length, float height,
   float roofHeight, bool isDog, PhongMaterial& mat)
  :wallWidth(5), SceneElement(sIF, mat)
{
	this->width = width;
	this->length = length;
	this->height = height;
  this->roofHeight = roofHeight;
	m_bottom = houseBottom;
  dogHouse = isDog;
  defineHouse();
}

House::~House()
{
  for(int i=0; i<models.size(); i++)
    delete models[i];

  if(!dogHouse) {delete doorKnob;}
}

void House::defineHouse()
{
	xyz[0] = m_bottom.x - (width/2);
	xyz[1] = m_bottom.x + (width/2);
	xyz[2] = m_bottom.y - (length/2);
	xyz[3] = m_bottom.y + (length/2);
	xyz[4] = m_bottom.z;
	xyz[5] = m_bottom.z + height + roofHeight;

	PhongMaterial gray(0.9607, 0.9607, 0.9607, 0.9607, 0.9607, 0.9607, 0.9607, 0.9607, 0.9607, 10, 1);
  PhongMaterial red(1, 0, 0.125, 1, 0, 0.125, 1, 0, 0.125, 10, 1);
  PhongMaterial black(0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
  PhongMaterial brick(0.796, 0.255, 0.329, 0.796, 0.255, 0.329, 0.796, 0.255, 0.329, 5, 1);

  cryph::AffPoint floorLoc(m_bottom.x-(width/2), m_bottom.y-(length/2), m_bottom.z);
  cryph::AffPoint wall1Loc(m_bottom.x-(width/2), m_bottom.y-(length/2), m_bottom.z+1);
  cryph::AffPoint wall2Loc(m_bottom.x-(width/2), m_bottom.y+(length/2)-wallWidth, m_bottom.z+1);
  cryph::AffPoint wall3Loc((m_bottom.x+(width/2))-wallWidth, m_bottom.y-(length/2), m_bottom.z+1);

   models.push_back(new Block(shaderIF, floorLoc.x, floorLoc.y, floorLoc.z, width, length, 1, gray)); //floor
   models.push_back(new Block(shaderIF, wall1Loc.x, wall1Loc.y, wall1Loc.z, wallWidth, length, height, matl)); //leftWall
   models.push_back(new Block(shaderIF, wall2Loc.x, wall2Loc.y, wall2Loc.z, width, wallWidth, height, matl)); //rear wall
   models.push_back(new Block(shaderIF, wall3Loc.x, wall3Loc.y, wall3Loc.z, wallWidth, length, height, matl)); //right wall
   models.push_back(new Block(shaderIF, wall1Loc.x, wall1Loc.y, wall1Loc.z, width, wallWidth, height, matl)); //front wall

  float thirdWidth = width / 3;
  float thirdHeight = height / 3;
  cryph::AffPoint door1Loc(wall1Loc.x+thirdWidth, wall1Loc.y-1, wall1Loc.z);
  cryph::AffPoint knobLoc (door1Loc.x+thirdWidth-(0.3*thirdWidth), door1Loc.y, (2*thirdHeight)/2);

  if(!dogHouse)
  {
    doorKnob = new Block(shaderIF, knobLoc.x, knobLoc.y, knobLoc.z, 0.2*thirdWidth, -1*(0.2*thirdWidth), 0.2*thirdHeight, black);
    models.push_back(new Block(shaderIF, door1Loc.x, door1Loc.y, door1Loc.z, thirdWidth, 1, 2*thirdHeight, red));
  }
  else
  {
    models.push_back(new Block(shaderIF, door1Loc.x, door1Loc.y, door1Loc.z, thirdWidth, 1, 2*thirdHeight, black));
  }

  cryph::AffPoint roofBottom(m_bottom.x, m_bottom.y, m_bottom.z+height);
  models.push_back(new Pyramid(shaderIF, roofBottom, roofHeight, width + (0.1*width), brick));
}

void House::getMCBoundingBox(double* xyzLimits) const
{
  xyzLimits[0] = xyz[0]; xyzLimits[1] = xyz[1];
	xyzLimits[2] = xyz[2]; xyzLimits[3] = xyz[3];
	xyzLimits[4] = xyz[4]; xyzLimits[5] = xyz[5];
}

void House::render()
{

  for(int i=0; i<models.size(); i++)
    models[i] -> render();

  if(!dogHouse)
  {
    doorKnob -> render();
  }
}
