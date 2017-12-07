
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
  drawingCottage = false;
  defineHouse();
}

House::House(ShaderIF* sIF, PhongMaterial& mat)
  : SceneElement(sIF, mat)
{
  drawingCottage = true;
  dogHouse = false;
  defineCottage();
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
    models.push_back(new Block(shaderIF, door1Loc.x, door1Loc.y, door1Loc.z, thirdWidth, 1, 2*thirdHeight, black));

  cryph::AffPoint roofBottom(m_bottom.x, m_bottom.y, m_bottom.z+height);
  models.push_back(new Pyramid(shaderIF, roofBottom, roofHeight, width + (0.1*width), width + (0.1*width), brick));
}

void House::defineCottage()
{
  PhongMaterial gray(0.9607, 0.9607, 0.9607, 0.9607, 0.9607, 0.9607, 0.9607, 0.9607, 0.9607, 10, 1);
  PhongMaterial red(1, 0, 0.125, 1, 0, 0.125, 1, 0, 0.125, 10, 1);
  PhongMaterial black(0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1);
  PhongMaterial brick(0.796, 0.255, 0.329, 0.796, 0.255, 0.329, 0.796, 0.255, 0.329, 5, 1);
  PhongMaterial screen(0.27, 0.5, 0.70, 0.27, 0.5, 0.70, 0.27, 0.5, 0.70, 10, 0.4);

  cryph::AffVector U(1,0,0);
  cryph::AffVector V(0,1,0);
  cryph::AffVector W(0,0,1);


  cryph::AffPoint r(135, 35.5, 110);
  cryph::AffPoint backEntryLoc(140, 163.5, 110);
  cryph::AffPoint extensLoc(-60, 32.5, 111);

  models.push_back(new Block(shaderIF, 50, -75, 10, 170, 220, 1, gray)); //main cabin floor
  models.push_back(new Block(shaderIF, 50, -75, 10, 2, 220, 100, brick)); //side on extension
  models.push_back(new Block(shaderIF, 220, -75, 10, -170, 2, 100, brick)); //side towards lake
  models.push_back(new Block(shaderIF, 220, -75, 10, -2, 220, 100, brick)); //side on bathroom
  models.push_back(new Block(shaderIF, 50, 145, 10, 170, 2, 100, brick));//back entrance wall

  models.push_back(new Block(shaderIF, 50, -30, 10, -200, 125, 1, gray)); //extension floor
  models.push_back(new Block(shaderIF, 50, -30, 10, -200, 2, 100, red));  //face towards lake
  models.push_back(new Block(shaderIF, -150, -30, 10, 2, 125, 100, red)); //face towards screen porch
  models.push_back(new Block(shaderIF, -150, 95, 10, 200, -2, 100, red)); //face towards rear entry

  models.push_back(new Block(shaderIF, -260, -30, 10, 110, 125, 1, gray)); //screen porch floor
  models.push_back(new Block(shaderIF, -260, -30, 10, 5, 5, 100, brick));  /*screen pillars */
  models.push_back(new Block(shaderIF, -260, 95, 10, 5, -5, 100, brick));
  models.push_back(new Block(shaderIF, -260, 30.5, 10, 5, 5, 100, brick));
  models.push_back(new Block(shaderIF, -205, 95, 10, 5, -5, 100, brick));
  models.push_back(new Block(shaderIF, -205, -30, 10, 5, 5, 100, brick));
  //models.push_back(new Block(shaderIF, -205, -30, 10, 55, -40, -2, red));

  models.push_back(new Block(shaderIF, -255, -28.5, 10, 105, 2, 100, screen));/* screens */
  models.push_back(new Block(shaderIF, -258.5, -28.5, 10, 2, 120, 100, screen));
  models.push_back(new Block(shaderIF, -255, 91.5, 10, 105, 2, 100, screen));

  models.push_back(new Block(shaderIF, 50, -150, -50, 170, 75, 60, matl, "textures/deckwood.jpg")); //front porch
  models.push_back(new Block(shaderIF, 50, -150, 10, 3, 3, 40, matl, "textures/deckwood2.jpg")); /* railing lines 106 - 126*/
  models.push_back(new Block(shaderIF, 50, -112.5, 10, 3, 3, 40, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 220, -112.5, 10, -3, 3, 40, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 130, -150, 10, 3, 3, 40, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 130, -190, 10, 3, 3, 40, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 170, -190, 10, -3, 3, 40, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 170, -150, 10, -3, 3, 40, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 220, -150, 10, -3, 3, 40, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 50, -150, 50, 3, 75, 3, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 50, -150, 50, 83, 3, 3, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 167, -150, 50, 53, 3, 3, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 220, -150, 50, -3, 75, 3, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 167, -190, 50, 3, 43, 3, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 170, -190, 50, -40, 3, 3, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 90, -150, 10, 3, 3, 40, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 50, -150, 29, 83, 3, 3, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 50, -150, 29, 3, 75, 3, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 170, -150, 29, 50, 3, 3, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 220, -150, 29, -3, 75, 3, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 170, -150, 29, -3, -40, 3, matl, "textures/deckwood2.jpg"));
  models.push_back(new Block(shaderIF, 170, -190, 29, -40, 3, 3, matl, "textures/deckwood2.jpg"));

  models.push_back(new Block(shaderIF, 130, -150, 10, 40, -40, -8, matl)); //stair platform
  models.push_back(new Block(shaderIF, 130, -150, 2, 8, -8, -52, matl)); /* platform posts */
  models.push_back(new Block(shaderIF, 130, -190, 2, 8, 8, -52, matl));
  models.push_back(new Block(shaderIF, 170, -190, 2, -8, 8, -52, matl));
  models.push_back(new Block(shaderIF, 170, -150, 2, -8, -8, -52, matl));

  models.push_back(new Block(shaderIF, 50, -190, -47, 3, 3, 40, matl));
  cryph::AffPoint p1(51.5, -188.5, -7);
  cryph::AffPoint p2(131.5, -188.5, 50);
  rail = BasicShape::makeBoundedCylinder(p1, p2, 2, 25, 2, BasicShape::CAP_AT_BOTH);
  railR = new BasicShapeRenderer(shaderIF, rail);

  models.push_back(new Block(shaderIF, 50, -150, -50, 11.4, -40, 3, matl)); /* stairs */
  models.push_back(new Block(shaderIF, 61.4, -150, -42, 11.4, -40, 3, matl));
  models.push_back(new Block(shaderIF, 72.8, -150, -34, 11.4, -40, 3, matl));
  models.push_back(new Block(shaderIF, 84.2, -150, -26, 11.4, -40, 3, matl));
  models.push_back(new Block(shaderIF, 95.6, -150, -18, 11.4, -40, 3, matl));
  models.push_back(new Block(shaderIF, 107, -150, -10, 11.4, -40, 3, matl));
  models.push_back(new Block(shaderIF, 118.4, -150, -2, 11.4, -40, 3, matl));



  models.push_back(new Block(shaderIF, 115, 146, 10, 3, 75, 100, red)); //back entry
  models.push_back(new Block(shaderIF, 115, 221, 10, 50, 3, 100, red));
  models.push_back(new Block(shaderIF, 165, 221, 10, -3, -75, 100, red));


  roofPieces.push_back(new TriPrism(shaderIF, gray, backEntryLoc, 15, 50, 135, false));
  roofPieces.push_back(new TriPrism(shaderIF, gray, extensLoc, 30, 420, 155, true));
  roofPieces.push_back(new Pyramid(shaderIF, r, 50, 185, 230, gray)); //roof
//  models.push_back(new Pyramid(shaderIF, r, 20, 50, 100, gray));
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
    if(models[i]->isVisible)
      models[i] -> render();

  for(int i=0; i<roofPieces.size(); i++)
    if(roofPieces[i]->isVisible)
      roofPieces[i] -> render();

  if(!dogHouse && (!drawingCottage))
    doorKnob -> render();

  establishMaterial();
  if(drawingCottage)
    railR->drawShape();
}

bool House::handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY)
{
  if(anASCIIChar == 'r')
  {
    std::cout<<"toggle roof\n";
    toggleVisibility(roofPieces);
    //GLFWController* c = dynamic_cast<GLFWController*>(Controller::getCurrentController());
    //c->handleDisplay();
  }
  else if(anASCIIChar == 'w')
  {
    std::cout<<"toggle translucent objects\n";
    toggleVisibility(translucents);
  }
  else if (anASCIIChar == 'O')
    ModelView::setProjection(ORTHOGONAL);
  else if (anASCIIChar == 'P')
    ModelView::setProjection(PERSPECTIVE);
  else if (anASCIIChar == 'Q')
    ModelView::setProjection(OBLIQUE);
  else
    return ModelView::handleCommand(anASCIIChar, ldsX, ldsY);

  Controller::getCurrentController()->redraw();
  return true;
}

void House::toggleVisibility(std::vector<SceneElement*> models)
{
  for(int i = 0; i < models.size(); i++)
    models[i]->isVisible = !models[i]->isVisible;

  render();
}
