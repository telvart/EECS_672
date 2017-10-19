// House.h

#ifndef HOUSE_H
#define HOUSE_H

#include "ModelView.h"
#include "ShaderIF.h"

class House : public ModelView
{
public:
	// As before: you will likely want to add parameters to the constructor
	House(ShaderIF* sIF, cryph::AffPoint houseBottom, float width, float length, float height);
	virtual ~House();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
	void defineHouse();
private:
	ShaderIF* shaderIF;
	ModelView* houseModels;

	float width, length, height;
	cryph::AffPoint m_bottom;

	float kd[3];
	float xyz[6];
};

#endif
