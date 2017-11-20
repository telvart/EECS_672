// House.h

#ifndef HOUSE_H
#define HOUSE_H

#include "SceneElement.h"
#include "SceneElement.h"
#include "ShaderIF.h"
#include "Block.h"
#include "Pyramid.h"
#include "TriPrism.h"
#include "GLFWController.h"
#include "BasicShapeRenderer.h"
#include <vector>

typedef float vec3[3];

class House : public SceneElement
{
public:
	// As before: you will likely want to add parameters to the constructor
	House(ShaderIF* sIF, cryph::AffPoint houseBottom, float width, float length, float height,
		 float roofHeight, bool isDog, PhongMaterial& mat);
	House(ShaderIF* sIF, PhongMaterial& mat);
	virtual ~House();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();

	void defineHouse();
	void defineCottage();
	bool handleCommand(unsigned char anASCIIChar, double ldsX, double ldsY);
private:

	void toggleVisibility(std::vector<SceneElement*> models);

	cryph::AffPoint m_bottom;
	Block *doorKnob;

	BasicShape* rail;
	BasicShapeRenderer* railR;

	std::vector<SceneElement*> models;
	std::vector<SceneElement*> roofPieces;
	std::vector<SceneElement*> translucents;

	bool dogHouse;
	bool drawingCottage;

	float wallWidth;
	float width, length, height, roofHeight;
	float xyz[6];

};

#endif
